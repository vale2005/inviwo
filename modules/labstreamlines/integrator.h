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

#include <labstreamlines/labstreamlinesmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo
{

class IVW_MODULE_LABSTREAMLINES_API Integrator
{ 
//Friends
//Types
public:

//Construction / Deconstruction
public:
    Integrator();
    virtual ~Integrator() = default;

//Methods
public:
    static vec2 sampleFromField(const VolumeRAM* vr, size3_t dims, const vec2& position);

    // TODO: Implement the methods below (one integration step with either Euler or 
    // Runge-Kutte of 4th order integration method)
    // Pass any other properties that influence the integration process
    // Examples would be the stepsize, inegreation direction, ...
    static vec2 rk4(const VolumeRAM* vr, size3_t dims, const vec2& position, float stepSize);
    
    //input position: [-8;8]x[-8;8] -> 
    //output:         [-8;8]x[-8;8], stepsize added
    static vec3 euler(const VolumeRAM* vr, size3_t dims, const vec3& position, float stepSize);
    
    
    //input v1,v2: [-8;8]x[-8;8] ->
    //converts to coordinates and draws them
    static void drawLineSegmentAndPoints(const vec3& v1, const vec3& v2,
                                         size3_t dims,
                                         IndexBufferRAM* indexBufferLines,
                                         IndexBufferRAM* indexBufferPoints,
                                         std::vector<BasicMesh::Vertex>& vertices);

    
    //input gridPoint: [-8;8]x[-8;8] -> 
    //output:         [0;1]x[0;1]
    static vec3 getCoordinates(size3_t dims, const vec3& gridPoint);

};

} // namespace
