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
        
        static vec2 rk4(const Volume* vol, const vec2& position, float stepSize);
        static std::vector<vec2> getStreamlinePoints(const Volume* vol, size2_t dims, int kernel, vec2 startPoint, float stepSize);
        
    };
    
}  // namespace inviwo
