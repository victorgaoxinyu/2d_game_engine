#ifndef BODY_H
#define BODY_H

#include "Shape.h"
#include "Vec2.h"

struct Body
{
  bool isColliding = false;

  // Linear motion
  Vec2 position;
  Vec2 velocity;
  Vec2 acceleration;

  // Angular motion, scalar!
  float rotation;            /* theta */
  float angularVelocity;     /* omega */
  float angularAcceleration; /* alpha */

  // Forces and torque
  Vec2 sumForces;
  float sumTorque;

  // Mass and moment of intertia
  float mass;
  float invMass; // most physic engine store this for performance
  float I;
  float invI;

  // Coefficient of restitution (elasticity)
  float restitution;
  // Coefficient of friction
  float friction;
  
  // a pointer to the geometry shape of the rigid body
  Shape *shape = nullptr;

  Body(const Shape &shape, float x, float y, float mass);
  ~Body();

  bool IsStatic() const;

  void AddForce(const Vec2 &force);
  void AddTorque(float torque);
  void ClearForces();
  void ClearTorque();

  // Integration
  void Update(float dt);
  void IntegrateLinear(float dt);
  void IntegrateAngular(float dt);

  // Linear impulse on center of mass
  void ApplyImpulse(const Vec2 &j);
  // Linear + Angular impulse on contact point
  void ApplyImpulse(const Vec2 &j, const Vec2 &r);
};

#endif