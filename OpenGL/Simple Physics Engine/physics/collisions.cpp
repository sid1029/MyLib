#include "physics/collisions.hpp"

namespace _462 {

inline Vector3 damp (Vector3 vel, real_t d) {
    return ( length( vel - d * vel ) > EPSILON ) ? ( vel - d * vel ) : Vector3::Zero;
    //return vel - d * vel;
}

bool collides( SphereBody& body1, SphereBody& body2, real_t collision_damping )
{
    register real_t dist = length(body2.position_phy - body1.position_phy);

    if ( fabs(dist) < ( body1.radius + body2.radius ) ) {
        Vector3 v1 = body1.velocity;
        Vector3 v2 = body2.velocity;
        Vector3 v1_d = v1 - v2;
        Vector3 d = ( body2.position_phy - body1.position_phy ) / dist;
        Vector3 v2_dd = 2.0 * d * ( body1.mass / (body1.mass + body2.mass) ) * dot(v1_d, d);
        
        body2.velocity = body2.velocity + v2_dd;
        body1.velocity = (body1.mass * v1 + body2.mass * v2 - body2.mass * body2.velocity) / body1.mass;

        body1.velocity = damp(body1.velocity, collision_damping);
        body2.velocity = damp(body2.velocity, collision_damping);
        return true;
    }
    else {
        return false;
    }
}

bool collides( SphereBody& body1, TriangleBody& body2, real_t collision_damping )
{
    // TODO detect collision. If there is one, update velocity
    register real_t dist =  dot( (body1.position_phy - body2.position_phy), (body2.normal_phy) );
    if ( fabs(dist) < body1.radius ) {

        // Compute projection of sphere center on triangle plane.
        Vector3 pd = body1.position_phy - dist * body2.normal_phy;

        // Compute barycentric coordinates
        Vector3 v0 = body2.vertices[2] - body2.vertices[0];
        Vector3 v1 = body2.vertices[1] - body2.vertices[0];
        Vector3 v2 = pd - body2.vertices[0];

        real_t dot00 = dot(v0, v0);
        real_t dot01 = dot(v0, v1);
        real_t dot02 = dot(v0, v2);
        real_t dot11 = dot(v1, v1);
        real_t dot12 = dot(v1, v2);

        real_t invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
        real_t beta = (dot11 * dot02 - dot01 * dot12) * invDenom;
        real_t gamma = (dot00 * dot12 - dot01 * dot02) * invDenom;

        // Check if point is in triangle
        if ( (beta >= 0) && (gamma >= 0) && (beta + gamma < 1) ) {
            Vector3 newVel = body1.velocity - 2  * ( dot(body1.velocity, body2.normal_phy) ) * body2.normal_phy;
            body1.velocity = damp(newVel, collision_damping);
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool collides( SphereBody& body1, PlaneBody& body2, real_t collision_damping )
{
    if ( fabs( dot( (body1.position_phy - body2.position_phy), (body2.normal) ) ) < body1.radius ) {
        body1.velocity = body1.velocity - 2 * ( dot(body1.velocity, body2.normal) ) * body2.normal;
        body1.velocity = damp(body1.velocity, collision_damping);
        return true;
    }
    else {
        return false;
    }
}

}
