/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:13
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <dh2320lab3/dh2320lab3moduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <memory>
#include <dh2320lab3/ray.h>
#include <dh2320lab3/rayintersection.h>
#include <dh2320lab3/material.h>

namespace inviwo {
class RayIntersection;
class Material;

/** \class Renderable
    \brief Abstract Class for Visible Geometry

    @author Himangshu Saikia
*/
class IVW_MODULE_DH2320LAB3_API Renderable : public std::enable_shared_from_this<Renderable> {
    //Friends
    //Types
public:

    //Construction / Deconstruction
public:
    Renderable();
    virtual ~Renderable() = default;

    //Methods
public:
    // Computes the point of intersection between ray and object.
    virtual bool closestIntersection(const Ray& ray, double maxLambda,
                                     RayIntersection& intersection) const = 0;
    virtual bool anyIntersection(const Ray& ray, double maxLambda) const = 0;
    virtual void drawGeometry(std::shared_ptr<BasicMesh> mesh,
                              std::vector<BasicMesh::Vertex>& vertices) const = 0;
    void setMaterial(std::shared_ptr<Material> material) { mMaterial = material; }
    std::shared_ptr<const Material> getMaterial() const { return mMaterial; }

    //Attributes
public:
    std::shared_ptr<Material> mMaterial;
};

} // namespace
