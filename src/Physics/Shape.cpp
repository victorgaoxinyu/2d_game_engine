#include "Shape.h"
#include "Vec2.h"
#include <iostream>
#include <vector>

CircleShape::CircleShape(float radius)
{
  this->radius = radius;
  std::cout << "CircleShape constructor called!" << std::endl;
};

CircleShape::~CircleShape()
{
  std::cout << "CircleShape destructor called!" << std::endl;
};

Shape *CircleShape::Clone() const
{
  return new CircleShape(radius);
};

float CircleShape::GetMomentOfInertia() const
{
  // For solid circle, 1/2 * r^2 * mass
  // this needs to be multiplied by mass when using
  return 0.5 * (radius * radius);
};

ShapeType CircleShape::GetType() const { return CIRCLE; }

PolygonShape::PolygonShape(const std::vector<Vec2> vertices)
{
  std::cout << "PolygonShape constructor called!" << std::endl;
};

PolygonShape::~PolygonShape()
{
  std::cout << "PolygonShape destructor called!" << std::endl;
}

Shape *PolygonShape::Clone() const
{
  return new PolygonShape(localVertices);
};

ShapeType PolygonShape::GetType() const { return POLYGON; }

float PolygonShape::GetMomentOfInertia() const
{
  // TODO:
  return 0.0;
};

Vec2 PolygonShape::EdgeAt(int index) const {
  int currVertex = index;
  int nextVertex = (index + 1) % worldVertices.size();

  return worldVertices[nextVertex] - worldVertices[currVertex];
}

float PolygonShape::FindMinSeparation(const PolygonShape* other) const {
    // TODO:
    // loop all vertices of "this" polygon
    //   find normal axis
    //   loop all vertices of "other" polygon
    //     project vertex b onto normal axis
    //     track of the min separation
    // return best separation outof all axis

    float separation = std::numeric_limits<float>::lowest();

    for (int i = 0; i < this->worldVertices.size(); i++) {
        Vec2 va = this->worldVertices[i];
        Vec2 normal = this->EdgeAt(i).Normal();

        float minSep = std::numeric_limits<float>::max();

        for (int j = 0; j < other->worldVertices.size(); j++) {
            Vec2 vb = other->worldVertices[j];
            minSep = std::min(minSep, (vb - va).Dot(normal));
        }
        separation = std::max(separation, minSep);
    }
    return separation;
}

void PolygonShape::UpdateVertices(float angle, const Vec2& position)
{
  // loop all vertices, transform from local to world
  for (int i = 0; i < localVertices.size(); i++)
  {
    worldVertices[i] = localVertices[i].Rotate(angle);
    worldVertices[i] += position;
  }

}

BoxShape::BoxShape(float width, float height)
{
  this->width = width;
  this->height = height;

  // load the vertices of the box polygon
  // local space
  localVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
  localVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
  localVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
  localVertices.push_back(Vec2(-width / 2.0, +height / 2.0));

  worldVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
  worldVertices.push_back(Vec2(+width / 2.0, -height / 2.0));
  worldVertices.push_back(Vec2(+width / 2.0, +height / 2.0));
  worldVertices.push_back(Vec2(-width / 2.0, +height / 2.0));

  std::cout << "PolygonShape constructor called!" << std::endl;
};

BoxShape::~BoxShape()
{
  std::cout << "PolygonShape destructor called!" << std::endl;
};

Shape *BoxShape::Clone() const
{
  return new BoxShape(width, height);
};

ShapeType BoxShape::GetType() const { return BOX; };

float BoxShape::GetMomentOfInertia() const
{
  // For rectangle, 1/12 * (w^2 + h^2) * mass
  // this needs to be multiplied by mass when using
  return (0.0833333) * (width * width + height * height);
}