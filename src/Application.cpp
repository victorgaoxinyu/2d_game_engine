#include "Application.h"
#include "./Physics/Constants.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();
    particle = new Particle(50, 100, 1.0);
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

    // Update objects in the scene
    particle->velocity = Vec2(100.0 * deltaTime, 30.0 * deltaTime);
    particle->position += particle->velocity;
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);  // transparency R G B
    Graphics::DrawFillCircle(particle->position.x, particle->position.y, 4, 0xFFFFFFFF);
    Graphics::RenderFrame();
}


void Application::Destroy() {
    // TOOD: destroy all objs
    delete particle;
    Graphics::CloseWindow();
}