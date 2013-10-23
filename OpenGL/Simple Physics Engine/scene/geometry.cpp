#include "scene/geometry.hpp"

namespace _462 {

Geometry::Geometry():
    position_gra( Vector3::Zero ),
    orientation_gra( Quaternion::Identity ),
    scale( Vector3::Ones )
{

}

Geometry::~Geometry() { } 

}
