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

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact) {
    const PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b->shape;
    Vec2 aAxis, bAxis;
    Vec2 aPoint, bPoint;
    float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aAxis, aPoint);

    if (abSeparation >= 0) {
        return false;
    }

    float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape, bAxis, bPoint);

    if (baSeparation >= 0) {
        return false;
    }
    // TODO: populate the contact information
    // contact.depth, contact.start, contact.end, contact.normal
    return true;
}
