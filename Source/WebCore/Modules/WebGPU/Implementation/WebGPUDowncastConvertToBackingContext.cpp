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

#include "config.h"
#include "WebGPUDowncastConvertToBackingContext.h"

#if HAVE(WEBGPU_IMPLEMENTATION)

#include "WebGPUAdapterImpl.h"
#include "WebGPUBindGroupImpl.h"
#include "WebGPUBindGroupLayout.h"
#include "WebGPUBuffer.h"
#include "WebGPUCommandBufferImpl.h"
#include "WebGPUCommandEncoderImpl.h"
#include "WebGPUCompositorIntegrationImpl.h"
#include "WebGPUComputePassEncoderImpl.h"
#include "WebGPUComputePipelineImpl.h"
#include "WebGPUDeviceImpl.h"
#include "WebGPUExternalTexture.h"
#include "WebGPUImpl.h"
#include "WebGPUPipelineLayoutImpl.h"
#include "WebGPUPresentationContextImpl.h"
#include "WebGPUQuerySet.h"
#include "WebGPUQueueImpl.h"
#include "WebGPURenderBundleEncoderImpl.h"
#include "WebGPURenderBundleImpl.h"
#include "WebGPURenderPassEncoderImpl.h"
#include "WebGPURenderPipelineImpl.h"
#include "WebGPUSampler.h"
#include "WebGPUShaderModule.h"
#include "WebGPUTexture.h"
#include "WebGPUTextureView.h"
#include "WebGPUXRBindingImpl.h"
#include "WebGPUXRProjectionLayerImpl.h"
#include "WebGPUXRSubImageImpl.h"
#include "WebGPUXRViewImpl.h"
#include <WebGPU/WebGPUCppBridge.h>
#include <wtf/TZoneMallocInlines.h>

namespace WebCore::WebGPU {

WTF_MAKE_TZONE_ALLOCATED_IMPL(DowncastConvertToBackingContext);

WGPUAdapter DowncastConvertToBackingContext::convertToBacking(const Adapter& adapter)
{
    return downcast<AdapterImpl>(adapter).backing();
}

WGPUBindGroup DowncastConvertToBackingContext::convertToBacking(const BindGroup& bindGroup)
{
    return downcast<BindGroupImpl>(bindGroup).backing();
}

WGPUBindGroupLayout DowncastConvertToBackingContext::convertToBacking(const BindGroupLayout& bindGroupLayout)
{
    return ::WebGPU::toAPI(const_cast<BindGroupLayout&>(bindGroupLayout));
}

WGPUBuffer DowncastConvertToBackingContext::convertToBacking(const Buffer& buffer)
{
    return ::WebGPU::toAPI(const_cast<Buffer&>(buffer));
}

WGPUCommandBuffer DowncastConvertToBackingContext::convertToBacking(const CommandBuffer& commandBuffer)
{
    return downcast<CommandBufferImpl>(commandBuffer).backing();
}

WGPUCommandEncoder DowncastConvertToBackingContext::convertToBacking(const CommandEncoder& commandEncoder)
{
    return downcast<CommandEncoderImpl>(commandEncoder).backing();
}

WGPUComputePassEncoder DowncastConvertToBackingContext::convertToBacking(const ComputePassEncoder& computePassEncoder)
{
    return downcast<ComputePassEncoderImpl>(computePassEncoder).backing();
}

WGPUComputePipeline DowncastConvertToBackingContext::convertToBacking(const ComputePipeline& computePipeline)
{
    return downcast<ComputePipelineImpl>(computePipeline).backing();
}

WGPUDevice DowncastConvertToBackingContext::convertToBacking(const Device& device)
{
    return downcast<DeviceImpl>(device).backing();
}

WGPUExternalTexture DowncastConvertToBackingContext::convertToBacking(const ExternalTexture& externalTexture)
{
    return ::WebGPU::toAPI(const_cast<ExternalTexture&>(externalTexture));
}

WGPUInstance DowncastConvertToBackingContext::convertToBacking(const GPU& gpu)
{
    return downcast<GPUImpl>(gpu).backing();
}

WGPUPipelineLayout DowncastConvertToBackingContext::convertToBacking(const PipelineLayout& pipelineLayout)
{
    return downcast<PipelineLayoutImpl>(pipelineLayout).backing();
}

WGPUSurface DowncastConvertToBackingContext::convertToBacking(const PresentationContext& presentationContext)
{
    return downcast<PresentationContextImpl>(presentationContext).backing();
}

WGPUQuerySet DowncastConvertToBackingContext::convertToBacking(const QuerySet& querySet)
{
    return ::WebGPU::toAPI(const_cast<QuerySet&>(querySet));
}

WGPUQueue DowncastConvertToBackingContext::convertToBacking(const Queue& queue)
{
    return downcast<QueueImpl>(queue).backing();
}

WGPURenderBundleEncoder DowncastConvertToBackingContext::convertToBacking(const RenderBundleEncoder& renderBundleEncoder)
{
    return downcast<RenderBundleEncoderImpl>(renderBundleEncoder).backing();
}

WGPURenderBundle DowncastConvertToBackingContext::convertToBacking(const RenderBundle& renderBundle)
{
    return downcast<RenderBundleImpl>(renderBundle).backing();
}

WGPURenderPassEncoder DowncastConvertToBackingContext::convertToBacking(const RenderPassEncoder& renderPassEncoder)
{
    return downcast<RenderPassEncoderImpl>(renderPassEncoder).backing();
}

WGPURenderPipeline DowncastConvertToBackingContext::convertToBacking(const RenderPipeline& renderPipeline)
{
    return downcast<RenderPipelineImpl>(renderPipeline).backing();
}

WGPUSampler DowncastConvertToBackingContext::convertToBacking(const Sampler& sampler)
{
    return ::WebGPU::toAPI(const_cast<Sampler&>(sampler));
}

WGPUShaderModule DowncastConvertToBackingContext::convertToBacking(const ShaderModule& shaderModule)
{
    return ::WebGPU::toAPI(const_cast<ShaderModule&>(shaderModule));
}

WGPUTexture DowncastConvertToBackingContext::convertToBacking(const Texture& texture)
{
    return ::WebGPU::toAPI(const_cast<Texture&>(texture));
}

WGPUTextureView DowncastConvertToBackingContext::convertToBacking(const TextureView& textureView)
{
    return ::WebGPU::toAPI(const_cast<TextureView&>(textureView));
}

CompositorIntegrationImpl& DowncastConvertToBackingContext::convertToBacking(CompositorIntegration& compositorIntegration)
{
    return downcast<CompositorIntegrationImpl>(compositorIntegration);
}

WGPUXRBinding DowncastConvertToBackingContext::convertToBacking(const XRBinding& xrBinding)
{
    return downcast<XRBindingImpl>(xrBinding).backing();
}

WGPUXRProjectionLayer DowncastConvertToBackingContext::convertToBacking(const XRProjectionLayer& layer)
{
    return downcast<XRProjectionLayerImpl>(layer).backing();
}

WGPUXRSubImage DowncastConvertToBackingContext::convertToBacking(const XRSubImage& subImage)
{
    return downcast<XRSubImageImpl>(subImage).backing();
}

WGPUXRView DowncastConvertToBackingContext::convertToBacking(const XRView& xrView)
{
    return downcast<XRViewImpl>(xrView).backing();
}

} // namespace WebCore::WebGPU

#endif // HAVE(WEBGPU_IMPLEMENTATION)
