#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <iostream>

bool Application::IsRunning() { return running; }

void Application::Setup() { running = Graphics::OpenWindow(); }

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
      //         Body* ball = new Body(event.button.x, event.button.y, 1);
      //         ball->radius = 5;
      //         bodies.push_back(ball);
      //     };
      //     break;
      // case SDL_MOUSEMOTION:
      //     mouseCursor.x = event.motion.x;
      //     mouseCursor.y = event.motion.y;
      //     break;
      // case SDL_MOUSEBUTTONDOWN:
      //     if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
      //     {
      //         leftMouseButtonDown = true;
      //         int x, y;
      //         SDL_GetMouseState(&x, &y);
      //         mouseCursor.x = x;
      //         mouseCursor.y = y;
      //     }
      //     break;
      // case SDL_MOUSEBUTTONUP:
      //     if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
      //     {
      //         leftMouseButtonDown = false;
      //         Vec2 impulseDirection = (bodies[bobNumber-1]->position -
      //         mouseCursor).UnitVector(); float impulseMagnitude =
      //         (bodies[bobNumber-1]->position - mouseCursor).Magnitude()
      //         * 5.0; bodies[bobNumber-1]->velocity = impulseDirection *
      //         impulseMagnitude;
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

  // Apply forces to the bodies

  for (auto body : bodies) {
    // Add weight
    Vec2 weight = Vec2(0.0, 9.8 * body->mass * PIXELS_PER_METER);
    body->AddForce(weight);

    // Add pushForce
    body->AddForce(pushForce);

    // Add dragForce
    Vec2 drag = Force::GenerateDragForce(*body, 0.01);
    body->AddForce(drag);
  }

  // apply sprintForce to bobOne
  Vec2 springForceOne =
      Force::GenerateSpringForce(*bodies[0], anchor, restLength, k);
  bodies[0]->AddForce(springForceOne);

  for (int i = 1; i < bobNumber; i++) {
    Vec2 springForce = Force::GenerateSpringForce(
        *bodies[i], bodies[i - 1]->position, restLength, k);
    bodies[i]->AddForce(springForce);
    bodies[i - 1]->AddForce(-springForce);
  }

  for (auto body : bodies) {
    body->Integrate(deltaTime);
  }

  for (auto body : bodies) {
    // check body position, limit and keep the body inside of window...
    if (body->position.x - body->radius <= 0) {
      body->position.x = body->radius;
      body->velocity.x *= -0.9; // 0.9 for energy loss
    } else if (body->position.x + body->radius >= Graphics::Width()) {
      body->position.x = Graphics::Width() - body->radius;
      body->velocity.x *= -0.9;
    } else if (body->position.y - body->radius <= 0) {
      body->position.y = body->radius;
      body->velocity.y *= -0.9;
    } else if (body->position.y + body->radius >= Graphics::Height()) {
      body->position.y = Graphics::Height() - body->radius;
      body->velocity.y *= -0.9;
    }
  }
}

void Application::Render() {
  Graphics::ClearScreen(0xFF056263); // transparency R G B

  // Draw the anchor, bob, and spring
  Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
  Graphics::DrawLine(anchor.x, anchor.y, bodies[0]->position.x,
                     bodies[0]->position.y, 0xFF313131);
  for (int i = 1; i < bobNumber; i++) {
    Graphics::DrawLine(bodies[i - 1]->position.x, bodies[i - 1]->position.y,
                       bodies[i]->position.x, bodies[i]->position.y,
                       0xFF313131);
  }

  for (int i = 0; i < bobNumber; i++) {
    Graphics::DrawFillCircle(bodies[i]->position.x, bodies[i]->position.y,
                             bodies[i]->radius, 0xFFFFFFFF);
  }

  // if (leftMouseButtonDown) {
  //     Graphics::DrawLine(mouseCursor.x, mouseCursor.y,
  //     bodies[bobNumber-1]->position.x, bodies[bobNumber-1]->position.y,
  //     0xFF123456);
  // }

  // Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
  // Graphics::DrawFillCircle(bodies[0]->position.x, bodies[0]->position.y,
  // bodies[0]->radius, 0xFFFFFFFF);
  // Graphics::DrawFillCircle(bodies[1]->position.x, bodies[1]->position.y,
  // partipcles[1]->radius, 0xFFFFFFFF);
  // Graphics::DrawFillCircle(bodies[2]->position.x, bodies[2]->position.y,
  // bodies[2]->radius, 0xFFFFFFFF);
  // Graphics::DrawFillCircle(bodies[3]->position.x, bodies[3]->position.y,
  // bodies[3]->radius, 0xFFFFFFFF);

  Graphics::RenderFrame();
}

void Application::Destroy() {
  for (auto body : bodies) {
    delete body;
  }
  Graphics::CloseWindow();
}
