#include "inviwo/core/network/portconnection.h"

namespace inviwo {

    PortConnection::PortConnection() {}

    PortConnection::PortConnection(Port* outport, Port* inport)
        : outport_(outport),
          inport_(inport) {}

    PortConnection::~PortConnection() {}

    void PortConnection::serialize(XmlSerializer& /*s*/) const {}
    void PortConnection::deserialize(XmlDeserializer& /*s*/) {}

} // namespace
