/*
 * Copyright (C) 2021-2023 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <WebCore/WebGPUBindGroup.h>
#include <WebCore/WebGPUBindGroupDescriptor.h>
#include <WebCore/WebGPUBindGroupLayout.h>
#include <WebCore/WebGPUBindGroupLayoutDescriptor.h>
#include <WebCore/WebGPUBuffer.h>
#include <WebCore/WebGPUBufferDescriptor.h>
#include <WebCore/WebGPUCommandBuffer.h>
#include <WebCore/WebGPUCommandEncoder.h>
#include <WebCore/WebGPUCommandEncoderDescriptor.h>
#include <WebCore/WebGPUComputePassEncoder.h>
#include <WebCore/WebGPUComputePipeline.h>
#include <WebCore/WebGPUCppAPI.h>
#include <WebCore/WebGPUErrorFilter.h>
#include <WebCore/WebGPUExternalTexture.h>
#include <WebCore/WebGPUPipelineLayout.h>
#include <WebCore/WebGPUPipelineLayoutDescriptor.h>
#include <WebCore/WebGPUQuerySet.h>
#include <WebCore/WebGPUQuerySetDescriptor.h>
#include <WebCore/WebGPUQueue.h>
#include <WebCore/WebGPURenderBundleEncoder.h>
#include <WebCore/WebGPURenderPassEncoder.h>
#include <WebCore/WebGPURenderPipeline.h>
#include <WebCore/WebGPUSampler.h>
#include <WebCore/WebGPUSamplerDescriptor.h>
#include <WebCore/WebGPUShaderModule.h>
#include <WebCore/WebGPUTexture.h>
#include <WebCore/WebGPUTextureDescriptor.h>
#include <wtf/CompletionHandler.h>
#include <wtf/text/WTFString.h>

namespace WebCore::WebGPU {

struct ComputePipelineDescriptor;
struct RenderPipelineDescriptor;
struct ShaderModuleDescriptor;

using Device = ::WebGPU::Device;

// WebCore keeps these descriptors, which own their arrays. These functions create the objects from
// them through the WebGPU::Device, which borrows the arrays for the call.
RefPtr<ShaderModule> createShaderModule(Device&, const ShaderModuleDescriptor&);
RefPtr<ComputePipeline> createComputePipeline(Device&, const ComputePipelineDescriptor&);
RefPtr<RenderPipeline> createRenderPipeline(Device&, const RenderPipelineDescriptor&);
void createComputePipelineAsync(Device&, const ComputePipelineDescriptor&, CompletionHandler<void(Expected<Ref<ComputePipeline>, ::WebGPU::PipelineError>&&)>&&);
void createRenderPipelineAsync(Device&, const RenderPipelineDescriptor&, CompletionHandler<void(Expected<Ref<RenderPipeline>, ::WebGPU::PipelineError>&&)>&&);
void createComputePipelineWithPipelineLayoutFromPipelineAsync(Device&, const ComputePipelineDescriptor&, const ComputePipeline& pipelineToReplace, CompletionHandler<void(Expected<Ref<ComputePipeline>, ::WebGPU::PipelineError>&&)>&&);
void createRenderPipelineWithPipelineLayoutFromPipelineAsync(Device&, const RenderPipelineDescriptor&, const RenderPipeline& pipelineToReplace, CompletionHandler<void(Expected<Ref<RenderPipeline>, ::WebGPU::PipelineError>&&)>&&);

} // namespace WebCore::WebGPU
