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
    , propKernelSize("kernelSize", "Kernel Size", 25, 1, 100, 1)

// TODO: Register additional properties

{
    // Register ports
    addPort(volumeIn_);
    addPort(noiseTexIn_);
    addPort(licOut_);

    // Register properties

    // TODO: Register additional properties
    addProperty(propKernelSize);

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
    int kernelSize = propKernelSize.get()/2;
    
    
    // TODO: Implement LIC and FastLIC
    // This code instead sets all pixels to the same gray value
    std::vector<std::vector<double>> licTexture(texDims_.x, std::vector<double>(texDims_.y, 127.0));
    float stepSize = std::min(1.0/(float)texDims_.x, 1.0/(float)texDims_.y);
    //float stepSize = 1;

    for (auto j = 0; j < texDims_.y; j++) {
        for (auto i = 0; i < texDims_.x; i++) {
            
            //display randomly generated image
            //double truncatedSum = Interpolator::sampleFromGrayscaleImage(tr, vec2(i, j));
            
            vec2 currPoint = vec2((float)i / texDims_.x, (float)j/texDims_.y);
            //vec2 currPoint = vec2(i,j);
            std::vector<vec2> currKernelPoints = Integrator::getStreamlinePoints(vol.get(), texDims_, kernelSize, currPoint, stepSize);
            
            float kernelSum = 0.0;
            for(int ind=0; ind<currKernelPoints.size(); ind++){
                int x = round(currKernelPoints[ind].x * texDims_.x);
                int y = round(currKernelPoints[ind].y * texDims_.y);
                kernelSum += Interpolator::sampleFromGrayscaleImage(tr, vec2(x,y));
                //kernelSum += Interpolator::sampleFromGrayscaleImage(tr, currKernelPoints[ind]);

            }
            int truncatedSum = std::round(kernelSum/(float)currKernelPoints.size());
            //LogProcessorInfo("current sum: " << truncatedSum);
            
            
            lr->setFromDVec4(size2_t(i, j), dvec4(truncatedSum, truncatedSum, truncatedSum, 255));

        }
    }

    licOut_.setData(outImage);
}

}  // namespace inviwo
