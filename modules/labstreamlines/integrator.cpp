/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Wednesday, September 20, 2017 - 12:04:15
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labstreamlines/integrator.h>

namespace inviwo
{

Integrator::Integrator()
{
}

vec2 Integrator::sampleFromField(const VolumeRAM* vr, size3_t dims, const vec2& position)
{
    // Sampled outside the domain!
    if (position[0] < 0 || position[0] > dims[0] - 1 || position[1] < 0 || position[1] > dims[1] - 1)
    {
        return vec2(0, 0);
    }

    int x0 = int(position[0]);
    int y0 = int(position[1]);

    // Leads to accessing only inside the volume
    // Coefficients computation takes care of using the correct values
    if (x0 == dims[0] - 1)
    {
        x0--;
    }
    if (y0 == dims[1] - 1)
    {
        y0--;
    }

    auto f00 = vr->getAsDVec2(size3_t(x0, y0, 0));
    auto f10 = vr->getAsDVec2(size3_t(x0 + 1, y0, 0));
    auto f01 = vr->getAsDVec2(size3_t(x0, y0 + 1, 0));
    auto f11 = vr->getAsDVec2(size3_t(x0 + 1, y0 + 1, 0));

    float x = position[0] - x0;
    float y = position[1] - y0;

    vec2 f;

    for (int i = 0; i < 2; i++)
    {
        f[i] = f00[i] * (1 - x) * (1 - y) + f01[i] * (1 - x) * y + f10[i] * x * (1 - y) + f11[i] * x * y;
    }

    return f;
}


// TODO: Implement a single integration step here

vec2 Integrator::euler(const VolumeRAM* vr, size3_t dims, const vec2& position, float stepSize)
{
    vec2 nextVec =  sampleFromField(vr, dims, position); 
    vec2 nextGridPoint = vec2(position.x + stepSize * nextVec.x, position.y + stepSize * nextVec.y);
    return nextGridPoint; 

}

vec2 Integrator::rk4(const VolumeRAM* vr, size3_t dims, const vec2& position, float stepSize)
{
    return vec2();
}

void Integrator::drawLineSegmentAndPoints(const vec2& v1, const vec2& v2,
                                      size3_t dims,
                                      IndexBufferRAM* indexBufferLines,
                                      IndexBufferRAM* indexBufferPoints,
                                      std::vector<BasicMesh::Vertex>& vertices) {
    // Add first vertex
    indexBufferLines->add(static_cast<std::uint32_t>(vertices.size()));
    indexBufferPoints->add(static_cast<std::uint32_t>(vertices.size()));
    // A vertex has a position, a normal, a texture coordinate and a color
    // we do not use normal or texture coordinate, but still have to specify them
    vertices.push_back({vec3(v1[0]/(dims.x-1), v1[1]/(dims.y-1), 0), vec3(0), vec3(0), vec4(0,1,0,1)});
    // Add second vertex
    indexBufferLines->add(static_cast<std::uint32_t>(vertices.size()));
    indexBufferPoints->add(static_cast<std::uint32_t>(vertices.size()));
    vertices.push_back({vec3(v2[0]/(dims.x-1), v2[1]/(dims.y-1), 0), vec3(0), vec3(0), vec4(0,1,0,1)});
}
} // namespace

