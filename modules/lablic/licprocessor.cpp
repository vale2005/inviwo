/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Monday, October 02, 2017 - 13:31:17
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <lablic/licprocessor.h>
#include <lablic/integrator.h>
#include <lablic/interpolator.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo LICProcessor::processorInfo_{
    "org.inviwo.LICProcessor",  // Class identifier
    "LICProcessor",             // Display name
    "KTH Labs",                 // Category
    CodeState::Experimental,    // Code state
    Tags::None,                 // Tags
};

const ProcessorInfo LICProcessor::getProcessorInfo() const { return processorInfo_; }

LICProcessor::LICProcessor()
    : Processor()
    , volumeIn_("volIn")
    , noiseTexIn_("noiseTexIn")
    , licOut_("licOut")
    , propKernelSize("kernelSize", "Kernel Size", 7, 1, 100, 1)
	, propMean("mean", "The mean color value of the pixels", 0.5, 0, 1, 0.1)
	, propStandardDev("standardDev", "Standard Deviation", 0, 0, 1, 0.1)
    , propLicType("licType", "Lic Type")

// TODO: Register additional properties

{
    // Register ports
    addPort(volumeIn_);
    addPort(noiseTexIn_);
    addPort(licOut_);

    // Register properties
    propLicType.addOption("normal", "Normal", 0);
    propLicType.addOption("fast", "Fast", 1);

    // TODO: Register additional properties
    addProperty(propKernelSize);
	addProperty(propMean);
	addProperty(propStandardDev);
    addProperty(propLicType);

}

void LICProcessor::process() {
    // Get input
    if (!volumeIn_.hasData()) {
        return;
    }

    if (!noiseTexIn_.hasData()) {
        return;
    }

    auto vol = volumeIn_.getData();
    vectorFieldDims_ = vol->getDimensions();
    auto vr = vol->getRepresentation<VolumeRAM>();
    auto dims = vr->getDimensions();



    // An accessible form of on image is retrieved analogous to a volume
    auto tex = noiseTexIn_.getData();
    texDims_ = tex->getDimensions();
    auto tr = tex->getRepresentation<ImageRAM>();

    // Prepare the output, it has the same dimensions and datatype as the output
    // and an editable version is retrieved analogous to a volume
    auto outImage = tex->clone();
    auto outLayer = outImage->getColorLayer();
    auto lr = outLayer->getEditableRepresentation<LayerRAM>();

    // To access the image at a floating point position, you can call
    //Interpolator::sampleFromGrayscaleImage(tr, somePos)
	int kernelSize = propKernelSize.get() / 2;
    
    auto base = vol->getBasis();
    
    // TODO: Implement LIC and FastLIC
    // This code instead sets all pixels to the same gray value
	float stepSize = std::min((float)(dims.x - 1.0) / texDims_.x, (float)(dims.y - 1.0) / texDims_.y);

    if(propLicType.get() == 0){
        for (auto i = 0; i < texDims_.x; i++) {
            for (auto j = 0; j < texDims_.y; j++) {
                
                //display randomly generated image
                //double truncatedSum = Interpolator::sampleFromGrayscaleImage(tr, vec2(i, j));
                

                //o to 1
                // vec2 currPoint = vec2((float)i / texDims_.x, (float)j/texDims_.y);
                vec2 currPoint = vec2((float)i * (float)(dims.x - 1.0)/texDims_.x, (float)j * (float)(dims.y - 1.0) / texDims_.y );
                std::vector<vec2> currKernelPoints = Integrator::getStreamlinePoints(vol.get(), kernelSize, currPoint, stepSize);
                
                float kernelSum = 0.0;
                for(int ind=0; ind<currKernelPoints.size(); ind++){
                    // int x = round(currKernelPoints[ind].x * texDims_.x);
                    // int y = round(currKernelPoints[ind].y * texDims_.y);
                    float x = currKernelPoints[ind].x * (float)texDims_.x / (dims.x - 1.0);
                    float y = currKernelPoints[ind].y * (float)texDims_.y / (dims.y - 1.0);
                    kernelSum += Interpolator::sampleFromGrayscaleImage(tr, vec2(x,y));
                    // kernelSum += Interpolator::sampleFromGrayscaleImage(tr, currKernelPoints[ind]);
                }
                int truncatedSum = std::round(kernelSum/(float)currKernelPoints.size());
                // LogProcessorInfo("\n\n")
                
                // Color enhancer with user specific input
                float meanCol = propMean.get();
                float standardDev = propStandardDev.get();
                int enhancedCol = 0;
                int prettyMean = std::round(meanCol * 255.0);
                if (standardDev > 0.0){

                    // The current color in the texture
                    int currentCol = truncatedSum;
                    // Make bright stuff brighter
                    if (currentCol > prettyMean) {
                        enhancedCol = std::round(currentCol* (1.0 + standardDev));
                        // Out of bounds check
                        if (enhancedCol > 255.0) {
                            enhancedCol = 255.0;
                        }
                    }
                      // Make dark stuff darker
                    if (currentCol <= prettyMean) {
                        enhancedCol = std::round(currentCol* (1.0 - standardDev));
                        // Out of bounds check
                        if (enhancedCol < 0) {
                            enhancedCol = 0;
                        }
                    }
                    lr->setFromDVec4(size2_t(i, j), dvec4(enhancedCol, enhancedCol, enhancedCol, 255)); 
                }else{
                    lr->setFromDVec4(size2_t(i, j), dvec4(truncatedSum, truncatedSum, truncatedSum, 255));
                }
            }
        }
    }else{
        bool visitedPoints[texDims_.x][texDims_.y];
        for (auto i = 0; i < texDims_.x; i++) {
            for (auto j = 0; j < texDims_.y; j++) {
                visitedPoints[i][j] = false;
            }
        }
        int visitedCount = 0;
        int otherVisitedCount = 0;
        for (auto i = 0; i < texDims_.x; i++) {
            for (auto j = 0; j < texDims_.y; j++) {
                if(!visitedPoints[i][j]){
                    vec2 currPoint = vec2((float)i * (float)(dims.x - 1.0)/texDims_.x, (float)j * (float)(dims.y - 1.0) / texDims_.y);
                    std::vector<vec2> currKernelPoints = Integrator::getWholeStreamlinePoints(vol.get(), currPoint, stepSize);
                    for(int ind=0; ind<currKernelPoints.size(); ind++){
                        float kernelSum = 0.0;
                        int pointX = round(currKernelPoints[ind].x * (float)texDims_.x / (dims.x - 1.0));
                        int pointY = round(currKernelPoints[ind].y * (float)texDims_.y / (dims.y - 1.0));
                        
                        if(!visitedPoints[pointX][pointY] && pointX < texDims_.x && pointY < texDims_.y ){
                            int counter = 0;
                            for(int kind=0; kind<kernelSize; kind++){
                                if((ind - kind) >= 0){
                                    float x = currKernelPoints[ind-kind].x * (float)texDims_.x / (dims.x - 1.0);
                                    float y = currKernelPoints[ind-kind].y * (float)texDims_.y / (dims.y - 1.0);
                                    kernelSum += Interpolator::sampleFromGrayscaleImage(tr, vec2(x,y));
                                    counter++;
                                }
                                if((ind+kind) < currKernelPoints.size()){
                                    float x = currKernelPoints[ind+kind].x * (float)texDims_.x / (dims.x - 1.0);
                                    float y = currKernelPoints[ind+kind].y * (float)texDims_.y / (dims.y - 1.0);
                                    kernelSum += Interpolator::sampleFromGrayscaleImage(tr, vec2(x,y));
                                    counter++;
                                }
                                
                            }
                            int truncatedSum = std::round(kernelSum/counter);
                            
                            // LogProcessorInfo("PointX: " << pointX << " " << pointY);
                            // LogProcessorInfo("Truncated sum: " << truncatedSum);
                            // LogProcessorInfo("\n\n");
                            
                            float meanCol = propMean.get();
                            float standardDev = propStandardDev.get();
                            int enhancedCol = 0;
                            int prettyMean = std::round(meanCol * 255.0);
                            if (standardDev > 0.0){

                                // The current color in the texture
                                int currentCol = truncatedSum;
                                // Make bright stuff brighter
                                if (currentCol > prettyMean) {
                                    enhancedCol = std::round(currentCol* (1.0 + standardDev));
                                    // Out of bounds check
                                    if (enhancedCol > 255.0) {
                                        enhancedCol = 255.0;
                                    }
                                }
                                // Make dark stuff darker
                                if (currentCol <= prettyMean) {
                                    enhancedCol = std::round(currentCol* (1.0 - standardDev));
                                    // Out of bounds check
                                    if (enhancedCol < 0) {
                                        enhancedCol = 0;
                                    }
                                }
                                lr->setFromDVec4(size2_t(pointX, pointY), dvec4(enhancedCol, enhancedCol, enhancedCol, 255)); 
                            }else{
                                lr->setFromDVec4(size2_t(pointX, pointY), dvec4(truncatedSum, truncatedSum, truncatedSum, 255));
                            }
                            visitedPoints[pointX][pointY]=true;
                        }
                        else{
                            visitedCount++;
                        }
                    }
                }else{
                    otherVisitedCount++;
                }
            }
        }
        LogProcessorInfo("Revisited stuff: " << visitedCount << " other visited stuff: " << otherVisitedCount);
    }

   


    licOut_.setData(outImage);
}

}  // namespace inviwo
