/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2017 Inviwo Foundation
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
 *********************************************************************************/

#include <inviwo/core/datastructures/buffer/bufferram.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

namespace inviwo {

BufferRAM::BufferRAM(const DataFormatBase* format, BufferUsage usage, BufferTarget target)
    : BufferRepresentation(format, usage, target) {}

std::type_index BufferRAM::getTypeIndex() const { return std::type_index(typeid(BufferRAM)); }

struct BufferRamCreationDispatcher {
    using type = std::shared_ptr<BufferRAM>;
    template <class T>
    std::shared_ptr<BufferRAM> dispatch(size_t size, BufferUsage usage, BufferTarget target) {
        typedef typename T::type F;
        switch (target) {
            case BufferTarget::Index:
                return std::make_shared<BufferRAMPrecision<F, BufferTarget::Index>>(size, usage);
            case BufferTarget::Data:
            default:
                return std::make_shared<BufferRAMPrecision<F, BufferTarget::Data>>(size, usage);
        }
    }
};

std::shared_ptr<BufferRAM> createBufferRAM(size_t size, const DataFormatBase* format,
                                           BufferUsage usage, BufferTarget target) {

    BufferRamCreationDispatcher disp;
    return format->dispatch(disp, size, usage, target);
}
}
