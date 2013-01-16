#ifndef IVW_BOOLPROPERTY_H
#define IVW_BOOLPROPERTY_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/properties/templateproperty.h"

namespace inviwo {

class IVW_CORE_API BoolProperty : public TemplateProperty<bool> {

public:
    BoolProperty(std::string identifier, std::string displayName, bool value);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_BOOLPROPERTY_H