/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Erik Sund�n
 *
 *********************************************************************************/

#ifndef IVW_LAYERREPRESENTATION_H
#define IVW_LAYERREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/datastructures/image/layer.h>
#include <inviwo/core/datastructures/image/imagetypes.h>

namespace inviwo {

class IVW_CORE_API LayerRepresentation : public DataRepresentation {

    friend class Layer;

public:
    LayerRepresentation(uvec2 dimensions = uvec2(256,256), LayerType type = COLOR_LAYER, const DataFormatBase* format = DataVec4UINT8::get());
    LayerRepresentation(const LayerRepresentation& rhs);
    LayerRepresentation& operator=(const LayerRepresentation& that);
    virtual LayerRepresentation* clone() const = 0;
    virtual ~LayerRepresentation();

    virtual void performOperation(DataOperation*) const {};
    virtual void resize(uvec2 dimensions);
    virtual bool copyAndResizeLayer(DataRepresentation*) const = 0;

    uvec2 getDimension() const;
    // Removes old data and reallocate for new dimension.
    // Needs to be overloaded by child classes.
    virtual void setDimension(uvec2 dimensions);

    LayerType getLayerType() const;

protected:
    uvec2 dimensions_;
    LayerType layerType_;
};

} // namespace

#endif // IVW_LAYERREPRESENTATION_H
