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

    // vrel is linear+angular of a MINUS linear+angular of b
    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;
    // this is how we calc cross product in 2D, omega x ra
    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);

    const Vec2 vrel = va - vb;

    // relative velocity along normal direction 
    float vrelDotNormal = vrel.Dot(normal);

    // collision impulse
    const Vec2 impluseDirection = normal;
    const float impluseMagnitude = - (1 + e) * vrelDotNormal / 
                                (
                                    a->invMass 
                                    + b->invMass
                                    + (ra.Cross(normal) * ra.Cross(normal) * a->invI)
                                    + (rb.Cross(normal) * rb.Cross(normal) * b->invI)
                                );

    Vec2 jn = impluseDirection * impluseMagnitude;

    // appply impulse vector to both objects in opposite direction
    a->ApplyImpulse(jn, ra);
    b->ApplyImpulse(-jn, rb);
}