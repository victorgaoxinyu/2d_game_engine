#include "./Force.h"
#include <algorithm>

Vec2 Force::GenerateDragForce(const Particle& particle, float k) {
    Vec2 dragForce = Vec2(0, 0);

    if (particle.velocity.MagnitudeSquared() > 0) {
        // Calc drag direction
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;
        
        // Calc drag magnitude, k * |v|^2
        float dragMagnitude = k * particle.velocity.MagnitudeSquared();

        // generate final drag force
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float k) {
    Vec2 frictionForce = Vec2(0, 0);

    // Calc friction direction
    Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0;

    // Calc friction magnitude, for now..
    float frictionMagnitude = k;

    // generate final friction force
    frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Particle& a, const Particle&b, float G, float minDistance, float maxDistance) {
    /*
    Fg = G * m_a * m_b * d_hat / ||d||^2
    */
    Vec2 d = (b.position - a.position);

    float distanceSquared = d.MagnitudeSquared();

    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);

    Vec2 attractionDirection = d.UnitVector();
    float attractionMagnitude = G * a.mass * b.mass / distanceSquared;

    Vec2 attactionForce = attractionDirection * attractionMagnitude;

    return attactionForce;
}

// TODO: Generate Sprint Force
// ...
