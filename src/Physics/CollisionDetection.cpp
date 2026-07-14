#include "CollisionDetection.h"
#include "Contact.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact) {
    ShapeType aShape = a->shape->GetType();
    ShapeType bShape = b->shape->GetType();

    bool aIsCircle = aShape == CIRCLE;
    bool bIsCircle = bShape == CIRCLE;
    bool aIsPolygon = aShape == POLYGON || aShape == BOX;
    bool bIsPolygon = bShape == POLYGON || bShape == BOX;

    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contact);
    }

    if (aIsPolygon && bIsPolygon) {
        return IsCollidingPolygonPolygon(a, b, contact);
    }

    return false;
};

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact) {
    CircleShape* aCircleShape = (CircleShape*) a->shape;
    CircleShape* bCircleShape = (CircleShape*) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding) {
        return false;
    }

    // collision!

    // Compute contact collision info
    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();

    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;

    contact.depth = (contact.end - contact.start).Magnitude();

    return true;
};

float FindMinSeparation(const PolygonShape& a, const PolygonShape& b) {
    // TODO:
    // loop all vertices of "a"
    //   find normal axis
    //   loop all vertices of "b"
    //     project vertex b onto normal axis
    //     track of the min separation
    // return best separation outof all axis

    float separation = std::numeric_limits<float>::lowest();

    for (int i = 0; i < a.worldVertices.size(); i++) {
        Vec2 va = a.worldVertices[i];
        Vec2 normal = a.EdgeAt(i).Normal();

        float minSep = std::numeric_limits<float>::max();

        for (int j = 0; j < b.worldVertices.size(); j++) {
            Vec2 vb = b.worldVertices[j];
            minSep = std::min(minSep, (vb - va).Dot(normal));
        }
        separation = std::max(separation, minSep);
    }
    return separation;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact) {
    // TODO: find the separation between a and b, and b and a
    const PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b->shape;

    if (FindMinSeparation(*aPolygonShape, *bPolygonShape) >= 0) {
        return false;
    }

    if (FindMinSeparation(*bPolygonShape, *aPolygonShape) >= 0) {
        return false;
    }

    return true;
}
