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
