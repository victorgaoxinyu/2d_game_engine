#ifndef BODY_H
#define BODY_H

#include "Shape.h"
#include "Vec2.h"

struct Body {
  // Linear motion
  Vec2 position;
  Vec2 velocity;
  Vec2 acceleration;

  // Angular motion
  float rotation;
  float angularVelocity;
  float angularAcceleration;

  // Forces and torque
  Vec2 sumForces;
  float sumTorque;

  // Mass and moment of intertia
  float mass;
  float invMass; // most physic engine store this for performance
  float I;
  float invI;

  // a pointer to the geometry shape of the rigid body
  Shape *shape = nullptr;

  Body(const Shape &shape, float x, float y, float mass);
  ~Body();

  void AddForce(const Vec2 &force);
  void AddTorque(float torque);
  void ClearForces();
  void ClearTorque();

  // Integration
  void IntegrateLinear (float dt);
  void IntegrateAngular(float dt);
};

#endif