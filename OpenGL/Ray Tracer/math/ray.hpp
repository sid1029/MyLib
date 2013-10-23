/* 
 * File:   ray.hpp
 * Author: siddhesv
 *
 * Created on March 25, 2012, 6:00 PM
 */

#ifndef _462_MATH_RAY_HPP_
#define	_462_MATH_RAY_HPP_

#include "math/math.hpp"
#include "math/vector.hpp"
#include "math/Intersection.hpp"
#include "math/HitInfo.hpp"

namespace _462 {


class Ray {

protected: // Protected attributes

    Vector3 origin; // Point of origin of the ray.
    Vector3 direction; // Normalized direction of the ray.

public: // Constructors and destructor
    Ray();
    Ray(const Ray& orig);
    Ray(const Vector3& o, const Vector3& d):origin(o),direction(d) {};
    
    //virtual ~Ray();
    
public: // Public methods:
    /**
      *@returns ray's origin
      */
    Vector3 getOrigin() const {return origin;}

    /**
      *@returns ray's direction
      */
    Vector3 getDirection() const {return direction;}

    /**
      *@param t time
      *@returns ray's position at given time
      */
    Vector3 positionAtTime(real_t t) const {
        return origin + (t * direction);
    }
    
    /**
      *@param t time
      *@returns Distance from ray-origin upto ray's position at given time
      */
    real_t distanceAtTime(real_t t) const {
        return length( ( (origin + (t * direction)) - origin ) );
    }

    /**
      *@returns Intersection info of this ray obtained by casting it in the
      * currently set direction from the current origin.
      */
    HitInfo cast (void);

};

}

#endif	/* _462_MATH_RAY_HPP */