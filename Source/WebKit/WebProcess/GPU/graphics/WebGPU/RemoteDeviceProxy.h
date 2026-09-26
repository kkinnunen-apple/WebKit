/*
 * Copyright (C) 2021-2025 Apple Inc. All rights reserved.
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

#if ENABLE(GPU_PROCESS)

#include "RemoteAdapterProxy.h"
#include "SharedVideoFrame.h"
#include "WebGPUIdentifier.h"
#include <WebCore/WebGPUCommandEncoderDescriptor.h>
#include <WebCore/WebGPUDevice.h>
#include <WebCore/WebGPUDeviceLostReason.h>
#include <WebCore/WebGPUErrorFilter.h>
#include <WebCore/WebGPUFeatureName.h>
#include <WebCore/WebGPURenderBundleEncoderDescriptor.h>
#include <wtf/TZoneMalloc.h>

#if PLATFORM(COCOA) && ENABLE(VIDEO)
#include <WebCore/MediaPlayerIdentifier.h>
#endif

namespace WebKit::WebGPU {

class ConvertToBackingContext;
class RemoteQueueProxy;

class RemoteDeviceProxy final : public WebCore::WebGPU::Device {
    WTF_MAKE_TZONE_ALLOCATED(RemoteDeviceProxy);
public:
    static Ref<RemoteDeviceProxy> create(Vector<WebCore::WebGPU::FeatureName>&& features, const ::WebGPU::Limits& limits, RemoteAdapterProxy& parent, ConvertToBackingContext& convertToBackingContext, WebGPUIdentifier identifier, WebGPUIdentifier queueIdentifier)
    {
        return adoptRef(*new RemoteDeviceProxy(WTF::move(features), limits, parent, convertToBackingContext, identifier, queueIdentifier));
    }

    virtual ~RemoteDeviceProxy();

    RemoteAdapterProxy& parent() const { return m_parent; }
    RemoteGPUProxy& root() { return m_parent->root(); }

    // Called by RemoteGPUProxy, which implements the device commands that take WebCore sources.
    RefPtr<WebCore::WebGPU::ExternalTexture> importExternalTexture(const WebCore::WebGPU::ExternalTextureDescriptor&);
#if PLATFORM(COCOA) && ENABLE(VIDEO)
    void updateExternalTexture(const WebCore::WebGPU::ExternalTexture&, const WebCore::MediaPlayerIdentifier&);
#endif
    WebGPUIdentifier backing() const { return m_backing; }

    Vector<WebCore::WebGPU::FeatureName> features() const final { return m_features; }
    const ::WebGPU::Limits& limits() const LIFETIME_BOUND final { return m_limits; }
    Ref<WebCore::WebGPU::Queue> NODELETE queue() final;
    void destroy() final;

    RefPtr<WebCore::WebGPU::Buffer> createBuffer(const WebCore::WebGPU::BufferDescriptor&) final;
    RefPtr<WebCore::WebGPU::Texture> createTexture(const WebCore::WebGPU::TextureDescriptor&) final;
    RefPtr<WebCore::WebGPU::Sampler> createSampler(const WebCore::WebGPU::SamplerDescriptor&) final;
#if PLATFORM(COCOA)
    RefPtr<WebCore::WebGPU::ExternalTexture> importExternalTexture(const ::WebGPU::ExternalTextureDescriptor&) final;
#endif
    RefPtr<WebCore::WebGPU::BindGroupLayout> createBindGroupLayout(const WebCore::WebGPU::BindGroupLayoutDescriptor&) final;
    RefPtr<WebCore::WebGPU::PipelineLayout> createPipelineLayout(const WebCore::WebGPU::PipelineLayoutDescriptor&) final;
    RefPtr<WebCore::WebGPU::BindGroup> createBindGroup(const WebCore::WebGPU::BindGroupDescriptor&) final;
    RefPtr<WebCore::WebGPU::ShaderModule> createShaderModule(const ::WebGPU::ShaderModuleDescriptor&) final;
    RefPtr<WebCore::WebGPU::ComputePipeline> createComputePipeline(const ::WebGPU::ComputePipelineDescriptor&) final;
    RefPtr<WebCore::WebGPU::RenderPipeline> createRenderPipeline(const ::WebGPU::RenderPipelineDescriptor&) final;
    void createComputePipelineAsync(const ::WebGPU::ComputePipelineDescriptor&, CompletionHandler<void(Expected<Ref<WebCore::WebGPU::ComputePipeline>, ::WebGPU::PipelineError>&&)>&&) final;
    void createRenderPipelineAsync(const ::WebGPU::RenderPipelineDescriptor&, CompletionHandler<void(Expected<Ref<WebCore::WebGPU::RenderPipeline>, ::WebGPU::PipelineError>&&)>&&) final;
    void createComputePipelineWithPipelineLayoutFromPipelineAsync(const ::WebGPU::ComputePipelineDescriptor&, const WebCore::WebGPU::ComputePipeline&, CompletionHandler<void(Expected<Ref<WebCore::WebGPU::ComputePipeline>, ::WebGPU::PipelineError>&&)>&&) final;
    void createRenderPipelineWithPipelineLayoutFromPipelineAsync(const ::WebGPU::RenderPipelineDescriptor&, const WebCore::WebGPU::RenderPipeline&, CompletionHandler<void(Expected<Ref<WebCore::WebGPU::RenderPipeline>, ::WebGPU::PipelineError>&&)>&&) final;
    RefPtr<WebCore::WebGPU::CommandEncoder> createCommandEncoder(const WebCore::WebGPU::CommandEncoderDescriptor&) final;
    RefPtr<WebCore::WebGPU::RenderBundleEncoder> createRenderBundleEncoder(const ::WebGPU::RenderBundleEncoderDescriptor&) final;
    RefPtr<WebCore::WebGPU::QuerySet> createQuerySet(const WebCore::WebGPU::QuerySetDescriptor&) final;
    RefPtr<WebCore::WebGPU::XRBinding> createXRBinding() final;

    void pushErrorScope(WebCore::WebGPU::ErrorFilter) final;
    void popErrorScope(CompletionHandler<void(bool, std::optional<::WebGPU::Error>&&)>&&) final;
    void resolveUncapturedErrorEvent(CompletionHandler<void(bool, std::optional<::WebGPU::Error>&&)>&&) final;
    void resolveDeviceLostPromise(CompletionHandler<void(WebCore::WebGPU::DeviceLostReason, String&&)>&&) final;
    void pauseAllErrorReporting(bool pause) final;
    void setLabel(String&&) final;
    bool isValid() const final;

private:
    friend class DowncastConvertToBackingContext;

    RemoteDeviceProxy(Vector<WebCore::WebGPU::FeatureName>&&, const ::WebGPU::Limits&, RemoteAdapterProxy&, ConvertToBackingContext&, WebGPUIdentifier, WebGPUIdentifier queueIdentifier);

    RemoteDeviceProxy(const RemoteDeviceProxy&) = delete;
    RemoteDeviceProxy(RemoteDeviceProxy&&) = delete;
    RemoteDeviceProxy& operator=(const RemoteDeviceProxy&) = delete;
    RemoteDeviceProxy& operator=(RemoteDeviceProxy&&) = delete;

    template<typename T>
    [[nodiscard]] IPC::Error send(T&& message)
    {
        return protect(root().streamClientConnection())->send(std::forward<T>(message), backing());
    }
    template<typename T, typename C>
    [[nodiscard]] std::optional<IPC::StreamClientConnection::AsyncReplyID> sendWithAsyncReply(T&& message, C&& completionHandler)
    {
        return protect(root().streamClientConnection())->sendWithAsyncReply(std::forward<T>(message), std::forward<C>(completionHandler), backing());
    }

    WebGPUIdentifier m_backing;
    const Vector<WebCore::WebGPU::FeatureName> m_features;
    const ::WebGPU::Limits m_limits;
    const Ref<ConvertToBackingContext> m_convertToBackingContext;
    const Ref<RemoteAdapterProxy> m_parent;
    const Ref<RemoteQueueProxy> m_queue;
#if PLATFORM(COCOA) && ENABLE(VIDEO)
    WebKit::SharedVideoFrameWriter m_sharedVideoFrameWriter;
#endif
};

} // namespace WebKit::WebGPU

#endif // ENABLE(GPU_PROCESS)
