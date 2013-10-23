/**
 * @file sphere.cpp
 * @brief Function defnitions for the Sphere class.
 *
 * @author Kristin Siu (kasiu)
 * @author Eric Butler (edbutler)
 */
#include <math.h>
#include "scene/sphere.hpp"
#include "application/opengl.hpp"

namespace _462 {

#define SPHERE_NUM_LAT 80
#define SPHERE_NUM_LON 100

#define SPHERE_NUM_VERTICES ( ( SPHERE_NUM_LAT + 1 ) * ( SPHERE_NUM_LON + 1 ) )
#define SPHERE_NUM_INDICES ( 6 * SPHERE_NUM_LAT * SPHERE_NUM_LON )
// index of the x,y sphere where x is lat and y is lon
#define SINDEX(x,y) ((x) * (SPHERE_NUM_LON + 1) + (y))
#define VERTEX_SIZE 8
#define TCOORD_OFFSET 0
#define NORMAL_OFFSET 2
#define VERTEX_OFFSET 5

static unsigned int Indices[SPHERE_NUM_INDICES];
static float Vertices[VERTEX_SIZE * SPHERE_NUM_VERTICES];

static void init_sphere()
{
    static bool initialized = false;
    if ( initialized )
        return;

    for ( int i = 0; i <= SPHERE_NUM_LAT; i++ ) {
        for ( int j = 0; j <= SPHERE_NUM_LON; j++ ) {
            real_t lat = real_t( i ) / SPHERE_NUM_LAT;
            real_t lon = real_t( j ) / SPHERE_NUM_LON;
            float* vptr = &Vertices[VERTEX_SIZE * SINDEX(i,j)];

            vptr[TCOORD_OFFSET + 0] = lon;
            vptr[TCOORD_OFFSET + 1] = 1-lat;

            lat *= PI;
            lon *= 2 * PI;
            real_t sinlat = sin( lat );

            vptr[NORMAL_OFFSET + 0] = vptr[VERTEX_OFFSET + 0] = sinlat * sin( lon );
            vptr[NORMAL_OFFSET + 1] = vptr[VERTEX_OFFSET + 1] = cos( lat ),
            vptr[NORMAL_OFFSET + 2] = vptr[VERTEX_OFFSET + 2] = sinlat * cos( lon );
        }
    }

    for ( int i = 0; i < SPHERE_NUM_LAT; i++ ) {
        for ( int j = 0; j < SPHERE_NUM_LON; j++ ) {
            unsigned int* iptr = &Indices[6 * ( SPHERE_NUM_LON * i + j )];

            unsigned int i00 = SINDEX(i,  j  );
            unsigned int i10 = SINDEX(i+1,j  );
            unsigned int i11 = SINDEX(i+1,j+1);
            unsigned int i01 = SINDEX(i,  j+1);

            iptr[0] = i00;
            iptr[1] = i10;
            iptr[2] = i11;
            iptr[3] = i11;
            iptr[4] = i01;
            iptr[5] = i00;
        }
    }

    initialized = true;
}

Sphere::Sphere()
    : radius(0), material(0) {}

Sphere::~Sphere() {}

// returns distance, 0 if nohit
bool Sphere::Intersects(const Ray &r, real_t &result, HitInfo &info) const
{
    //Compute A, B and C coefficients
    //float a = dot(r.getDirection(), r.getDirection());
    float a = 1;
    float b = 2 * dot(r.getDirection(), r.getOrigin());
    float c = dot(r.getOrigin(), r.getOrigin()) - (this->radius * this->radius);

    //Find discriminant
    float disc = b * b - 4 * a * c;
    
    // if discriminant is negative there are no real roots, so return
    // false as ray misses sphere
    if (disc < 0)
        return false;

    // compute q as described above
    float distSqrt = sqrtf(disc);
    float q;
    if (b < 0)
        q = (-b - distSqrt)/2.0;
    else
        q = (-b + distSqrt)/2.0;

    // compute the 2 solutions
    real_t sol0 = q / a;
    real_t sol1 = c / q;

    // make sure soln0 is smaller than soln1
    if (sol0 > sol1)
    {
        float temp = sol0;
        sol0 = sol1;
        sol1 = temp;
    }
    
    // if t1 is less than zero, the object is in the ray's negative direction
    // and consequently the ray misses the sphere
    if (sol1 < 0)
        return false;

    // Test successfull.

    // Fill up the Hit Info of this intersection with with relevant data.
    info.pix_color = this->material->ambient + this->material->diffuse + this->material->specular;
    info.Ka = this->material->ambient;
    info.Kd = this->material->diffuse;
    info.Ks = this->material->specular;

    
    // If theres no texture
    if (this->material->texture_filename == "") {
        // just return diffuse color
        info.pix_color = this->material->diffuse;
        info.Ka = this->material->ambient;
        info.Kd = this->material->diffuse;
        info.Ks = this->material->specular;
    }
    else {
        // else Return texture
        info.pix_color = this->material->get_texture_pixel(5,5);
    }
    // Return nearest intersection solution.
    // if t0 is less than zero, the intersection point is at t1
    if (sol0 < 0){
        result = sol1;
        if (result < 0.01)
            return false;
		info.normal = normalize (r.positionAtTime(result) - Vector3(0,0,0) );
        return true;
    }
    // else the intersection point is at t0
    else {
        result = sol0;
        if (result < 0.01)
            return false;
        info.normal = normalize (r.positionAtTime(result) - Vector3(0,0,0));
        return true;
    }
    /*
    //Compute A, B and C coefficients
    Vector3 eminusc = this->position - r.getOrigin();
    //real_t a = 1;
    //float a = dot(r.getDirection(), r.getDirection());
    real_t b = 2 * dot( r.getDirection(), eminusc);
    real_t c = dot(eminusc , eminusc) - (this->radius * this->radius);

    //Find discriminant
    real_t disc = b * b - (4.0 * c);
    
    // if discriminant is negative there are no real roots, so return 
    // false as ray misses sphere
    if (disc < 0) {
        result = 0;
        return false;
    }
    // compute sqrt of discriminant as described above
    real_t distSqrt = sqrtf(disc);
    t0 = (real_t)(-b - distSqrt)/(2.0);
    
    if (t0 > 0) {
        result = t0;
        info.surf_normal = ( r.positionAtTime(result) - this->position );
        info.pix_color = this->material->diffuse;
        return true;
    }
    else {
        t1 = (real_t)(-b + distSqrt)/(2.0);
        if (t1 > 0) {
            result = t1;
            return true;
        }
        else {
            result = 0;
            return false;
        }
    }
    */
}

void Sphere::render() const
{
    // create geometry if we haven't already
    init_sphere();

    if ( material )
        material->set_gl_state();

    // just scale by radius and draw unit sphere
    glPushMatrix();
    glScaled( radius, radius, radius );
    glInterleavedArrays( GL_T2F_N3F_V3F, VERTEX_SIZE * sizeof Vertices[0], Vertices );
    glDrawElements( GL_TRIANGLES, SPHERE_NUM_INDICES, GL_UNSIGNED_INT, Indices );
    glPopMatrix();

    if ( material )
        material->reset_gl_state();
}


} /* _462 */

