/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, September 19, 2017 - 15:08:33
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labstreamlines/streamlineintegrator.h>
#include <labstreamlines/integrator.h>
#include <inviwo/core/util/utilities.h>
#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo StreamlineIntegrator::processorInfo_{
    "org.inviwo.StreamlineIntegrator",  // Class identifier
    "Streamline Integrator",            // Display name
    "KTH Lab",                          // Category
    CodeState::Experimental,            // Code state
    Tags::None,                         // Tags
};

const ProcessorInfo StreamlineIntegrator::getProcessorInfo() const { return processorInfo_; }

StreamlineIntegrator::StreamlineIntegrator()
    : Processor()
    , inData("volIn")
    , outMesh("meshOut")
    , propStartPoint("startPoint", "Start Point", vec2(0.5, 0.5), vec2(0), vec2(1024), vec2(0.5))
    , propSeedMode("seedMode", "Seeds")
    , propDirection("direction", "Direction of the integration")
    , propStepSize("stepSize", "Integration step size", 0.2f, 0.001f, 2.0f, 0.001f)
    , propNormalized("normalized", "Use normalized vector field")
    , propNumberOfSteps("numberOfSteps", "Number of integration steps", 100, 1, 1000)
    , propArcLength("arcLength", "Stop after arc length", 100, 0, 100, 0.1)
    , propStopBoundary("stopBoudary", "Stop at boundary", true)
    , propStopAtZero("stopAtZero", "Stop at zeros", true)
    , propMinVelocity("minVelocity", "Velocity min", 0, 0, 5, 0.1)
	, propLineCount("lineCount", "Number of lines", 1, 1, 100, 1)
    , propGridSeed("gridSeed", "Seed on grid cells")
    , propXSeedCount("xSeedCount", "Seeds X", 1, 1, 100, 1)
    , propYSeedCount("ySeedCount", "Seeds Y", 1, 1, 100, 1)
    // TODO: Initialize additional properties
    // propertyName("propertyIdentifier", "Display Name of the Propery",
    // default value (optional), minimum value (optional), maximum value (optional), increment
    // (optional)); propertyIdentifier cannot have spaces
    , mouseMoveStart("mouseMoveStart", "Move Start", [this](Event* e) { eventMoveStart(e); },
                     MouseButton::Left, MouseState::Press | MouseState::Move) {
    // Register Ports
    addPort(inData);
    addPort(outMesh);

    // Register Properties
    propSeedMode.addOption("one", "Single Start Point", 0);
    propSeedMode.addOption("multiple", "Multiple Seeds", 1);

    propDirection.addOption("forward", "forward direction", 0);
    propDirection.addOption("backwards", "backward direction", 1);


    addProperty(propSeedMode);
    addProperty(propStartPoint);
    addProperty(mouseMoveStart);
    addProperty(propDirection);
    addProperty(propStepSize);
    addProperty(propNormalized);
    addProperty(propNumberOfSteps);
    addProperty(propArcLength);
    addProperty(propStopBoundary);
    addProperty(propStopAtZero);
    addProperty(propMinVelocity);
	addProperty(propLineCount);
    addProperty(propGridSeed);
    addProperty(propXSeedCount);
    addProperty(propYSeedCount);

    // TODO: Register additional properties
    // addProperty(propertyName);

    // You can hide and show properties for a single seed and hide properties for multiple seeds (TODO)
    propSeedMode.onChange([this]() {
        if (propSeedMode.get() == 0) {
            util::show(propStartPoint, mouseMoveStart);
            // util::hide(...)
        } else {
            util::hide(propStartPoint, mouseMoveStart);
            // util::show(...)
        }
    });

}

void StreamlineIntegrator::eventMoveStart(Event* event) {
    // Handle mouse interaction only if we
    // are in the mode with a single point
    if (propSeedMode.get() == 1) return;
    auto mouseEvent = static_cast<MouseEvent*>(event);
    vec2 mousePos = mouseEvent->posNormalized();
    // Denormalize to volume dimensions
    mousePos.x *= dims.x - 1;
    mousePos.y *= dims.y - 1;
    // Update starting point
    propStartPoint.set(mousePos);
    event->markAsUsed();
}

void StreamlineIntegrator::process() {
    // Get input
    if (!inData.hasData()) {
        return;
    }
    auto vol = inData.getData();

    // Retreive data in a form that we can access it
    auto vr = vol->getRepresentation<VolumeRAM>();
    dims = vol->getDimensions();
    // The start point should be inside the volume (set maximum to the upper right corner)
    propStartPoint.setMaxValue(vec2(dims.x - 1, dims.y - 1));

    auto mesh = std::make_shared<BasicMesh>();
    std::vector<BasicMesh::Vertex> vertices;

    if (propSeedMode.get() == 0) {
        auto indexBufferPoints = mesh->addIndexBuffer(DrawType::Points, ConnectivityType::None);
        auto indexBufferLines = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::Strip);
        // Draw start point
        vec2 startPoint = propStartPoint.get();
        vertices.push_back({vec3(startPoint.x / (dims.x - 1), startPoint.y / (dims.y - 1), 0),
                            vec3(0), vec3(0), vec4(0, 0, 0, 1)});
        indexBufferPoints->add(static_cast<std::uint32_t>(0));
        indexBufferLines->add(static_cast<std::uint32_t>(0));
        doIntegration(startPoint, dims,vr,  indexBufferLines, indexBufferPoints, vertices, vec4(0,0,1,1));
        // TODO: Create one stream line from the given start point
    } else if(propSeedMode.get() == 1) {
        // TODO: Seed multiple stream lines either randomly or using a uniform grid
        // (TODO: Bonus, sample randomly according to magnitude of the vector field)

        if(!propGridSeed.get()){
            for (int i = 0; i < propLineCount.get(); i++) {
                auto indexBufferPoints = mesh->addIndexBuffer(DrawType::Points, ConnectivityType::None);
                auto indexBufferLines = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::Strip);
                // Draw start point
                float x1 = (rand() % ((dims.x-1) * 1000)) / 1000.0;
                float y1 = (rand() % ((dims.y-1) * 1000)) / 1000.0;
                LogProcessorInfo("Points: " << x1 <<  " " << y1); 
                vec2 startPoint = vec2(x1, y1);
                vertices.push_back({ vec3(startPoint.x / (dims.x - 1), startPoint.y / (dims.y - 1), 0),
                    vec3(0), vec3(0), vec4(0, 0, 1, 1) });
                indexBufferPoints->add(static_cast<std::uint32_t>(vertices.size()));
                indexBufferLines->add(static_cast<std::uint32_t>(vertices.size()));


                doIntegration(startPoint, dims, vr, indexBufferLines, indexBufferPoints, vertices, vec4(0, 0, 1, 1));
                
            }
        } else {
            for (int i = 0; i < propXSeedCount.get(); i++) {
                float x1 = (float)i / propXSeedCount.get() * (dims.x);
                for (int j = 0; j < propYSeedCount.get(); j++) {
                    auto indexBufferPoints = mesh->addIndexBuffer(DrawType::Points, ConnectivityType::None);
                    auto indexBufferLines = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::Strip);
                    float y1 = (float)j / propYSeedCount.get() * (dims.y);
                    LogProcessorInfo("Points: " << x1 <<  " " << y1); 
                    vec2 startPoint = vec2(x1, y1);
                    vertices.push_back({ vec3(startPoint.x / (dims.x - 1), startPoint.y / (dims.y - 1), 0),
                        vec3(0), vec3(0), vec4(0, 0, 1, 1) });
                    indexBufferPoints->add(static_cast<std::uint32_t>(vertices.size()));
                    indexBufferLines->add(static_cast<std::uint32_t>(vertices.size()));


                    doIntegration(startPoint, dims, vr, indexBufferLines, indexBufferPoints, vertices, vec4(0, 0, 1, 1));   
                }
            }
        }
    }

    mesh->addVertices(vertices);
    outMesh.setData(mesh);

}

void StreamlineIntegrator::doIntegration(vec2 startPoint, size3_t dims, const VolumeRAM* vr,  IndexBufferRAM* indexBufferLines,
                                         IndexBufferRAM* indexBufferPoints,
                                      std::vector<BasicMesh::Vertex>& vertices, const vec4& color)
{
    vec2 prevPoint = startPoint;
    float stepSize = (propDirection.get() == 0) ? propStepSize.get() : - propStepSize.get();
    float arcLength = 0.0;
    for(int i=0; i<propNumberOfSteps.get(); i++){ 
        vec2 nextPoint = rk4(vr, dims, prevPoint, stepSize);
        if (propStopAtZero.get() &&
            (nextPoint[0] < 0 || nextPoint[0] > dims[0] - 1 || 
            nextPoint[1] < 0 || nextPoint[1] > dims[1] - 1)) break;
        Integrator::drawLineSegmentAndPoints(prevPoint, nextPoint, dims, indexBufferLines, 
                                            indexBufferPoints, vertices, color);
        
        float velocity = sqrt(pow(nextPoint.x-prevPoint.x, 2) + pow(nextPoint.y-prevPoint.y, 2));
        arcLength += velocity;

        if(velocity <= propMinVelocity.get()) break;
        if(arcLength >= propArcLength.get()) break;
        if(propStopAtZero.get() && prevPoint.x == nextPoint.x && prevPoint.y == nextPoint.y) break;
       
        prevPoint = vec3(nextPoint.x, nextPoint.y, 0);
    }


}

vec2 StreamlineIntegrator::rk4(const VolumeRAM* vr, size3_t dims, const vec2& position, float stepSize)
{
    bool isNormalized = propNormalized.get();
    vec2 v1 = Integrator::sampleFromField(vr, dims, position, isNormalized);
    vec2 v1step = vec2(position.x+(stepSize/2.0)*v1.x, position.y+(stepSize/2.0)*v1.y);
    vec2 v2 = Integrator::sampleFromField(vr, dims, v1step, isNormalized);
    vec2 v2step = vec2(position.x+(stepSize/2.0)*v2.x, position.y+(stepSize/2.0)*v2.y);
    vec2 v3 = Integrator::sampleFromField(vr, dims, v2step, isNormalized);
    vec2 v3step = vec2(position.x+stepSize*v3.x, position.y+stepSize*v3.y);
    vec2 v4 = Integrator::sampleFromField(vr, dims, v3step, isNormalized);
    float xCoord = position.x + stepSize * (v1.x/6.0 + v2.x/3.0 + v3.x/3.0 + v4.x/6.0);
    float yCoord = position.y + stepSize * (v1.y/6.0 + v2.y/3.0 + v3.y/3.0 + v4.y/6.0);
    return vec2(xCoord, yCoord);
}


}  // namespace inviwo
