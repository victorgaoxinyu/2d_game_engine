#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Body.h"
#include <vector>

class Application {
private:
  bool running = false;
  bool leftMouseButtonDown = false;

  std::vector<Body *> bodies;
  Vec2 pushForce = Vec2(0, 0);
  Vec2 mouseCursor;

  Vec2 anchor;
  float k = 400;
  float restLength = 30;
  int bobNumber = 15;

  // SDL_Rect liquid;

public:
  Application() = default;
  ~Application() = default;
  bool IsRunning();
  void Setup();
  void Input();
  void Update();
  void Render();
  void Destroy();
};

#endif
