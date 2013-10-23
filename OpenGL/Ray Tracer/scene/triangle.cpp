/**
 * @file triangle.cpp
 * @brief Function definitions for the Triangle class.
 *
 * @author Eric Butler (edbutler)
 */

#include "scene/triangle.hpp"
#include "application/opengl.hpp"

namespace _462 {

Triangle::Triangle()
{
    vertices[0].material = 0;
    vertices[1].material = 0;
    vertices[2].material = 0;
}

Triangle::~Triangle() { }

void Triangle::render() const
{
    bool materials_nonnull = true;
    for ( int i = 0; i < 3; ++i )
        materials_nonnull = materials_nonnull && vertices[i].material;

    // this doesn't interpolate materials. Ah well.
    if ( materials_nonnull )
        vertices[0].material->set_gl_state();

    glBegin(GL_TRIANGLES);

    glNormal3dv( &vertices[0].normal.x );
    glTexCoord2dv( &vertices[0].tex_coord.x );
    glVertex3dv( &vertices[0].position.x );

    glNormal3dv( &vertices[1].normal.x );
    glTexCoord2dv( &vertices[1].tex_coord.x );
    glVertex3dv( &vertices[1].position.x);

    glNormal3dv( &vertices[2].normal.x );
    glTexCoord2dv( &vertices[2].tex_coord.x );
    glVertex3dv( &vertices[2].position.x);

    glEnd();

    if ( materials_nonnull )
        vertices[0].material->reset_gl_state();
}

void FitRange ( int& sampling_x, int& sampling_y, real_t src_range_x, real_t src_range_y, int tar_range_x, int tar_range_y)
{
   sampling_x = floor((src_range_x * (tar_range_x)) + 0.5);
   sampling_y = floor((src_range_y * (tar_range_y)) + 0.5);
}

bool Triangle::Intersects(const Ray &r, real_t &result, HitInfo &info) const
{
    /*
    // Calculating Normal to triangle/plane containing triangle.
    Vector3 triNorm = cross(E1, E2);
    if ( triNorm == Vector3(0,0,0) ) // triangle is degenerate
        return false;

    w0 = r.getOrigin() - this->vertices[0].position;
    a = -dot(triNorm,w0);
    b = dot(triNorm,r.getDirection());
    if (fabs(b) < 0.00000001) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return false;
        else
            return false;          // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    res = a / b;
    if (res < 0.0)                   // ray goes away from triangle
        return false;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    Vector3 I = r.positionAtTime(res); // intersect point of ray and plane
    
    // is I inside T?
    real_t uu, uv, vv, wu, wv, D;
    uu = dot(E1,E1);
    uv = dot(E1,E2);
    vv = dot(E2,E2);
    w = I - this->vertices[0].position;
    wu = dot(w,E1);
    wv = dot(w,E2);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    real_t s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return false;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0) {  // I is outside T
        return false;
    }
    */

    // NEW--------------
    Vector3 p, q, s; // ray vectors
    real_t res, a, f;
    float alpha, beta, gamma;

    // Getting edges of triangle
    Vector3 E1 = this->vertices[1].position - this->vertices[0].position;
    Vector3 E2 = this->vertices[2].position - this->vertices[0].position;
    
    p = cross(r.getDirection(), E2);
    a = dot(E1, p);

    if ((a > -EPSILON) && (a < EPSILON))
        return false;

    f = 1.0/a;
    s = r.getOrigin() - this->vertices[0].position;
    beta = f * dot(s, p);
    if ( (beta < 0.0) || (beta > 1.0) )
        return false;
    q = cross(s, E1);
    gamma = f * dot(r.getDirection(), q);
    if ( (gamma < 0.0) || ( (beta + gamma) > 1.0 ) )
        return false;
    res = f * dot(E2, q);
    // NEW--------------

    /* Test is successfull at this point : return relevant hit info and true to raytracer. */
    // Returning t of this test.
    result = res;
    if (result < 0.01)
            return false;
    
    /* Fill up the Hit Info of this intersection with with relevant data. */
    alpha = (1 - beta - gamma);
    info.beta = beta;
    info.gamma = gamma;
    int tex_width = 0, tex_ht = 0;
    this->vertices[1].material->get_texture_size(&tex_width, &tex_ht);
    info.Ka = this->vertices[0].material->ambient * alpha + this->vertices[1].material->ambient * beta + this->vertices[2].material->ambient * gamma;
    info.Kd = this->vertices[0].material->diffuse * alpha + this->vertices[1].material->diffuse * beta + this->vertices[2].material->diffuse * gamma;
    info.Ks = this->vertices[0].material->specular * alpha + this->vertices[1].material->specular * beta + this->vertices[2].material->specular * gamma;
    
    info.normal = normalize( this->vertices[0].normal + beta * (this->vertices[1].normal - this->vertices[0].normal) + gamma * (this->vertices[2].normal - this->vertices[0].normal) );

    if (tex_width!=0 || tex_ht!=0) {
        
        Vector2 tex_Coord = (this->vertices[0].tex_coord * alpha) + (this->vertices[1].tex_coord * beta) + (this->vertices[2].tex_coord * gamma);
        int sample_coord_x, sample_coord_y;
        
        this->vertices[0].material->get_texture_size(&tex_width, &tex_ht);
        FitRange(sample_coord_x, sample_coord_y, tex_Coord.x, tex_Coord.y, tex_width, tex_ht);
        Color3 col1 = this->vertices[0].material->get_texture_pixel(sample_coord_x, sample_coord_y);

        this->vertices[1].material->get_texture_size(&tex_width, &tex_ht);
        FitRange(sample_coord_x, sample_coord_y, tex_Coord.x, tex_Coord.y, tex_width, tex_ht);
        Color3 col2 = this->vertices[1].material->get_texture_pixel(sample_coord_x, sample_coord_y);

        this->vertices[2].material->get_texture_size(&tex_width, &tex_ht);
        FitRange(sample_coord_x, sample_coord_y, tex_Coord.x, tex_Coord.y, tex_width, tex_ht);
        Color3 col3 = this->vertices[2].material->get_texture_pixel(sample_coord_x, sample_coord_y);
        
        info.tex_color = col1 * col2 * col3;
    }
    else {
		info.tex_color = Color3::White;
    }
    
    return true;
    
    /*
    info.pix_color = (this->vertices[0].material->diffuse * (1 - s - t) +
                    this->vertices[1].material->diffuse * (s) +
                    this->vertices[2].material->diffuse * (t)) +

                    (this->vertices[0].material->ambient * (1 - s - t) +
                    this->vertices[1].material->ambient * (s) +
                    this->vertices[2].material->ambient * (t)) +
            
                    (this->vertices[0].material->specular * (1 - s - t) +
                    this->vertices[1].material->specular * (s) +
                    this->vertices[2].material->specular * (t));
    */
    
    /*
    real_t a,b,c,d,e,f,g,h,i,j,k,l,beta,gamma,M;

    a = vertices[0].position.x - vertices[1].position.x;
    b = vertices[0].position.y - vertices[1].position.y;
    c = vertices[0].position.z - vertices[1].position.z;
    d = vertices[0].position.x - vertices[2].position.x;
    e = vertices[0].position.y - vertices[2].position.y;
    f = vertices[0].position.z - vertices[2].position.z;

    g = r.getDirection().x;
    h = r.getDirection().y;
    i = r.getDirection().z;

    j = vertices[0].position.x - r.getOrigin().x;
    k = vertices[0].position.y - r.getOrigin().y;
    l = vertices[0].position.z - r.getOrigin().z;

    real_t ei_minus_hf = e*i - h*f;
    real_t gf_minus_di = g*f - d*i;
    real_t dh_minus_eg = d*h - e*g;
    real_t ak_minus_jb = a*k - j*b;
    real_t jc_minus_al = j*c - a*l;
    real_t bl_minus_kc = b*l - k*c;
    
    M = a*ei_minus_hf + b*gf_minus_di + c*dh_minus_eg;
    result = -(f*ak_minus_jb + e*jc_minus_al + d*bl_minus_kc) / M;

    if( ( result < 0 ) || ( result > 50000 ) ) { //no intersection
        return false;
    }

    gamma = (i*ak_minus_jb + h*jc_minus_al + g*bl_minus_kc) / M;
    
    if(gamma<0.0 || gamma>1.0) {//no intersection
        return false;
    }

    beta = (j*ei_minus_hf + k*gf_minus_di + l*dh_minus_eg) / M;
    
    if(beta<0.0 || beta>(1.0-gamma)) { //no intersection
        return false;
    }
    
    return true;
    */
}

} /* _462 */