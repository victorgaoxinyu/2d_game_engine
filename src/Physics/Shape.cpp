#include "Shape.h"
#include "Vec2.h"
#include <iostream>
#include <vector>

CircleShape::CircleShape(float radius) {
  this->radius = radius;
  std::cout << "CircleShape constructor called!" << std::endl;
};

CircleShape::~CircleShape() {
  std::cout << "CircleShape destructor called!" << std::endl;
};

ShapeType CircleShape::GetType() const { return CIRCLE; }

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
  std::cout << "PolygonShape constructor called!" << std::endl;
};

PolygonShape::~PolygonShape() {
  std::cout << "PolygonShape destructor called!" << std::endl;
}

ShapeType PolygonShape::GetType() const { return POLYGON; }

BoxShape::BoxShape(float width, float height) {
  std::cout << "PolygonShape constructor called!" << std::endl;
};

BoxShape::~BoxShape() {
  std::cout << "PolygonShape destructor called!" << std::endl;
}

ShapeType BoxShape::GetType() const { return BOX; };
