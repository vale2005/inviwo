/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Wednesday, September 20, 2017 - 12:04:15
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <lablic/lablicmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/datastructures/volume/volume.h>
#include <functional>

namespace inviwo {

class IVW_MODULE_LABLIC_API Integrator {
// Friends
// Types
public:
// Construction / Deconstruction
public:
    Integrator();
    virtual ~Integrator() = default;

// Methods
public:

    static vec2 sampleFromField(const Volume* vol, size3_t dims, const vec2& position, bool isNormalized);

    static vec2 rk4(const Volume* vol, size3_t dims, const vec2& position, float stepSize);
    
    //input position: 16x16 -> 
    //output:         16x16, stepsize added
    static vec2 euler(const Volume* vol, size3_t dims, const vec2& position, float stepSize);
    
    
    //input v1,v2: 16x16 ->
    //draws line based on dimensions
    static void drawLineSegmentAndPoints(const vec2& v1, const vec2& v2,
                                         size3_t dims,
                                         IndexBufferRAM* indexBufferLines,
                                         IndexBufferRAM* indexBufferPoints,
                                         std::vector<BasicMesh::Vertex>& vertices,
                                         const vec4& color);
};

}  // namespace inviwo
