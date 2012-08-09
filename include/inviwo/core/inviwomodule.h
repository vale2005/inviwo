#ifndef IVW_INVIWOMODULE_H
#define IVW_INVIWOMODULE_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/processors/processor.h"
#include "inviwo/core/properties/property.h"
#include "inviwo/core/io/datareader.h"
#include "inviwo/core/io/datawriter.h"
#include "inviwo/core/datastructures/representationconverter.h"

#include "ext/voreen/serialization/serialization.h"

namespace inviwo {

class InviwoModule {

public:
    InviwoModule();
    virtual ~InviwoModule();

    std::string getIdentifier() const;

    bool isInitialized() const;

    const std::vector<Processor*>& getProcessors() const;
    const std::vector<Property*>& getProperties() const;
    const std::vector<DataReader*>& getDataReaders() const;
    const std::vector<DataWriter*>& getDataWriters() const;
    const std::vector<RepresentationConverter*>& getRepresentationConverters() const;
    const std::vector<voreen::SerializableFactory*>& getSerializableFactories() const;

    std::string getDescription() const;
    void setDescription(const std::string& description) const;

    virtual void initialize() throw (Exception);
    virtual void deinitialize() throw (Exception);

protected:

    void setIdentifier(const std::string& identifier);

    void addProcessor(Processor* processor);
    void addProperty(Property* property);
    void addDataReader(DataReader* reader);
    void addDataWriter(DataWriter* writer);
    void addRepresentationConverter(RepresentationConverter* representationConverter);

    void setXMLFileName(const std::string& xmlDocuFileName);

    std::string getPath(const std::string& suffix = "") const;

private:
    std::string identifier_;

    bool initialized_;

    std::vector<Processor*> processors_;
    std::vector<Property*> properties_;
    std::vector<DataReader*> dataReaders_;
    std::vector<DataWriter*> dataWriters_;
    std::vector<RepresentationConverter*> representationConverters_;
    std::vector<voreen::SerializableFactory*> serializableFactories_;

    std::string xmlDocuFileName_;
};

} // namespace

#endif // IVW_INVIWOMODULE_H
