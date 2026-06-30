#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>

bool Application::IsRunning() { return running; }

void Application::Setup()
{
  running = Graphics::OpenWindow();

  Body *bigBall = new Body(CircleShape(100), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 5.0);
  bodies.push_back(bigBall);

  // Body *smallBall = new Body(CircleShape(50), Graphics::Width() / 2.0, Graphics::Height() / 2.0 + 100, 1.0);
  // bodies.push_back(smallBall);

}
void Application::Input()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
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
    case SDL_MOUSEBUTTONDOWN:
      int x, y;
      SDL_GetMouseState(&x, &y);
      Body* smallBall = new Body(CircleShape(40), x, y, 1.0);
      smallBall->restitution = 0.9;
      bodies.push_back(smallBall);
      break;
    }
  }
}

void Application::Update()
{
  Graphics::ClearScreen(0xFF056263);
  // Check if we are too fast, and if so waste ms
  // until we reach MILLISECS_PER_FRAME
  static int timePreviousFrame;
  int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
  if (timeToWait > 0)
    SDL_Delay(timeToWait);

  float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
  if (deltaTime > 0.016)
  {
    deltaTime = 0.016;
  }

  // Set time of current frame to be used in next iter
  timePreviousFrame = SDL_GetTicks();

  bodies[0]->AddForce(pushForce);

  // // Apply forces to the bodies
  // for (auto body : bodies)
  // {
  //   // // Add weight
  //   Vec2 weight = Vec2(0.0, 9.8 * body->mass * PIXELS_PER_METER);
  //   body->AddForce(weight);

  //   // Add windForce
  //   Vec2 wind = Vec2(20.0 * PIXELS_PER_METER, 0.0);
  //   body->AddForce(wind);

  // }

  for (auto body : bodies)
  {
    body->Update(deltaTime);
  }

  for (auto body : bodies)
  {
    body->isColliding = false;
  }

  // Check all rigidbodies with the other rigidbodies for collision
  for (int i = 0; i <= bodies.size() - 1; i++)
  {
    for (int j = i + 1; j < bodies.size(); j++)
    {
      // check bodies[i] with bodies[j]
      Body* a = bodies[i];
      Body* b = bodies[j];
      Contact contact;
      if (CollisionDetection::IsColliding(a, b, contact))
      {

        contact.ResolveCollision();

        Graphics::DrawFillCircle(contact.start.x, contact.start.y, 5, 0xFFFF0000);
        Graphics::DrawFillCircle(contact.end.x, contact.end.y, 5, 0xFF00FF00);
        Graphics::DrawLine(contact.start.x, contact.start.y, contact.end.x, contact.end.y, 0xFFFFFFFF);

        a->isColliding = true;
        b->isColliding = true;
      }
    }
    
  }
  

  for (auto body : bodies)
  {
    if (body->shape->GetType() == CIRCLE)
    {
      CircleShape *circleShape = (CircleShape *)body->shape;
      // check body position, limit and keep the body inside of window...
      if (body->position.x - circleShape->radius <= 0)
      {
        body->position.x = circleShape->radius;
        body->velocity.x *= -0.9; // 0.9 for energy loss
      }
      else if (body->position.x + circleShape->radius >= Graphics::Width())
      {
        body->position.x = Graphics::Width() - circleShape->radius;
        body->velocity.x *= -0.9;
      }
      else if (body->position.y - circleShape->radius <= 0)
      {
        body->position.y = circleShape->radius;
        body->velocity.y *= -0.9;
      }
      else if (body->position.y + circleShape->radius >= Graphics::Height())
      {
        body->position.y = Graphics::Height() - circleShape->radius;
        body->velocity.y *= -0.9;
      }
    } else if (body->shape->GetType() == BOX)
    {
      BoxShape *boxShape = (BoxShape *)body->shape;
      if (body->position.x - boxShape->width / 2 <= 0)
      {
        body->position.x = boxShape->width / 2;
        body->velocity.x *= -0.9;
      }
      else if (body->position.x + boxShape->width / 2 >= Graphics::Width())
      {
        body->position.x = Graphics::Width() - boxShape->width / 2;
        body->velocity.x *= -0.9;
      }
      else if (body->position.y - boxShape->height / 2 <= 0)
      {
        body->position.y = boxShape->height / 2;
        body->velocity.y *= -0.9;
      }
      else if (body->position.y + boxShape->height / 2 >= Graphics::Height())
      {
        body->position.y = Graphics::Height() - boxShape->height / 2;
        body->velocity.y *= -0.9;
      }
    }
  }
}

void Application::Render()
{
  // Graphics::ClearScreen(0xFF056263); // transparency R G B

  for (auto body : bodies)
  {
    Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;
    if (body->shape->GetType() == CIRCLE)
    {
      CircleShape *circleShape = (CircleShape *)body->shape;
      Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
    } else if (body->shape->GetType() == BOX)
    {
      BoxShape *boxShape = (BoxShape *)body->shape;
      Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);
    }
  }


  Graphics::RenderFrame();
}

void Application::Destroy()
{
  for (auto body : bodies)
  {
    delete body;
  }
  Graphics::CloseWindow();
}
