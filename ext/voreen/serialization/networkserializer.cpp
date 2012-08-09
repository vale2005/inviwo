/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2011 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#include "networkserializer.h"
#include "inviwo/core/network/processornetwork.h"

//#include "voreen/core/datastructures/volume/volumehandle.h"

namespace voreen {

NetworkSerializer::SerializationResource::SerializationResource(ProcessorNetwork* network,
                                                                XmlSerializerBase* serializer,
                                                                const std::string& filename,
                                                                const std::ios_base::openmode openMode)
    : network_(network)
    , serializer_(serializer)
    , stream_(filename.c_str(), openMode)
{
    serializer_->setUseAttributes(true);
}

NetworkSerializer::SerializationResource::~SerializationResource() {
    stream_.close();
}

std::fstream& NetworkSerializer::SerializationResource::getStream() {
    return stream_;
}

ProcessorNetwork* NetworkSerializer::readNetworkFromFile(std::string filename)
    throw (SerializationException)
{
    ProcessorNetwork* net = 0;

    XmlDeserializer d(filename);
    SerializationResource resource(net, &d, filename, std::ios_base::in);

    d.read(resource.getStream(), this);
    d.deserialize("ProcessorNetwork", net);

    return net;
}

void NetworkSerializer::writeNetworkToFile(const ProcessorNetwork* network, const std::string& filename)
    throw (SerializationException)
{
    XmlSerializer s(filename);
    SerializationResource resource(const_cast<ProcessorNetwork*>(network), &s, filename, std::ios_base::out);

    s.serialize("ProcessorNetwork", network);
    s.write(resource.getStream());
}

void NetworkSerializer::process(TiXmlDocument& /*document*/) {
    std::cout << "applying preprocessor" << std::endl;
}

int NetworkSerializer::readVersionFromFile(std::string filename)
    throw (SerializationException)
{
    TiXmlDocument doc(filename);
    if (!doc.LoadFile())
        throw SerializationException("Could not load network file " + filename);
    int version = findVersion(&doc);
    return version;
}

int NetworkSerializer::findVersion(TiXmlNode* node) {
    if (node->Type() != TiXmlNode::ELEMENT) {
        TiXmlNode* pChild;
        for (pChild = node->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) {
            int result=findVersion(pChild);
            if (result!=0)
                return result;
        }
    }
    else {
        TiXmlElement* element=node->ToElement();
        if (element->FirstAttribute() ) {
            TiXmlAttribute* attribute=element->FirstAttribute();
            std::string s = attribute->Name();
            if (s == "Version" || s == "version")
                return attribute->IntValue();
        }
    }
    return 0;
}

TiXmlElement* NetworkSerializer::findProcessorNetworkNode(TiXmlElement* node) {
    const std::string PROCESSORNETWORKNODE = "ProcessorNetwork";

    if (node->ValueStr() == PROCESSORNETWORKNODE)
        return node;

    TiXmlElement* processorNetworkNode = 0;
    for (TiXmlElement* child = node->FirstChildElement(); child != 0; child = child->NextSiblingElement())
        if ((processorNetworkNode = findProcessorNetworkNode(child)) != 0)
            return processorNetworkNode;

    return 0;
}

} // namespace voreen
