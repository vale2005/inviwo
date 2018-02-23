/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Friday, October 20, 2017 - 12:20:21
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <dh2320lab3/dh2320lab3moduledefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

/** \class Util
    \brief Utilities for raytracing

    @author Himangshu Saikia
*/
class IVW_MODULE_DH2320LAB3_API Util {
    //Friends
    //Types
public:

    //Construction / Deconstruction
public:
    Util();
    virtual ~Util() = default;

    //Methods
public:
    static vec3 normalize(const vec3& v);
    static vec3 scalarMult(const double& s, const vec3& v);
    static void drawLineSegment(const vec3& v1, const vec3& v2, const vec4& color,
                                IndexBufferRAM* indexBuffer,
                                std::vector<BasicMesh::Vertex>& vertices);
    //Attributes
public:
    static const double epsilon;

};

} // namespace
