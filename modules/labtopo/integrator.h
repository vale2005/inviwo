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

#include <labtopo/labtopomoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/datastructures/volume/volume.h>
#include <functional>

namespace inviwo {

class IVW_MODULE_LABTOPO_API Integrator {
// Friends
// Types
public:
// Construction / Deconstruction
public:
    Integrator();
    virtual ~Integrator() = default;

// Methods
public:
    static vec2 rk4(const Volume* vol, const vec2& position, float stepSize);

    static std::vector<vec2> getWholeStreamlinePoints(const Volume* vol, vec2 startPoint, float stepSize);
      
};

}  // namespace inviwo
