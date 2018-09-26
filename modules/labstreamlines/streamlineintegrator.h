/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, September 19, 2017 - 15:08:33
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labstreamlines/labstreamlinesmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/ports/meshport.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <cmath>

namespace inviwo {

/** \docpage{org.inviwo.StreamlineIntegrator, Streamline Integrator}
    ![](org.inviwo.StreamlineIntegrator.png?classIdentifier=org.inviwo.StreamlineIntegrator)

    Processor to integrate streamlines.

    ### Inports
    * __data__ The input here is 2-dimensional vector field (with vectors of
    two components thus two values within each voxel) but it is represented
    by a 3-dimensional volume.
    This processor deals with 2-dimensional data only, therefore it is assumed
    the z-dimension will have size 1 otherwise the 0th slice of the volume
    will be processed.

    ### Outports
    * __outMesh__ The output mesh contains linesegments (no points apart from
    the start point) for the two streamlines computed with different integration
    schemes and both starting at a given start point

    ### Properties
    * __propSeedMode__ Mode for the number of seeds, either a single start point or
    multiple
    * __propStartPoint__ Location of the start point with x-coordinate in
    [0, number of voxels in x - 1] and y-coordinate in [0, number of voxels in y - 1]
    * __mouseMoveStart__ Move the start point when a selected mouse button is pressed
    (default left)
*/

class IVW_MODULE_LABSTREAMLINES_API StreamlineIntegrator : public Processor {
// Friends
// Types
public:

// Construction / Deconstruction
public:
    StreamlineIntegrator();
    virtual ~StreamlineIntegrator() = default;

// Methods
public:
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    /// Our main computation function
    virtual void process() override;
    void eventMoveStart(Event* event);

    void doIntegration(vec2 startPoint, size3_t dims, const VolumeRAM* vr, IndexBufferRAM* indexBufferLines,
                                         IndexBufferRAM* indexBufferPoints,
                                      std::vector<BasicMesh::Vertex>& vertices, const vec4& color);

    vec2 rk4(const VolumeRAM* vr, size3_t dims, const vec2& position, float stepSize);
    // (TODO: You could define some helper functions here,
    // e.g. a function creating a single streamline from one seed point)

// Ports
public:
    // Input Vector Field
    VolumeInport inData;
    // Output mesh
    MeshOutport outMesh;

// Properties
public:
    EventProperty mouseMoveStart;
    FloatVec2Property propStartPoint;
    TemplateOptionProperty<int> propSeedMode;

    TemplateOptionProperty<int> propDirection;
    FloatProperty propStepSize;
    BoolProperty propNormalized;
    IntProperty propNumberOfSteps;
    FloatProperty propArcLength;
    BoolProperty propStopBoundary;
    BoolProperty propStopAtZero;
    FloatProperty propMinVelocity;
	IntProperty propLineCount;
    IntProperty propXSeedCount;
    IntProperty propYSeedCount;
    BoolProperty propGridSeed;

    // TODO: Declare additional properties
    // Some types that you might need are given below
    // IntProperty properyName;
    // FloatProperty propertyName2;
    // IntVec2Property propertyName3;
    // TemplateOptionProperty<int> propertyName4;
    // BoolProperty propertyName4;

// Attributes
private:
    // Dimensions of the current vector field
    size3_t dims;

    
};

}  // namespace inviwo
