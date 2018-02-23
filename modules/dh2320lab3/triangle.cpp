/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dh2320lab3/triangle.h>
#include <dh2320lab3/util.h>
#include <memory>

namespace inviwo {

Triangle::Triangle() {
}

Triangle::Triangle(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& uvw0,
                   const vec3& uvw1, const vec3& uvw2) {
    mVertices[0] = v0;
    mVertices[1] = v1;
    mVertices[2] = v2;
    mUVW[0] = uvw0;
    mUVW[1] = uvw1;
    mUVW[2] = uvw2;
}

bool SameSide(const vec3& p1, const vec3& p2, const vec3& a, const vec3& b) {
    const vec3 cp1 = cross(b - a, p1 - a);
    const vec3 cp2 = cross(b - a, p2 - a);
    if (dot(cp1, cp2) >= 0)
        return true;
    return false;
}

bool InTriangle(const vec3 p, const vec3 a, const vec3 b, const vec3 c) {
    if (SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b))
        return true;
    return false;
}

bool Triangle::closestIntersection(const Ray& ray, double maxLambda,
                                   RayIntersection& intersection) const {
    //Programming TASK 1: Implement this method
    //Your code should compute the intersection between a ray and a triangle.
    //
    //If you detect an intersection, the return type should look similar to this:
    //if(rayIntersectsTriangle)
    //{
    //  intersection = RayIntersection(ray,shared_from_this(),lambda,ray.normal,uvw);
    //  return true;
    //} 
    //
    // Hints :
    // As explained in Lecture on Raytracing
    // 1. Ray origin p_r : ray.getOrigin()
    //    Ray direction t_r : ray.getDirection()
    // 2. For a triangle we have t1 = v1 - v0, t2 = v2 - v0, n = cross(t1, t2)
    // 3. First you need to check if the ray : p = p_r + lambda * t_r intersects the Plane : (p - v0).n = 0
    //    solve for lambda. Let this value be lambda'
    //    If lambda' is within [0, maxLambda] the ray intersects the plane
    // 4. If the ray does not intersect the plane, return false. Otherwise, you need to check if the 
    //    point p = p_r + lambda' * t_r (Use ray.pointOnRay(lambda') for this)
    //    lies within the triangle (v0, v1, v2). Using barycentric coordinates is one way of solving this.

    return false;
}

bool Triangle::anyIntersection(const Ray& ray, double maxLambda) const {
    RayIntersection temp;
    return closestIntersection(ray, maxLambda, temp);
}

void Triangle::drawGeometry(std::shared_ptr<BasicMesh> mesh,
                            std::vector<BasicMesh::Vertex>& vertices) const {
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    Util::drawLineSegment(mVertices[0], mVertices[1], vec4(0.2, 0.2, 0.2, 1), indexBuffer,
                          vertices);
    Util::drawLineSegment(mVertices[1], mVertices[2], vec4(0.2, 0.2, 0.2, 1), indexBuffer,
                          vertices);
    Util::drawLineSegment(mVertices[2], mVertices[0], vec4(0.2, 0.2, 0.2, 1), indexBuffer,
                          vertices);
}

} // namespace
