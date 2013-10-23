/* 
 * File:   HitInfo.hpp
 * Author: siddhesv
 *
 * Created on March 25, 2012, 6:48 PM
 */

#ifndef _462_MATH_HITINFO_HPP
#define	_462_MATH_HITINFO_HPP


#include "math/math.hpp"
#include "math/vector.hpp"
#include "Intersection.hpp"
#include "math/color.hpp"

namespace _462 {

class HitInfo {

public:
    Vector3 intersection;
    Vector3 normal;
    Color3 pix_color, Ka, Kd, Ks, tex_color;
    real_t beta, gamma; // Barycentric co-ordinates returned from triangle intersection.
    bool isTex;
    Vector2 texCoord;
    
    HitInfo();
    HitInfo(int sampleint);
    HitInfo(const HitInfo& orig);
    //virtual ~HitInfo();
private:
    Vector3 hitPoint;
    int sampleint;
};

}
#endif	/* HITINFO_HPP */

