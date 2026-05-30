#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();
    
    Particle* smallBall = new Particle(200, 200, 2.0);
    smallBall->radius = 5;
    particles.push_back(smallBall);

    Particle* bigBall = new Particle(500, 500, 20.0);
    bigBall->radius = 20;
    particles.push_back(bigBall);

    // liquid.x = 0;
    // liquid.y = Graphics::Height() / 2;
    // liquid.w = Graphics::Width();
    // liquid.h = Graphics::Height() / 2;
}


void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                break;
            // Click to create more balls
            // case SDL_MOUSEBUTTONDOWN:
            //     if (event.button.button == SDL_BUTTON_LEFT) {
            //         Particle* ball = new Particle(event.button.x, event.button.y, 1);
            //         ball->radius = 5;
            //         particles.push_back(ball);
            //     };
            //     break;
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
                    particles[0]->velocity = impulseDirection * impulseMagnitude;
                }
                break;
        }
    }
}


void Application::Update() {
    // Check if we are too fast, and if so waste ms
    // until we reach MILLISECS_PER_FRAME
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
        SDL_Delay(timeToWait);
    
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016) {
        deltaTime = 0.016;
    }
 
    // Set time of current frame to be used in next iter
    timePreviousFrame = SDL_GetTicks();

    // Apply forces to the particles

    for (auto particle: particles) {
        // Add weight
        // Vec2 weight = Vec2(0.0, 9.8 * particle->mass * PIXELS_PER_METER);
        // particle->AddForce(weight);
    
        // Add pushForce
        // particle->AddForce(pushForce);

        // Add frictionForce
        Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);
        particle->AddForce(friction);
        
        // // apply dragForce if inside of liquid
        // if (particle->position.y >= liquid.y) {
        //     Vec2 drag = Force::GenerateDragForce(*particle, 0.03);
        //     particle->AddForce(drag);
        // } else {
        //     // Add windForce
        //     Vec2 wind = Vec2(1.0 * PIXELS_PER_METER, 0.0);
        //     particle->AddForce(wind);
        // }
    }

    // Add gravitationalForce to two particles
    Vec2 attraction = Force::GenerateGravitationalForce(*particles[0], *particles[1], 4000.0, 5, 100);
    particles[0]->AddForce(attraction);
    particles[1]->AddForce(-attraction);

    for (auto particle: particles) {
        particle->Integrate(deltaTime);
    }
    
    for (auto particle: particles) {
    // check particle position, limit and keep the particle inside of window...
        if (particle->position.x - particle->radius <= 0) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9;  // 0.9 for energy loss
        } else if (particle->position.x + particle->radius >= Graphics::Width()) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.9;
        } else if (particle->position.y - particle->radius <= 0) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9;
        } else if (particle->position.y + particle->radius >= Graphics::Height()) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.9;
        }
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);  // transparency R G B

    // Draw the liquid, x, y here is the center of DrawFillRect :P
    // Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFFF56042);
    // if (leftMouseButtonDown == true) {
    //     Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFF4254F5);
    // }

    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFF0000FF);
    Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFF00FFFF);
    

    // for (auto particle: particles) {
    //     Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    // }
    Graphics::RenderFrame();
}


void Application::Destroy() {
    for (auto particle: particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}
