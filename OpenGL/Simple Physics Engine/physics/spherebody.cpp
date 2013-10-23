#include "physics/spherebody.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "scene/sphere.hpp"
#include <iostream>
#include <exception>
#include <algorithm>

namespace _462 {

SphereBody::SphereBody( Sphere* geom )
{
    sphere = geom;
    position_phy = sphere->position_gra;
    radius = sphere->radius;
    orientation_phy = sphere->orientation_gra;
    mass = 0.0;
    velocity = Vector3::Zero;
    angular_velocity = Vector3::Zero;
    force = Vector3::Zero;
    torque = Vector3::Zero;
}

Vector3 SphereBody::step_position( real_t dt, real_t motion_damping )
{
    this->sphere->position_gra = this->position_phy;
    return this->sphere->position_gra;
}

Vector3 SphereBody::step_orientation( real_t dt, real_t motion_damping )
{
    this->sphere->orientation_gra = this->orientation_phy;
    return this->sphere->position_gra;
}

void SphereBody::apply_force( const Vector3& f, const Vector3& offset )
{
    Vector3 fDir = cross(offset, f);
    if ( (offset == Vector3::Zero) || (fDir == Vector3::Zero) ) {
        this->force += f;
    }
    else {
        //Split force into 2 components.
        Vector3 parr = this->position_phy - offset;
		this->force += ( dot(f, offset) / squared_length(offset) ) * offset;        
		// (0.4 * this->mass * this->radius * this->radius);
    }
}

}
