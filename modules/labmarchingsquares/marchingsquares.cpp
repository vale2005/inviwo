/*********************************************************************
 *  Author  : Himangshu Saikia, Wiebke Koepp, ...
 *  Init    : Monday, September 11, 2017 - 12:58:42
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labmarchingsquares/marchingsquares.h>
#include <inviwo/core/util/utilities.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo MarchingSquares::processorInfo_
{
    "org.inviwo.MarchingSquares",      // Class identifier
    "Marching Squares",                // Display name
    "KTH Lab",                          // Category
    CodeState::Experimental,           // Code state
    Tags::None,                        // Tags
};

const ProcessorInfo MarchingSquares::getProcessorInfo() const
{
    return processorInfo_;
}


MarchingSquares::MarchingSquares()
    :Processor()
    , inData("volumeIn")
    , meshOut("meshOut")
    , propShowGrid("showGrid", "Show Grid")
    , propDeciderType("deciderType", "Decider Type")
    , propMultiple("multiple", "Iso Levels")
    , propIsoValue("isovalue", "Iso Value")
    , propGridColor("gridColor", "Grid Lines Color", vec4(0.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f), vec4(1.0f), vec4(0.1f),
        InvalidationLevel::InvalidOutput, PropertySemantics::Color)
    , propIsoColor("isoColor", "Color", vec4(0.0f, 0.0f, 1.0f, 1.0f),
        vec4(0.0f), vec4(1.0f), vec4(0.1f),
        InvalidationLevel::InvalidOutput, PropertySemantics::Color)
    , propNumContours("numContours", "Number of Contours", 1, 1, 50, 1)
    , propIsoTransferFunc("isoTransferFunc", "Colors", &inData)
{
    // Register ports
    addPort(inData);
    addPort(meshOut);
	
    // Register properties
    addProperty(propShowGrid);
    addProperty(propGridColor);
	
    addProperty(propDeciderType);
    propDeciderType.addOption("midpoint", "Mid Point", 0);
    propDeciderType.addOption("asymptotic", "Asymptotic", 1);

    addProperty(propMultiple);
    
    propMultiple.addOption("single", "Single", 0);
    addProperty(propIsoValue);
    addProperty(propIsoColor);

    propMultiple.addOption("multiple", "Multiple", 1);
    addProperty(propNumContours);
    addProperty(propIsoTransferFunc);

    // The default transfer function has just two blue points
    propIsoTransferFunc.get().clearPoints();
    propIsoTransferFunc.get().addPoint(vec2(0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f));
    propIsoTransferFunc.get().addPoint(vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f));
    propIsoTransferFunc.setCurrentStateAsDefault();

    util::hide(propGridColor, propNumContours, propIsoTransferFunc);

    // Show the grid color property only if grid is actually displayed
    propShowGrid.onChange([this]()
    {
        if (propShowGrid.get())
        {
            util::show(propGridColor);
        }
        else
        {
            util::hide(propGridColor);
        }
    });

    // Show options based on display of one or multiple iso contours
    propMultiple.onChange([this]()
    {
        if (propMultiple.get() == 0)
        {
            util::show(propIsoValue, propIsoColor);
            util::hide(propNumContours, propIsoTransferFunc);
        }
        else
        {
            util::hide(propIsoValue);
            util::show(propIsoColor, propNumContours);
            
            // TODO (Bonus): Comment out above if you are using the transfer function
            // and comment in below instead
            // util::hide(propIsoValue, propIsoColor);
            // util::show(propNumContours, propIsoTransferFunc);
        }
    });

}

void MarchingSquares::process()
{
    if (!inData.hasData()) {
	    return;
    }

    // This results in a shared pointer to a volume
    auto vol = inData.getData();

    // Extract the minimum and maximum value from the input data
    const double minValue = vol->dataMap_.valueRange[0];
    const double maxValue = vol->dataMap_.valueRange[1];

    // Set the range for the isovalue to that minimum and maximum
    propIsoValue.setMinValue(minValue);
    propIsoValue.setMaxValue(maxValue);

    // You can print to the Inviwo console with Log-commands:
    LogProcessorInfo("This scalar field contains values between " << minValue << " and " << maxValue << ".");
    // You can also inform about errors and warnings:
    // LogProcessorWarn("I am warning about something"); // Will print warning message in yellow
    // LogProcessorError("I am letting you know about an error"); // Will print error message in red
    // (There is also LogNetwork...() and just Log...(), these display a different source,
    // LogProcessor...() for example displays the name of the processor in the workspace while
    // Log...() displays the identifier of the processor (thus with multiple processors of the
    // same kind you would not know which one the information is coming from
    
    // Retreive data in a form that we can access it
    const VolumeRAM* vr = vol->getRepresentation< VolumeRAM >();
    const size3_t dims = vol->getDimensions();

    // Initialize mesh and vertices
    auto mesh = std::make_shared<BasicMesh>();
    std::vector<BasicMesh::Vertex> vertices;

    // Values within the input data are accessed by the function below
    // It's input is the VolumeRAM from above, the dimensions of the volume
    // and the indeces i and j of the position to be accessed where
    // i is in [0, dims.x-1] and j is in [0, dims.y-1]
    float valueat00 = getInputValue(vr, dims, 0, 0);
    LogProcessorInfo("Value at (0,0) is: " << valueat00);
    LogProcessorInfo("The number of dimensions are : " << dims);
    // You can assume that dims.z = 1 and do not need to consider others cases

    // TODO (Bonus) Gaussian filter
    // Our input is const, but you need to compute smoothed data and write it somewhere
    // Create an editable volume like this:
    // Volume volSmoothed(vol->getDimensions(), vol->getDataFormat());
    // auto vrSmoothed = volSmoothed.getEditableRepresentation<VolumeRAM>();
    // Values can be set with
    // vrSmoothed->setFromDouble(vec3(i,j,0), value);
    // getting values works with an editable volume as well
    // getInputValue(vrSmoothed, dims, 0, 0);

    // Grid
    int xMaxInd = dims.x-1;
    int yMaxInd = dims.y-1;
    auto indexBufferGrid = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    // Properties are accessed with propertyName.get() 
    if (propShowGrid.get())
    {
        // TODO: Add grid lines of the given color 
      
        // The function drawLineSegments creates two vertices at the specified positions, 
        // that are placed into the Vertex vector defining our mesh. 
        // An index buffer specifies which of those vertices should be grouped into to make up lines/trianges/quads.
        // Here two vertices make up a line segment.

        
        for(int i=0; i<=xMaxInd; i++){
            float currPoint = (float)i/xMaxInd;
            drawLineSegment(vec2(0, currPoint), vec2(1, currPoint), propGridColor.get(), indexBufferGrid, vertices);
        }

        for(int i=0; i<=yMaxInd; i++){
            float currPoint = (float)i/yMaxInd;
            drawLineSegment(vec2(currPoint, 0), vec2(currPoint, 1), propGridColor.get(), indexBufferGrid, vertices);
        }

        // Draw a line segment from v1 to v2 with a color, the coordinates in the final 
        // image range from 0 to 1 for both x and y
     
        // vec2 v1 = vec2(0.5, 0.5);
        // vec2 v2 = vec2(0.7, 0.7);
        // drawLineSegment(v1, v2, propGridColor.get(), indexBufferGrid, vertices);
    }

    // Iso contours

    float xScale = 1.0 / xMaxInd;
    float yScale = 1.0 / yMaxInd;

    int xCellCount = xMaxInd - 1;
    int yCellCount = yMaxInd - 1; 

    if (propMultiple.get() == 0)
    {
        for(int x=0; x<=xCellCount; ++x)
            for(int y=0; y<=yCellCount; ++y){
                float currX = x*xScale;
                float currY = y*yScale;

                float value00 = getInputValue(vr, dims, x, y);
                float value01 = getInputValue(vr, dims, x, y+1);
                float value10 = getInputValue(vr, dims, x+1, y);
                float value11 = getInputValue(vr, dims, x+1, y+1);

                std::vector<float> isoXs;
                std::vector<float> isoYs;

                if(isoLineCross(value00, value01, propIsoValue)){
                    float distance = value01 - value00;

                    float yCross = currY + ((propIsoValue-value00)/distance)*yScale;
                    isoXs.push_back(currX);
                    isoYs.push_back(yCross); 
                }
                if(isoLineCross(value00, value10, propIsoValue)){
                    float distance = value10 - value00;

                    float xCross = currX + ((propIsoValue-value00)/distance)*xScale; 
                    isoXs.push_back(xCross);
                    isoYs.push_back(currY); 
                }
                if(isoLineCross(value01, value11, propIsoValue)){
                    float distance = value11 - value01;

                    float xCross = currX + ((propIsoValue-value01)/distance)*xScale; 
                    isoXs.push_back(xCross); 
                    isoYs.push_back(currY + yScale); 
                }
                if(isoLineCross(value10, value11, propIsoValue)){
                    float distance = value11 - value10;

                    float yCross = currY + ((propIsoValue-value10)/distance)*yScale; 
                    isoXs.push_back(currX + xScale); 
                    isoYs.push_back(yCross); 
                }

                if(isoXs.size() == 2){
                    vec2 v1 = vec2(isoXs[0], isoYs[0]);
                    vec2 v2 = vec2(isoXs[1], isoYs[1]);
                    drawLineSegment(v1, v2, propIsoColor.get(), indexBufferGrid, vertices);
                }
                else if(isoXs.size() == 4){
                    float avg = (value00 + value01 + value10 + value11)/4;
                    if((avg >= propIsoValue) == (value00 >= propIsoValue)){
                        vec2 v1 = vec2(isoXs[0], isoYs[0]);
                        vec2 v2 = vec2(isoXs[2], isoYs[2]);
                        drawLineSegment(v1, v2, propIsoColor.get(), indexBufferGrid, vertices);
                        vec2 v3 = vec2(isoXs[1], isoYs[1]);
                        vec2 v4 = vec2(isoXs[3], isoYs[3]);
                        drawLineSegment(v3, v4, propIsoColor.get(), indexBufferGrid, vertices);
                    }else{
                        vec2 v1 = vec2(isoXs[0], isoYs[0]);
                        vec2 v2 = vec2(isoXs[1], isoYs[1]);
                        drawLineSegment(v1, v2, propIsoColor.get(), indexBufferGrid, vertices);
                        vec2 v3 = vec2(isoXs[2], isoYs[2]);
                        vec2 v4 = vec2(isoXs[3], isoYs[3]);
                        drawLineSegment(v3, v4, propIsoColor.get(), indexBufferGrid, vertices);
                    }
                }   

            }

        // TODO: Draw a single isoline at the specified isovalue (propIsoValue) 
        // and color it with the specified color (propIsoColor)
    }
    else
    {
        // TODO: Draw the given number (propNumContours) of isolines between 
        // the minimum and maximum value
        
        // TODO (Bonus): Use the transfer function property to assign a color
        // The transfer function normalizes the input data and sampling colors
        // from the transfer function assumes normalized input, that means
        // vec4 color = propIsoTransferFunc.get().sample(0.0f);
        // is the color for the minimum value in the data
        // vec4 color = propIsoTransferFunc.get().sample(1.0f);
        // is the color for the maximum value in the data

    }

    // Note: It is possible to add multiple index buffers to the same mesh,
    // thus you could for example add one for the grid lines and one for
    // each isoline
    // Also, consider to write helper functions to avoid code duplication
    // e.g. for the computation of a single iso contour

    mesh->addVertices(vertices);
    meshOut.setData(mesh);
}

double MarchingSquares::getInputValue(const VolumeRAM* data, const size3_t dims, 
    const size_t i, const size_t j) {
    // Check if the indices are withing the dimensions of the volume
    if (i < dims.x && j < dims.y) {
        return data->getAsDouble(size3_t(i, j, 0));
    } else {
        LogProcessorError(
            "Attempting to access data outside the boundaries of the volume, value is set to 0");
        return 0;
    }
}

void MarchingSquares::drawLineSegment(const vec2& v1, const vec2& v2, const vec4& color,
                                      IndexBufferRAM* indexBuffer,
                                      std::vector<BasicMesh::Vertex>& vertices) {
    // Add first vertex
    indexBuffer->add(static_cast<std::uint32_t>(vertices.size()));
    // A vertex has a position, a normal, a texture coordinate and a color
    // we do not use normal or texture coordinate, but still have to specify them
    vertices.push_back({vec3(v1[0], v1[1], 0), vec3(0, 0, 1), vec3(v1[0], v1[1], 0), color});
    // Add second vertex
    indexBuffer->add(static_cast<std::uint32_t>(vertices.size()));
    vertices.push_back({vec3(v2[0], v2[1], 0), vec3(0, 0, 1), vec3(v2[0], v2[1], 0), color});
}

bool MarchingSquares::isoLineCross(float value1, float value2, float isoValue){
    return (value1 > isoValue) != (value2 > isoValue);
}

} // namespace
