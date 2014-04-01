/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file author: Daniel J�nsson
 *
 *********************************************************************************/

#include <modules/opencl/kernelowner.h>
#include <modules/opencl/kernelmanager.h>
namespace inviwo {

bool KernelOwner::addKernel(const std::string& filePath,
                            const std::string& kernelName,
                            const std::string& defines /*= ""*/ ) {
    
    cl::Program* program = KernelManager::getRef().buildProgram(filePath, defines);

    cl::Kernel* kernel = KernelManager::getRef().getKernel(program, kernelName, this);
    if (kernel) {
        kernels_.push_back(kernel);
        return true;
    }
    return false;
}

KernelOwner::~KernelOwner() {
    // Make sure that we are not notifed after destruction
    KernelManager::getRef().stopObservingKernels(this);
}

ProcessorKernelOwner::ProcessorKernelOwner(Processor* processor)
    : KernelOwner()
    , processor_(processor) {
}

void ProcessorKernelOwner::onKernelCompiled( const cl::Kernel* kernel ) {
    processor_->invalidate(PropertyOwner::INVALID_RESOURCES);
}

} // namespace
