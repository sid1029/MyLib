#include "physics/trianglebody.hpp"
#include <iostream>

namespace _462 {

TriangleBody::TriangleBody( Triangle* geom )
{
    vertices[0] = geom->vertices[0].position;
    vertices[1] = geom->vertices[1].position;
    vertices[2] = geom->vertices[2].position;
    orientation_phy = Quaternion::Identity;
    velocity = Vector3::Zero;
    normal_phy = normalize( cross(vertices[1] - vertices[0], vertices[2] - vertices[0]) );
}

Vector3 TriangleBody::step_position( real_t dt, real_t motion_damping )
{
    return Vector3::Zero;
}

Vector3 TriangleBody::step_orientation( real_t dt, real_t motion_damping )
{
    return Vector3::Zero;
}

void TriangleBody::apply_force( const Vector3& f, const Vector3& offset )
{
    return;
}

}
