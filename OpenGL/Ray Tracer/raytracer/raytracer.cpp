/**
 * @file raytacer.cpp
 * @brief Raytracer class
 *
 * Implement these functions for project 2.
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "raytracer.hpp"
#include "scene/scene.hpp"
#include "math/Ray.hpp"
#include "math/HitInfo.hpp"

#include <SDL/SDL_timer.h>
#include <iostream>
#include <fstream>

// Recursion depth for reflected and refracted rays.
#define REC_DEPTH 3

namespace _462 {
    
Vector3 camPos, look, left, right, up, s;
//Matrix4 *tran, *inv_tran;

real_t d, pix_width, pix_height;
real_t scrX, scrY, t0 = 0, t1 = 5000;
/*
 * Horizontal view frustum half-angle.
 */
real_t theta_x;

/*
 * Vertical view frustum half-angle.
 */
real_t theta_y;

Raytracer::Raytracer()
    : scene( 0 ), width( 0 ), height( 0 ) { }

Raytracer::~Raytracer() {
    d = 0;
    up = Vector3(0,0,0);
    look = Vector3(0,0,0);
    left = Vector3(0,0,0);
    right = Vector3(0,0,0);
    camPos = Vector3(0,0,0);

    theta_x = 0;
    theta_y = 0;
}

inline real_t maxnum(real_t num1, real_t num2) {
   return (num1 >= num2) ? num1 : num2;
}

Color3 Raytracer::PhongShade( const Scene* scene, HitInfo& hit ) {
    return hit.Ka + hit.Kd + hit.Kd;
}

/**
 * helper function to scale a number from a given source range to a given target range.
 * @param num The number to scale.
 * @param srange_lower The lower limit of the range that the number belongs to.
 * @param srange_higher The higher limit of the range that the number belongs to.
 * @param trange_lower The lower limit of the range that the number is to be fit into.
 * @param trange_higher The higher limit of the range that the number is to be fit into.
 * @return num fit into the target range.
 */
inline real_t FitRange (real_t num, real_t srange_lower, real_t srange_higher, real_t trange_lower, real_t trange_higher) {
    return (real_t)( trange_lower + ( ( (num - srange_lower) / (srange_higher - srange_lower) ) * (trange_higher - trange_lower) ));
}

/**
 * Initializes the raytracer for the given scene. Overrides any previous
 * initializations. May be invoked before a previous raytrace completes.
 * @param scene The scene to raytrace.
 * @param width The width of the image being raytraced.
 * @param height The height of the image being raytraced.
 * @return true on success, false on error. The raytrace will abort if
 *  false is returned.
 */
bool Raytracer::initialize( Scene* scene, size_t width, size_t height )
{
    this->scene = scene;
    this->width = width;
    this->height = height;
    current_row = 0;

    /* Setting up basic camera vectors and quantities for reuse */
    d = (real_t)(height*0.5)/tan(scene->camera.get_fov_radians()*0.5);
    up = normalize( scene->camera.get_up() );
    look = ( scene->camera.get_direction() );
    left = ( cross( scene->camera.get_up() , scene->camera.get_direction() ) );
    right = cross( scene->camera.get_direction(), scene->camera.get_up() );
    camPos = scene->camera.get_position();
    theta_x = scene->camera.get_fov_radians() * scene->camera.get_aspect_ratio() * 0.5;
    theta_y = scene->camera.get_fov_radians() * 0.5;
    pix_width = ( ( d * tan(theta_x)) / width );
    pix_height = ( ( d * tan(theta_y)) / height );

    #pragma region Print Values
    std::cout<<"Look Direction : "<<look<<std::endl;
    std::cout<<"Cam Up : "<< up <<std::endl;
    std::cout<<"Right : "<<right<<std::endl;
    std::cout<<"Left : "<<left<<std::endl;
    std::cout<<"Aspect ratio : "<<scene->camera.get_aspect_ratio()<<std::endl;

    std::cout<<"Material count : "<<this->scene->num_materials()<<std::endl;
    std::cout<<"Mesh count : "<<this->scene->num_meshes()<<std::endl;
    std::cout<<"Light count : "<<this->scene->num_lights()<<std::endl;
    std::cout<<"Geometries count : "<<this->scene->num_geometries()<<std::endl;
    std::cout<<"BG Color : "<<this->scene->background_color<<std::endl;
    std::cout<<"Ambient Light : "<<this->scene->ambient_light<<std::endl;
    #pragma endregion Printing out basic camera and scene values

    if (this->scene->num_meshes() == 0) {
        // Only trace for procedurally generated objects (eg. sphere, plane etc..).
        
    }
    else {
        // Trace for everything. (procedurally generated + .obj meshes)
    }

    tran = new Matrix4[scene->num_geometries()];
    inv_tran = new Matrix4[scene->num_geometries()];
    trans_norm = new Matrix3[scene->num_geometries()];

    /* Construct transformation and inverse transformation matrices for all geometries in scene so they can be reused wherever needed */
    Geometry* const* geometry = scene->get_geometries();
    for (size_t i = 0; i < scene->num_geometries(); i++) {
        make_transformation_matrix( &tran[i], geometry[i]->position, geometry[i]->orientation, geometry[i]->scale );
        make_inverse_transformation_matrix( &inv_tran[i], geometry[i]->position, geometry[i]->orientation, geometry[i]->scale );
        make_normal_matrix(&trans_norm[i], tran[i]);
    }
    
    return true;
}

/**
 * Performs a raytrace on the given pixel on the current scene.
 * The pixel is relative to the bottom-left corner of the image.
 * @param scene The scene to trace.
 * @param x The x-coordinate of the pixel to trace.
 * @param y The y-coordinate of the pixel to trace.
 * @param width The width of the screen in pixels.
 * @param height The height of the screen in pixels.
 * @return The color of that pixel in the final image.
 */
Color3 Raytracer::trace_pixel( const Scene* scene, Ray& ray, int depth )
{
    real_t t = 0, collT = 5000;
    Geometry* obj = NULL;
    size_t objIdx = 0;
    
    Vector3 collPos;
    HitInfo hInfo, objInfo;
    Ray localSpaceRay;

    Color3 finalColor = Color3::Black;
    Color3 ambientColor = Color3::Black;
    Color3 sigmaColor = Color3::Black;

    Geometry* const* geom = scene->get_geometries();
    
    /* Intersecting eyeray iterating through all geometries. */
    for (size_t i = 0; i < scene->num_geometries(); ++i) {

        // Inverse transforming the world space ray's origin and direction to convert them to local space.
        Vector3 trans_ray_orig = inv_tran[i].transform_point(ray.getOrigin());
        Vector3 trans_ray_dir = inv_tran[i].transform_vector(ray.getDirection());

        localSpaceRay = Ray(trans_ray_orig, trans_ray_dir);
        
        if (geom[i]->Intersects(localSpaceRay, t, hInfo)) { // Eye Rays Intersection test.
            if (t < collT) { // Select the closest intersection among all the objects that this ray collides with.
                objInfo = hInfo; // Store the hitinfo for color computation later
                collT = t;
                obj = geom[i];
                objIdx = i;
                ambientColor = hInfo.Ka * scene->ambient_light;
            }
        }
        // else no intersection occured... so return background color.
        else {
            finalColor = scene->background_color;
        }
    }

    // In case an intersection occured above, cast shadow, reflected and refracted rays to compute final color of pixel.
    if (obj != NULL) {
        HitInfo shadowHitInfo;
        real_t shadowT = 0;
        collPos = localSpaceRay.positionAtTime(collT);

        /* Transforming the object space normal and direction from the intersection above 
		 * into world space normal and direction.
		 */
        Vector3 worldNorm = normalize (trans_norm[objIdx] * hInfo.normal);
        tran[objIdx].transform_point(collPos);

        // Iterating over all lights in the scene, adding contribution from each color to the final color.
        for (size_t li = 0; li < scene->num_lights(); ++li) {
            // Checking for visibility of intersection point from position of point light by iterating over all geometries in scene.
            for (size_t g = 0; g < scene->num_geometries(); ++g) {
                /*
                 * Constructing the shadow ray that originates at point of intersection and travels
                 * in the direction of current point light.
                 */
                //std::cout<<scene->get_lights()[li].position;
                Ray shadowRay = Ray( collPos, normalize (scene->get_lights()[li].position - collPos) ); // Shadow Ray
                if (!geom[g]->Intersects(shadowRay, shadowT, shadowHitInfo)) { // If no intersection occurs.
                                                                               // Account for intensity of this light.
                    
                    // Calculating attenuation factor for this light.
					real_t lDist = distance(scene->get_lights()[li].position, collPos);
                    
                    Color3 tempLightCol = scene->get_lights()[li].color * ( 1.0/
                                                                            (scene->get_lights()[li].attenuation.constant +
                                                                            scene->get_lights()[li].attenuation.linear * lDist +
                                                                            scene->get_lights()[li].attenuation.quadratic * lDist * lDist
                                                                            ) );
                    //tempLightCol *= hInfo.Kd * maxnum( dot(worldNorm,shadowRay.getDirection()), 0.0 );
                    
                    // Adding up contribution from this light.
                    sigmaColor += tempLightCol * hInfo.Kd * maxnum( dot(worldNorm, shadowRay.getDirection()), 0.0 );
                }
            }
        }
        
        // Recursive Rays : Reflected and refracted rays.
        if (depth < REC_DEPTH) {
            // Calculating reflected ray direction using the above calculated normal.
            real_t comp = -dot( worldNorm, ray.getDirection() );
            Vector3 Reflect = ray.getDirection() - (2 * worldNorm * comp );
            
			Ray reflRay = Ray( collPos, Reflect);
            // Casting recursive reflection ray. add light + tex
            finalColor = ( ambientColor + sigmaColor ) * hInfo.tex_color;
            //if (sigmaColor != Color3::Black)
                //std::cout<<sigmaColor;
            //finalColor *= trace_pixel(scene, reflRay, depth+1 );
        }
    }
    
    return finalColor;
    //return Color3(fabs(ray.getDirection().x), fabs(ray.getDirection().x), fabs(ray.getDirection().x));
}

/**
 * Raytraces some portion of the scene. Should raytrace for about
 * max_time duration and then return, even if the raytrace is not copmlete.
 * The results should be placed in the given buffer.
 * @param buffer The buffer into which to place the color data. It is
 *  32-bit RGBA (4 bytes per pixel), in row-major order.
 * @param max_time, If non-null, the maximum suggested time this
 *  function raytrace before returning, in seconds. If null, the raytrace
 *  should run to completion.
 * @return true if the raytrace is complete, false if there is more
 *  work to be done.
 */
bool Raytracer::raytrace( unsigned char *buffer, real_t* max_time )
{
    // TODO Add any modifications to this algorithm, if needed.
    
    static const size_t PRINT_INTERVAL = 64;

    // the time in milliseconds that we should stop
    unsigned int end_time = 0;
    bool is_done;

    if ( max_time ) {
        // convert duration to milliseconds
        unsigned int duration = (unsigned int) ( *max_time * 1000 );
        end_time = SDL_GetTicks() + duration;
    }

    // until time is up, run the raytrace. we render an entire row at once
    // for simplicity and efficiency.
    for ( ; !max_time || end_time > SDL_GetTicks(); ++current_row ) {

        if ( current_row % PRINT_INTERVAL == 0 ) {
            std::cout<<"Raytracing (row "<<current_row<<")...\n";
        }

        // we're done if we finish the last row
        is_done = current_row == height;
        // break if we finish
        if ( is_done )
            break;
        for ( size_t x = 0; x < width; ++x ) {
            assert( 0 <= x && x < width );
            assert( 0 <= current_row && current_row < height );

            //Calculate eye ray directions.
            Vector3 nDir = normalize( ( d * look ) +
                                      ( (current_row - height * 0.5) * up ) +
                                      ( (x - width*0.5) * right ) );
            // Contructing the eye ray object.
            Ray eyeRay = Ray(camPos, nDir);
            //std::cout<<"Before : "<<eyeRay.getOrigin()<<"  "<<eyeRay.getDirection()<<std::endl;
            // trace a pixel
            Color3 color = trace_pixel( scene, eyeRay, 0 );
            // write the result to the buffer, always use 1.0 as the alpha
            color.to_array( &buffer[4 * ( current_row * width + x )] );
        }
    }

    if ( is_done ) {
        printf( "Done raytracing!\n" );
    }

    return is_done;
}

} /* _462 */

