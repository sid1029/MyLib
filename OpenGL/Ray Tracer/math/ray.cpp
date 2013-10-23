/* 
 * @file:   ray.cpp
 * @brief:  Generic Ray class.
 * @author: siddhesv
 * 
 * Created on March 25, 2012, 6:00 PM
 */

#include "math/Ray.hpp"
#include "math/vector.hpp"
#include "HitInfo.hpp"

namespace _462 {


Ray::Ray() {
}

Ray::Ray(const Ray& orig) {
}

/*
Ray::~Ray() {
}
*/
HitInfo cast () {
    HitInfo hi = HitInfo(30);
    /*
    Vector3 dist = s.pos - r.start;
    double B = r.dir * dist;
    double D = B*B - dist * dist + s.size * s.size; 
    if (D < 0.0f) 
        return false; 
    double t0 = B - sqrt(D); 
    double t1 = B + sqrt(D);
    bool retvalue = false;  
    if ((t0 > 0.1f) && (t0 < t)) 
    {
        t = (float)t0;
        retvalue = true; 
    } 
    if ((t1 > 0.1f) && (t1 < t)) 
    {
        t = (float)t1; 
        retvalue = true; 
    }
    return
    */
    return hi;
}

}