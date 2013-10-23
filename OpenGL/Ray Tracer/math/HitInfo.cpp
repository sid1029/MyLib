/* 
 * File:   HitInfo.cpp
 * Author: siddhesv
 * 
 * Created on March 25, 2012, 6:48 PM
 */

#include "HitInfo.hpp"

namespace _462 {

HitInfo::HitInfo() {
    
}
HitInfo::HitInfo(int sample) {
    sampleint = sample;
}

HitInfo::HitInfo(const HitInfo& orig) {
    this->intersection = orig.intersection;
    this->normal = orig.normal;
    this->pix_color = orig.pix_color;
    this->Ka = orig.Ka;
    this->Kd = orig.Kd;
    this->Ks = orig.Ks;
    this->tex_color = orig.tex_color;
    this->beta = orig.beta;
    this->gamma = orig.gamma;
    this->isTex = orig.isTex;
    this->texCoord = orig.texCoord;
}
/*
HitInfo::~HitInfo() {
}
*/
}