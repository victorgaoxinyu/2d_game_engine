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

    if (aIsPolygon && bIsCircle) {
        return IsCollidingPolygonCircle(a, b, contact);
    }

    if (aIsCircle && bIsPolygon) {
        return IsCollidingPolygonCircle(b, a, contact);
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

    contact.a = a;
    contact.b = b;

    if (abSeparation > baSeparation) {
        contact.depth = -abSeparation;
        contact.normal = aAxis.Normal();
        contact.start = aPoint;
        contact.end = aPoint + contact.normal * contact.depth;
    } else {
        contact.depth = -baSeparation;
        contact.normal = -bAxis.Normal();
        contact.start = bPoint - contact.normal * contact.depth;
        contact.end = bPoint;
    }
    return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body *polygon, Body *circle, Contact& contact) {
    // Find nearest edge
    // find the 
    const PolygonShape* polygonShape = (PolygonShape*) polygon->shape;
    const std::vector<Vec2>& polygonVertices = polygonShape->worldVertices;

    Vec2 minCurrVertex;
    Vec2 minNextVertex;

    for (int i = 0; i < polygonVertices.size(); i++) {
        int currVertex = i;
        int nextVertex = (i + 1) % polygonVertices.size();
        Vec2 edge = polygonShape->EdgeAt(currVertex);
        Vec2 normal = edge.Normal();

        // Compare the circle center with the rectangle vertex
        Vec2 circleCenter = circle->position - polygonVertices[currVertex];

        // Project the circle center on to the edge normal
        float projection = circleCenter.Dot(normal);

        // if we found a dot product projection that is in the positive side of normal
        if (projection > 0) {
            minCurrVertex = polygonShape->worldVertices[currVertex];
            minNextVertex = polygonShape->worldVertices[nextVertex];
            break;
        }
    }
    Graphics::DrawFillCircle(minCurrVertex.x, minCurrVertex.y, 5, 0xFF00FFFF);
    Graphics::DrawFillCircle(minNextVertex.x, minNextVertex.y, 5, 0xFF00FFFF);

    return false;
}
