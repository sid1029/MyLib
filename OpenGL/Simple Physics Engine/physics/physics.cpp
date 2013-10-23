#include <iostream>

#include "physics/physics.hpp"

namespace _462 {

Physics::Physics()
{
    reset();
}

Physics::~Physics()
{
    reset();
}

Vector3 vel = Vector3::Zero;

real_t onebysix = (1.0/6.0), onebythree = (1.0/3.0);

void Physics::step( real_t dt )
{
    // Iterate through all spheres checking collisions against every other geometry.
    for (size_t i = 0; i < this->num_spheres(); i++) {
        this->spheres[i]->force = Vector3::Zero;

        // Apply Gravity to centre of mass.
        this->spheres[i]->apply_force(this->gravity * this->spheres[i]->mass, Vector3::Zero);

        // Collisions
        register size_t j;
        for (j = 0; j < this->num_planes(); j++) {
            collides( *this->spheres[i], *this->planes[j], this->collision_damping );
        }

        for (j = 0; j < this->num_spheres(); j++) {
            if (i == j) continue; // Skip self
            collides( *this->spheres[i], *this->spheres[j], this->collision_damping );
        }

        for (j = 0; j < this->num_triangles(); j++) {
            collides( *this->spheres[i], *this->triangles[j], this->collision_damping );
        }

        // Iterate through all springs.
        for (j = 0; j< this->num_springs(); j++) {
            this->springs[i]->step(dt);
        }
		std::cout<<this->spheres[i]->force<<std::endl;
        Vector3 accn  = this->spheres[i]->force / this->spheres[i]->mass;

        Vector3 K1_vel = dt * accn;
        Vector3 K2_vel = dt * ( accn + ( K1_vel * 0.5) );
        Vector3 K3_vel = dt * ( accn + ( K2_vel * 0.5) );
        Vector3 K4_vel = dt * ( accn + K3_vel );
        
        Vector3 K1_pos = dt * this->spheres[i]->velocity;
        Vector3 K2_pos = dt * ( this->spheres[i]->velocity + ( K1_pos * 0.5) );
        Vector3 K3_pos = dt * ( this->spheres[i]->velocity + ( K2_pos * 0.5) );
        Vector3 K4_pos = dt * ( this->spheres[i]->velocity + K3_pos );

        this->spheres[i]->position_phy += ( onebysix * K1_pos + onebythree * K2_pos + onebythree * K3_pos + onebysix * K4_pos );
        this->spheres[i]->velocity += ( onebysix * K1_vel + onebythree * K2_vel + onebythree * K3_vel + onebysix * K4_vel );

        real_t Vel_mag = length(this->spheres[i]->angular_velocity);
        Quaternion Ang_vel_quat = Quaternion(this->spheres[i]->angular_velocity / Vel_mag, dt * Vel_mag);

//        Quaternion K1_rot = dt * Ang_vel_quat;
//        Quaternion K2_rot = dt * ( Ang_vel_quat * ( K1_rot * 0.5) );
//        Quaternion K3_rot = dt * ( Ang_vel_quat * ( K2_rot * 0.5) );
//        Quaternion K4_rot = dt * ( Ang_vel_quat * K3_rot );
//
//        Quaternion RK_rot = ( onebysix * K1_rot * (1.0/3.0) * K2_rot * (1.0/3.0) * K3_rot * (1.0/6.0) * K4_rot );

        this->spheres[i]->orientation_phy = this->spheres[i]->orientation_phy * Ang_vel_quat;
        normalize(this->spheres[i]->orientation_phy);
        
        this->spheres[i]->step_position( dt, this->collision_damping );
        this->spheres[i]->step_orientation( dt, this->collision_damping );
    }
}

void Physics::add_sphere( SphereBody* b ) {
    spheres.push_back( b );
}

size_t Physics::num_spheres() const
{
    return spheres.size();
}

void Physics::add_plane( PlaneBody* p )
{
    planes.push_back( p );
}

size_t Physics::num_planes() const
{
    return planes.size();
}

void Physics::add_triangle( TriangleBody* t )
{
    triangles.push_back( t );
}

size_t Physics::num_triangles() const
{
    return triangles.size();
}

void Physics::add_spring( Spring* s )
{
    springs.push_back( s );
}

size_t Physics::num_springs() const
{
    return springs.size();
}

void Physics::reset()
{
    for ( SphereList::iterator i = spheres.begin(); i != spheres.end(); i++ ) {
        delete *i;
    }
    for ( PlaneList::iterator i = planes.begin(); i != planes.end(); i++ ) {
        delete *i;
    }
    for ( TriangleList::iterator i = triangles.begin(); i != triangles.end(); i++ ) {
        delete *i;
    }
    for ( SpringList::iterator i = springs.begin(); i != springs.end(); i++ ) {
        delete *i;
    }

    spheres.clear();
    planes.clear();
    triangles.clear();
    springs.clear();
    
    gravity = Vector3::Zero;
    collision_damping = 0.0;
}

}
