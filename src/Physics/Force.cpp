#include "./Force.h"


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

// TODO: Generate Friction Force
// TODO: Generate Sprint Force
// ...
