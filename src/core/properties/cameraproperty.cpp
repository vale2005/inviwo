#include "inviwo/core/properties/cameraproperty.h"

namespace inviwo {

CameraProperty::CameraProperty(std::string identifier, std::string displayName,
                               vec3 lookFrom, vec3 lookTo, vec3 lookUp)
    : CompositeProperty(identifier, displayName),
    lookFrom_("lookFrom", "Look from", lookFrom, vec3(std::numeric_limits<float>::min()), vec3(std::numeric_limits<float>::max()), vec3(0.0f, 0.0f, -1.0f)),
    lookTo_("lookTo", "Look to", lookTo, vec3(std::numeric_limits<float>::min()), vec3(std::numeric_limits<float>::max()), vec3(0.0f, 0.0f, 0.0f)),
    lookUp_("lookUp", "Look up", lookUp, vec3(std::numeric_limits<float>::min()), vec3(std::numeric_limits<float>::max()), vec3(0.0f, 1.0f, 0.0f))
{
    addProperty(lookFrom_);
    addProperty(lookTo_);
    addProperty(lookUp_);
    updateViewMatrix();
}

CameraProperty::~CameraProperty() {}

void CameraProperty::setLookFrom(vec3 lookFrom) {
    lookFrom_.set(lookFrom);
    updateViewMatrix();
}

void CameraProperty::setLookTo(vec3 lookTo) {
    lookTo_.set(lookTo);
    updateViewMatrix();
}

void CameraProperty::setLookUp(vec3 lookUp) {
    lookUp_.set(lookUp);
    updateViewMatrix();
}

void CameraProperty::updateViewMatrix() {
    vec3 lookVector = normalize(lookTo_.get() - lookFrom_.get());
    vec3 upVector = normalize(lookUp_.get());
    vec3 sideVector = cross(lookVector, upVector);
    upVector = cross(sideVector, lookVector);
    mat4 m = mat4(vec4(sideVector,       0.0f),
                  vec4(upVector,         0.0f),
                  vec4(-lookVector,      0.0f),
                  vec4(0.0f, 0.0f, 0.0f, 1.0f));
    viewMatrix_ = m * mat4::createTranslation(-lookFrom_.get());
}



} // namespace
