/**
 * @file model.cpp
 * @brief Model class
 *
 * @author Eric Butler (edbutler)
 * @author Zeyang Li (zeyangl)
 */

#include "scene/model.hpp"
#include "scene/material.hpp"
#include <GL/gl.h>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>


namespace _462 {

Model::Model() : mesh( 0 ), material( 0 ) { }
Model::~Model() { }

void Model::render() const
{
    if ( !mesh )
        return;
    if ( material )
        material->set_gl_state();
    mesh->render();
    if ( material )
        material->reset_gl_state();
}

bool Model::Intersects(const Ray &r, real_t &result, HitInfo &info) const
{
    bool isIntersect = true;
    for (size_t i = 0; i < this->mesh->num_triangles(); i++) {
        Vector3 w0, w; // ray vectors
        real_t res, a, b; // params to calc ray-plane intersect

        // Getting edges of triangle
        Vector3 E1 = this->mesh->get_vertices()[this->mesh->get_triangles()[i].vertices[1]].position - this->mesh->get_vertices()[this->mesh->get_triangles()[i].vertices[0]].position;
        Vector3 E2 = this->mesh->get_vertices()[this->mesh->get_triangles()[i].vertices[2]].position - this->mesh->get_vertices()[this->mesh->get_triangles()[i].vertices[0]].position;

        // Calculating Normal to triangle plane.
        Vector3 triNorm = cross(E1, E2);
        if ( triNorm == Vector3(0,0,0) ) // triangle is degenerate
            isIntersect = false;

        w0 = r.getOrigin() - this->mesh->get_vertices()[this->mesh->get_triangles()[i].vertices[0]].position;
        a = -dot(triNorm,w0);
        b = dot(triNorm,r.getDirection());
        if (fabs(b) < 0.00000001) {     // ray is parallel to triangle plane
            if (a == 0)                // ray lies in triangle plane
                isIntersect = false;
            else
                isIntersect = false;          // ray disjoint from plane
        }

        // get intersect point of ray with triangle plane
        res = a / b;
        if (res < 0.0)                   // ray goes away from triangle
            isIntersect = false;                  // => no intersect
        // for a segment, also test if (r > 1.0) => no intersect

        Vector3 I = r.positionAtTime(res); // intersect point of ray and plane

        // is I inside T?
        real_t uu, uv, vv, wu, wv, D;
        uu = dot(E1,E1);
        uv = dot(E1,E2);
        vv = dot(E2,E2);
        w = I - this->mesh->get_vertices()[this->mesh->get_triangles()[i].vertices[0]].position;
        wu = dot(w,E1);
        wv = dot(w,E2);
        D = uv * uv - uu * vv;

        // get and test parametric coords
        real_t s, t;
        s = (uv * wv - vv * wu) / D;
        if (s < 0.0 || s > 1.0)        // I is outside T
            isIntersect = false;
        t = (uv * wu - uu * wv) / D;
        if (t < 0.0 || (s + t) > 1.0)  // I is outside T
            isIntersect = false;

        result = res;
        // Fill up the Hit Info of this intersection with with relevant data.
        info.beta = s;
        info.gamma = t;
        info.pix_color = Color3(0.5,0.6,0.0);
        isIntersect = false; // I is in T
    }
    if (!isIntersect)
        return false;
    else
        return true;
}

} /* _462 */

