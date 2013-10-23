/**
 * @file raytacer.hpp
 * @brief Raytracer class
 *
 * Implement these functions for project 2.
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#ifndef _462_RAYTRACER_HPP_
#define _462_RAYTRACER_HPP_

#include "math/color.hpp"
#include "math/vector.hpp"
#include "math/HitInfo.hpp"
#include "math/matrix.hpp"
#include "math/Ray.hpp"

namespace _462 {

class Scene;

class Raytracer
{
public:

    Raytracer();

    ~Raytracer();

    bool initialize( Scene* scene, size_t width, size_t height );

    Color3 PhongShade( const Scene* scene, HitInfo& hit );
    
    Color3 trace_pixel( const Scene* scene, Ray& ray, int depth );

    bool raytrace( unsigned char* buffer, real_t* max_time );

    Matrix4* tran;
    Matrix4* inv_tran;
    Matrix3* trans_norm;
private:

    // the scene to trace
    Scene* scene;

    // the dimensions of the image to trace
    size_t width, height;

    // the next row to raytrace
    size_t current_row;
};

} /* _462 */

#endif /* _462_RAYTRACER_HPP_ */

