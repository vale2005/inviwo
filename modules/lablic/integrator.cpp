/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Wednesday, September 20, 2017 - 12:04:15
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <lablic/integrator.h>
#include <lablic/interpolator.h>

namespace inviwo {
    
    Integrator::Integrator() {}
    
    // HINT: There is a change in sampleFromField():
    //      Interpolator::sampleFromField(vol.get(), somePosition);
    
    
    // TODO: Implement a single integration step here
    
    
    vec2 Integrator::rk4(const Volume* vol, const vec2& position, float stepSize)
    {
        vec2 v1 = Interpolator::sampleFromField(vol, position);
        vec2 v1step = vec2(position.x+(stepSize/2.0)*v1.x, position.y+(stepSize/2.0)*v1.y);
        vec2 v2 = Interpolator::sampleFromField(vol, v1step);
        vec2 v2step = vec2(position.x+(stepSize/2.0)*v2.x, position.y+(stepSize/2.0)*v2.y);
        vec2 v3 = Interpolator::sampleFromField(vol, v2step);
        vec2 v3step = vec2(position.x+stepSize*v3.x, position.y+stepSize*v3.y);
        vec2 v4 = Interpolator::sampleFromField(vol, v3step);
        float xCoord = position.x + stepSize * (v1.x/6.0 + v2.x/3.0 + v3.x/3.0 + v4.x/6.0);
        float yCoord = position.y + stepSize * (v1.y/6.0 + v2.y/3.0 + v3.y/3.0 + v4.y/6.0);
        return vec2(xCoord, yCoord);
    }
    
    std::vector<vec2> Integrator::getStreamlinePoints(const Volume* vol, size2_t dims, int kernel, vec2 startPoint, float stepSize){
            
            //initialize vector for points along the stream line
            std::vector<vec2> streamlinePoints;
            
            //initialize startpoints
            vec2 currPointForward = startPoint;
            vec2 currPointBackward = startPoint;
            
            for (int i = 0; i<kernel; i++) {
                currPointForward = rk4(vol, currPointForward, stepSize);
                currPointBackward = rk4(vol, currPointBackward, stepSize*(-1));
                streamlinePoints.push_back(currPointBackward);
                streamlinePoints.push_back(currPointForward);
            }
            
            streamlinePoints.push_back(startPoint);
            
            return streamlinePoints;
            
    }
        
}  // namespace inviwo
