/*********************************************************************
*  Author  : Anke Friederici
*
*  Project : KTH Inviwo Modules
*
*  License : Follows the Inviwo BSD license model
**********************************************************************/

#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <labtopo/integrator.h>
#include <labtopo/interpolator.h>
#include <labtopo/topology.h>
#include <labtopo/utils/gradients.h>

namespace inviwo
{

const vec4 Topology::ColorsCP[6] =
    {
        vec4(1, 1, 0, 1),  // Saddle
        vec4(0, 0, 1, 1),  // AttractingNode
        vec4(1, 0, 0, 1),  // RepellingNode
        vec4(0.5, 0, 1, 1),// AttractingFocus
        vec4(1, 0.5, 0, 1),// RepellingFocus
        vec4(0, 1, 0, 1)   // Center
};

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo Topology::processorInfo_{
    "org.inviwo.Topology",  // Class identifier
    "Vector Field Topology",// Display name
    "KTH Lab",              // Category
    CodeState::Experimental,// Code state
    Tags::None,             // Tags
};

const ProcessorInfo Topology::getProcessorInfo() const
{
    return processorInfo_;
}

Topology::Topology()
    : Processor(), outMesh("meshOut"), inData("inData")
// TODO: Initialize additional properties
// propertyName("propertyIdentifier", "Display Name of the Propery",
// default value (optional), minimum value (optional), maximum value (optional), increment (optional));
// propertyIdentifier cannot have spaces
{
    // Register Ports
    addPort(outMesh);
    addPort(inData);

    // TODO: Register additional properties
    // addProperty(propertyName);
}

void Topology::process()
{
    // Get input
    if (!inData.hasData())
    {
        return;
    }
    auto vol = inData.getData();

    // Retreive data in a form that we can access it
    const VolumeRAM* vr = vol->getRepresentation<VolumeRAM>();
    uvec3 dims = vr->getDimensions();

    // Initialize mesh, vertices and index buffers for the two streamlines and the
    auto mesh = std::make_shared<BasicMesh>();
    std::vector<BasicMesh::Vertex> vertices;
    // Either add all line segments to this index buffer (one large buffer),
    // or use several index buffers with connectivity type adjacency.
    auto indexBufferSeparatrices = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);
    auto indexBufferPoints = mesh->addIndexBuffer(DrawType::Points, ConnectivityType::None);

    // TODO: Compute the topological skeleton of the input vector field.
    // Find the critical points and color them according to their type.
    // Integrate all separatrices.
    // You can use your previous integration code (copy it over or call it from <lablic/integrator.h>).

    // Looping through all values in the vector field.
          
    for (int x = 0; x < dims[0]-1; ++x){
        for (int y = 0; y < dims[1]-1; ++y){
            vec2 zeroPosInCurrCell = getCriticalPointInGridCell(vol.get(), vec2(x,y), 1.0f);
            if(zeroPosInCurrCell != vec2(-1, -1)){
                LogProcessorInfo("Zero crossing between point: " << x << " " << y);
                float xCoord = (float)zeroPosInCurrCell.x / (dims.x - 1);
                float yCoord = (float)zeroPosInCurrCell.y / (dims.y - 1);
                indexBufferPoints->add(static_cast<std::uint32_t>(vertices.size()));
                vertices.push_back({vec3(xCoord, yCoord, 0), vec3(0, 0, 1), vec3(xCoord, yCoord, 0), vec4(0,0,0,1)});
            }

        //     currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x, y)));
        //     currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x+1, y)));
        //     currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x, y+1)));
        //     currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x+1, y+1)));
        //     LogProcessorInfo("Curr grid edges: " << currGridEdges[0] << " " << currGridEdges[1] 
        //                      << " " << currGridEdges[2] << " " << currGridEdges[3]);
        //     if(checkZeroCrossing(currGridEdges)){
        //         LogProcessorInfo("Zero crossing between point: " << x << " " << y);
        //         flo xCoord = (float)x / (dims.x - 1);
        //         float yCoord = (float)y / (dims.y - 1);
        //         vertices.push_back({vec3(xCoord, yCoord, 0), vec3(0, 0, 1), vec3(xCoord, yCoord, 0), vec4(0,0,0,1)});
        //         indexBufferPoints->add(static_cast<atstd::uint32_t>(vertices.size()));
        //     }
        }
    }
        
    mesh->addVertices(vertices);
    outMesh.setData(mesh);
}

vec2 Topology::getCriticalPointInGridCell(const Volume* vol, vec2 position, float distance = 1.0f){
    if(distance < 0.0005){
        return position;
    }

    float x = position.x;
    float y = position.y;

    std::vector<vec2> currGridEdges;
    currGridEdges.push_back(Interpolator::sampleFromField(vol, vec2(x, y)));
    currGridEdges.push_back(Interpolator::sampleFromField(vol, vec2(x+distance, y)));
    currGridEdges.push_back(Interpolator::sampleFromField(vol, vec2(x, y+distance)));
    currGridEdges.push_back(Interpolator::sampleFromField(vol, vec2(x+distance, y+distance)));
    if(checkZeroCrossing(currGridEdges)){
        distance = distance / 2.0;
        vec2 p1 = getCriticalPointInGridCell(vol, vec2(x, y), distance);
        if (p1 != vec2(-1, -1)) return p1;
        vec2 p2 = getCriticalPointInGridCell(vol, vec2(x+distance, y), distance);
        if (p2 != vec2(-1, -1)) return p2;
        vec2 p3 = getCriticalPointInGridCell(vol, vec2(x, y+distance), distance);
        if (p3 != vec2(-1, -1)) return p3;
        vec2 p4 = getCriticalPointInGridCell(vol, vec2(x+distance, y+distance), distance);
        if (p4 != vec2(-1, -1)) return p4;
        return vec2(-1, -1);
    }else{
        return vec2(-1, -1);
    }
}

// getCellPoints(vec2 startPosition, float distance, int quadrant){
//     float x = startPosition.x;
//     float y = startPosition.y;
//     if(quadrant == 0){
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x, y)));           
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x+distance, y)));
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x, y+distance)));
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x+distance, y+distance)));
//     }
//     else if(quadrant == 1){
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x-distance, y)));           
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x+distance, y)));
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x-distance, y+distance)));
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x, y+distance)));
//     }
//     else if(quadrant == 2){
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x, y-distance)));           
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x+distance, y-distance)));
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x, y)));
//         currGridEdges.push_back(Interpolator::sampleFromField(vol.get(), vec2(x+distance, y)));
//     }
// }


bool Topology::checkZeroCrossing(std::vector<vec2> gridEdges){
    for(int i=0; i < gridEdges.size(); ++i){
        for(int j=i+1; j < gridEdges.size(); ++j){
            if((gridEdges[i].x) <= 0 != (gridEdges[j].x <= 0) &&
               (gridEdges[i].y) <= 0 != (gridEdges[j].y <= 0)){
                   return true;
               }
            if((gridEdges[i].x) >= 0 != (gridEdges[j].x >= 0) &&
               (gridEdges[i].y) >= 0 != (gridEdges[j].y >= 0)){
                   return true;
               }
        }
    }
    return false;
}

}// namespace
