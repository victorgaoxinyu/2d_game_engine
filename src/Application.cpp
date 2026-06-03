#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();
    
    // anchor--k1--bobOne--k2--bobTwo--k3--bobThree--k4--bobFour
    anchor = Vec2(Graphics::Width() / 2.0, 50);

    for (int i = 1; i <= bobNumber; i++) {
        Particle* bob = new Particle(Graphics::Width() / 2.0, 50 + restLength * i, 2.0);
        bob->radius = 5;
        particles.push_back(bob);
    }
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
            // case SDL_MOUSEMOTION:
            //     mouseCursor.x = event.motion.x;
            //     mouseCursor.y = event.motion.y;
            //     break;
            // case SDL_MOUSEBUTTONDOWN:
            //     if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
            //         leftMouseButtonDown = true;
            //         int x, y;
            //         SDL_GetMouseState(&x, &y);
            //         mouseCursor.x = x;
            //         mouseCursor.y = y;
            //     }
            //     break;
            // case SDL_MOUSEBUTTONUP:
            //     if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
            //         leftMouseButtonDown = false;
            //         Vec2 impulseDirection = (particles[bobNumber-1]->position - mouseCursor).UnitVector();
            //         float impulseMagnitude = (particles[bobNumber-1]->position - mouseCursor).Magnitude() * 5.0;
            //         particles[bobNumber-1]->velocity = impulseDirection * impulseMagnitude;
            //     }
            //     break;
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
        Vec2 weight = Vec2(0.0, 9.8 * particle->mass * PIXELS_PER_METER);
        particle->AddForce(weight);
    
        // Add pushForce
        particle->AddForce(pushForce);

        // Add dragForce
        Vec2 drag = Force::GenerateDragForce(*particle, 0.01);
        particle->AddForce(drag);
    }

    // apply sprintForce to bobOne
    Vec2 springForceOne = Force::GenerateSpringForce(*particles[0], anchor, restLength, k);
    particles[0]->AddForce(springForceOne);

    for (int i = 1; i < bobNumber; i++) {
        Vec2 springForce = Force::GenerateSpringForce(*particles[i], particles[i-1]->position, restLength, k);
        particles[i]->AddForce(springForce);
        particles[i-1]->AddForce(-springForce);
    }


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

    // Draw the anchor, bob, and spring
    Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
    Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131);
    for (int i = 1; i < bobNumber; i ++) {
        Graphics::DrawLine(particles[i-1]->position.x, particles[i-1]->position.y, particles[i]->position.x, particles[i]->position.y, 0xFF313131);
    }

    for (int i = 0; i < bobNumber; i ++) {
        Graphics::DrawFillCircle(particles[i]->position.x, particles[i]->position.y, particles[i]->radius, 0xFFFFFFFF);
    }

    // if (leftMouseButtonDown) {
    //     Graphics::DrawLine(mouseCursor.x, mouseCursor.y, particles[bobNumber-1]->position.x, particles[bobNumber-1]->position.y, 0xFF123456);
    // }

    // Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
    // Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFFFFFFF);
    // Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, partipcles[1]->radius, 0xFFFFFFFF);
    // Graphics::DrawFillCircle(particles[2]->position.x, particles[2]->position.y, particles[2]->radius, 0xFFFFFFFF);
    // Graphics::DrawFillCircle(particles[3]->position.x, particles[3]->position.y, particles[3]->radius, 0xFFFFFFFF);

    Graphics::RenderFrame();
}


void Application::Destroy() {
    for (auto particle: particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}
