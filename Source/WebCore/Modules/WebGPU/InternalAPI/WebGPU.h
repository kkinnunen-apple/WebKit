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

#include "WebGPUXRBinding.h"
#include "WebGPUXRProjectionLayer.h"
#include "WebGPUXRSubImage.h"
#include "WebGPUXRView.h"
#include <WebCore/WebGPUAdapter.h>
#include <WebCore/WebGPUBindGroup.h>
#include <WebCore/WebGPUBindGroupLayout.h>
#include <WebCore/WebGPUBuffer.h>
#include <WebCore/WebGPUCommandBuffer.h>
#include <WebCore/WebGPUCommandEncoder.h>
#include <WebCore/WebGPUComputePassEncoder.h>
#include <WebCore/WebGPUComputePipeline.h>
#include <WebCore/WebGPUDevice.h>
#include <WebCore/WebGPUExtent3D.h>
#include <WebCore/WebGPUExternalTexture.h>
#include <WebCore/WebGPUPipelineLayout.h>
#include <WebCore/WebGPUPresentationContext.h>
#include <WebCore/WebGPUQuerySet.h>
#include <WebCore/WebGPUQueue.h>
#include <WebCore/WebGPURenderBundle.h>
#include <WebCore/WebGPURenderBundleEncoder.h>
#include <WebCore/WebGPURenderPassEncoder.h>
#include <WebCore/WebGPURenderPipeline.h>
#include <WebCore/WebGPURequestAdapterOptions.h>
#include <WebCore/WebGPUSampler.h>
#include <WebCore/WebGPUShaderModule.h>
#include <WebCore/WebGPUTexture.h>
#include <WebCore/WebGPUTextureView.h>
#include <optional>
#include <wtf/AbstractRefCounted.h>
#include <wtf/CompletionHandler.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>

#if PLATFORM(COCOA) && ENABLE(VIDEO)
#include <WebCore/MediaPlayerIdentifier.h>
#endif

namespace WebCore {
class NativeImage;
class IntSize;
class GraphicsContext;
class VideoFrame;
}

namespace WebCore::WebGPU {

class CompositorIntegration;
class GPU;
class GPUImpl;

struct ExternalTextureDescriptor;
struct ImageCopyExternalImage;
struct ImageCopyTextureTagged;
struct PresentationContextDescriptor;

class GPU : public AbstractRefCounted {
public:
    WEBCORE_EXPORT virtual ~GPU() = default;

    virtual void requestAdapter(const RequestAdapterOptions&, CompletionHandler<void(RefPtr<Adapter>&&)>&&) = 0;

    virtual RefPtr<PresentationContext> createPresentationContext(const PresentationContextDescriptor&) = 0;

    virtual RefPtr<CompositorIntegration> createCompositorIntegration() = 0;
    virtual void paintToCanvas(WebCore::NativeImage&, const WebCore::IntSize&, WebCore::GraphicsContext&) = 0;

    // The queue commands that take WebCore sources, which the WebGPU implementations cannot see.
    virtual void copyExternalImageToTexture(Queue&, const ImageCopyExternalImage& source, const ImageCopyTextureTagged& destination, const Extent3D& copySize) = 0;
    virtual RefPtr<WebCore::NativeImage> nativeImage(Queue&, WebCore::VideoFrame&) = 0;
    // The device commands that take WebCore sources.
    virtual RefPtr<ExternalTexture> importExternalTexture(Device&, const ExternalTextureDescriptor&) = 0;
#if PLATFORM(COCOA) && ENABLE(VIDEO)
    virtual void updateExternalTexture(Device&, const ExternalTexture&, const WebCore::MediaPlayerIdentifier&) = 0;
#endif
    virtual bool isValid(const CompositorIntegration&) const = 0;
    virtual bool isValid(const Buffer&) const = 0;
    virtual bool isValid(const Adapter&) const = 0;
    virtual bool isValid(const BindGroup&) const = 0;
    virtual bool isValid(const BindGroupLayout&) const = 0;
    virtual bool isValid(const CommandBuffer&) const = 0;
    virtual bool isValid(const CommandEncoder&) const = 0;
    virtual bool isValid(const ComputePassEncoder&) const = 0;
    virtual bool isValid(const ComputePipeline&) const = 0;
    virtual bool isValid(const Device&) const = 0;
    virtual bool isValid(const ExternalTexture&) const = 0;
    virtual bool isValid(const PipelineLayout&) const = 0;
    virtual bool isValid(const PresentationContext&) const = 0;
    virtual bool isValid(const QuerySet&) const = 0;
    virtual bool isValid(const Queue&) const = 0;
    virtual bool isValid(const RenderBundleEncoder&) const = 0;
    virtual bool isValid(const RenderBundle&) const = 0;
    virtual bool isValid(const RenderPassEncoder&) const = 0;
    virtual bool isValid(const RenderPipeline&) const = 0;
    virtual bool isValid(const Sampler&) const = 0;
    virtual bool isValid(const ShaderModule&) const = 0;
    virtual bool isValid(const Texture&) const = 0;
    virtual bool isValid(const TextureView&) const = 0;
    virtual bool isValid(const XRBinding&) const = 0;
    virtual bool isValid(const XRSubImage&) const = 0;
    virtual bool isValid(const XRProjectionLayer&) const = 0;
    virtual bool isValid(const XRView&) const = 0;

    virtual bool isRemoteGPUProxy() const { return false; }
    virtual bool isGPUImpl() const { return false; }

protected:
    GPU() = default;

private:
    GPU(const GPU&) = delete;
    GPU(GPU&&) = delete;
    GPU& operator=(const GPU&) = delete;
    GPU& operator=(GPU&&) = delete;
};

} // namespace WebCore::WebGPU
