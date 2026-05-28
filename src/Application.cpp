#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();
    
    Particle* smallBall = new Particle(50, 100, 1);
    smallBall->radius = 10;
    particles.push_back(smallBall);

    // Particle* bigBall = new Particle(100, 100, 3.0);
    // bigBall->radius = 30;
    // particles.push_back(bigBall);

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;
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
        // Add windForce
        // Vec2 wind = Vec2(1.0 * PIXELS_PER_METER, 0.0);
        // particle->AddForce(wind);

        // Add weight
        Vec2 weight = Vec2(0.0, 9.8 * particle->mass * PIXELS_PER_METER);
        particle->AddForce(weight);
    
        // Add pushForce
        // particle->AddForce(pushForce);

        // TODO: apply dragForce if inside of liquid
        if (particle->position.y >= liquid.y) {
            Vec2 drag = Force::GenerateDragForce(*particle, 0.00001);
            particle->AddForce(drag);
            std::cout
                << "velY: " << particle->velocity.y
                << " dragY: " << drag.y
                << " gravityY: " << weight.y
                << std::endl;
        }

        particle->Integrate(deltaTime);
    
 
    // TOOD:
    // check particle position, limit and keep the particle inside of window...
    // 
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
    Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFFF56042);

    for (auto particle: particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}


void Application::Destroy() {
    for (auto particle: particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}
