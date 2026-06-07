#include "Body.h"
#include <iostream>

Body::Body(const Shape &shape, float x, float y, float mass)
{
  this->shape = shape.Clone();
  this->position = Vec2(x, y);
  this->velocity = Vec2(0, 0);
  this->acceleration = Vec2(0, 0);
  this->rotation = 0.0;
  this->angularVelocity = 0.0;
  this->angularAcceleration = 0.0;
  this->sumForces = Vec2(0, 0);
  this->sumTorque = 0.0;
  this->mass = mass;

  if (mass != 0.0)
  {
    this->invMass = 1.0 / mass;
  }
  else
  {
    this->invMass = 0.0;
  }

  this->I = shape.GetMomentOfInertia() * mass;

  if (this->I != 0.0)
  {
    this->invI = 1.0 / this->I;
  }
  else
  {
    this->invI = 0.0;
  }

  std::cout << "Body constructor called!" << std::endl;
}

Body::~Body()
{
  delete shape;
  std::cout << "Body destructor called!" << std::endl;
}

void Body::AddForce(const Vec2 &force) { sumForces += force; }

void Body::AddTorque(float torque) { sumTorque += torque; }

void Body::ClearForces() { sumForces = Vec2(0.0, 0.0); }

void Body::ClearTorque() { sumTorque = 0.0; }

void Body::IntegrateLinear(float dt)
{
  acceleration = sumForces * invMass;
  velocity += acceleration * dt;
  position += velocity * dt;
  ClearForces();
};

void Body::IntegrateAngular(float dt)
{
  angularAcceleration = sumTorque * invI;
  angularVelocity += angularAcceleration * dt;
  rotation += angularVelocity * dt;
  ClearTorque();
};

void Body::Update(float dt)
{
  IntegrateLinear(dt);
  IntegrateAngular(dt);
  bool isPolygon = shape->GetType() == POLYGON || shape->GetType() == BOX;
  if (isPolygon)
  {
    PolygonShape* polygonShape = (PolygonShape *)shape;
    polygonShape->UpdateVertices(rotation, position);
  }
}
