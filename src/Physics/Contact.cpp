#include "Contact.h"


void Contact::ResolvePenetration() {

    if (a->IsStatic() && b->IsStatic()) {
        return;
    }
    

    float da = depth / (a->invMass + b->invMass) * a->invMass;
    float db = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * da;
    b->position += normal * db;
};

// Resolved Collision using the impluse method
// J_n = - (1 + e) * (v_rel * normal) / (1/m_a + 1/m_b)
void Contact::ResolveCollision() {
    // Apply positional correction using projection method
    ResolvePenetration();

    // Elasticity
    float e = std::min(a->restitution, b->restitution);

    // relative velocity
    const Vec2 vrel = a->velocity - b->velocity;

    // relative velocity along normal direction 
    float vrelDotNormal = vrel.Dot(normal);

    // collision impulse
    const Vec2 impluseDirection = normal;
    const float impluseMagnitude = - (1 + e) * vrelDotNormal / (a->invMass + b->invMass);

    Vec2 j = impluseDirection * impluseMagnitude;

    // appply impulse vector to both objects in opposite direction
    a->ApplyImpulse(j);
    b->ApplyImpulse(-j);
}