#ifndef BODY_H
#define BODY_H

#include "Shape.h"
#include "Vec2.h"

struct Body {
  int radius;

  // Linear motion
  Vec2 position;
  Vec2 velocity;
  Vec2 acceleration;

  // TODO: Angular motion

  Vec2 sumForces;

  float mass;    // unit?
  float invMass; // most physic engine store this for performance

  // a pointer to the geometry shape of the rigid body
  Shape *shape = NULL;

  Body(const Shape &shape, float x, float y, float mass);
  ~Body();

  void AddForce(const Vec2 &force);
  void ClearForces();

  void Integrate(float dt);
};

#endif