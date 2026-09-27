/*
 * Copyright (c) 2022-2023 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#import "Adapter.h"
#import "BindGroup.h"
#import "BindGroupLayout.h"
#import "Buffer.h"
#import "CommandBuffer.h"
#import "CommandEncoder.h"
#import "ComputePassEncoder.h"
#import "ComputePipeline.h"
#import "Device.h"
#import "ExternalTexture.h"
#import "Instance.h"
#import "PipelineLayout.h"
#import "PresentationContext.h"
#import "QuerySet.h"
#import "Queue.h"
#import "RenderBundle.h"
#import "RenderBundleEncoder.h"
#import "RenderPassEncoder.h"
#import "RenderPipeline.h"
#import "Sampler.h"
#import "ShaderModule.h"
#import "Texture.h"
#import "TextureView.h"
#import "WebGPUCppConversions.h"
#import "XRBinding.h"
#import "XRProjectionLayer.h"
#import "XRSubImage.h"
#import "XRView.h"
#import <wtf/BlockPtr.h>
#import <wtf/SwiftBridging.h>
#import <wtf/text/WTFString.h>

namespace WebGPU::Metal {

// A process has only one implementation of the C++ API, so the C++ API objects that WebGPU::Metal
// receives are WebGPU::Metal objects.
inline BindGroupLayout& metal(WebGPU::BindGroupLayout& bindGroupLayout)
{
    return static_cast<BindGroupLayout&>(bindGroupLayout);
}

inline PipelineLayout& metal(WebGPU::PipelineLayout& pipelineLayout)
{
    return static_cast<PipelineLayout&>(pipelineLayout);
}

inline ShaderModule& metal(WebGPU::ShaderModule& shaderModule)
{
    return static_cast<ShaderModule&>(shaderModule);
}

inline Buffer& metal(WebGPU::Buffer& buffer)
{
    return static_cast<Buffer&>(buffer);
}

inline Device& metal(WebGPU::Device& device)
{
    return static_cast<Device&>(device);
}

inline ExternalTexture& metal(WebGPU::ExternalTexture& externalTexture)
{
    return static_cast<ExternalTexture&>(externalTexture);
}

inline QuerySet& metal(WebGPU::QuerySet& querySet)
{
    return static_cast<QuerySet&>(querySet);
}

inline RenderBundle& metal(WebGPU::RenderBundle& renderBundle)
{
    return static_cast<RenderBundle&>(renderBundle);
}

inline Sampler& metal(WebGPU::Sampler& sampler)
{
    return static_cast<Sampler&>(sampler);
}

inline Texture& metal(WebGPU::Texture& texture)
{
    return static_cast<Texture&>(texture);
}

inline TextureView& metal(WebGPU::TextureView& textureView)
{
    return static_cast<TextureView&>(textureView);
}

// For Swift, which cannot call the functions above: it does not see that the WebGPU::Metal classes
// derive from the C++ API classes.
inline Buffer& metal(const Ref<WebGPU::Buffer>& buffer)
{
    return metal(buffer.get());
}

inline QuerySet& metal(const Ref<WebGPU::QuerySet>& querySet)
{
    return metal(querySet.get());
}

inline QuerySet* metalOrNull(const RefPtr<WebGPU::QuerySet>& querySet)
{
    return querySet ? &metal(*querySet) : nullptr;
}

inline Texture& metal(const Ref<WebGPU::Texture>& texture)
{
    return metal(texture.get());
}

// FIXME: It would be cool if we didn't have to list all these overloads, but instead could do something like bridge_cast() in WTF.

inline Adapter& fromAPI(WGPUAdapter handle)
{
    return static_cast<Adapter&>(WebGPU::fromAPI(handle));
}

inline BindGroup& fromAPI(WGPUBindGroup handle)
{
    return static_cast<BindGroup&>(WebGPU::fromAPI(handle));
}

inline BindGroupLayout& fromAPI(WGPUBindGroupLayout handle)
{
    return static_cast<BindGroupLayout&>(WebGPU::fromAPI(handle));
}

inline Buffer& fromAPI(WGPUBuffer handle)
{
    return static_cast<Buffer&>(WebGPU::fromAPI(handle));
}

inline CommandBuffer& fromAPI(WGPUCommandBuffer handle)
{
    return static_cast<CommandBuffer&>(WebGPU::fromAPI(handle));
}

inline CommandEncoder& fromAPI(WGPUCommandEncoder handle)
{
    return static_cast<CommandEncoder&>(WebGPU::fromAPI(handle));
}

inline ComputePassEncoder& fromAPI(WGPUComputePassEncoder handle)
{
    return static_cast<ComputePassEncoder&>(WebGPU::fromAPI(handle));
}

inline ComputePipeline& fromAPI(WGPUComputePipeline handle)
{
    return static_cast<ComputePipeline&>(WebGPU::fromAPI(handle));
}

inline Device& fromAPI(WGPUDevice handle)
{
    return static_cast<Device&>(WebGPU::fromAPI(handle));
}

inline ExternalTexture& fromAPI(WGPUExternalTexture handle)
{
    return static_cast<ExternalTexture&>(WebGPU::fromAPI(handle));
}

inline Instance& fromAPI(WGPUInstance handle)
{
    return static_cast<Instance&>(WebGPU::fromAPI(handle));
}

inline PipelineLayout& fromAPI(WGPUPipelineLayout handle)
{
    return static_cast<PipelineLayout&>(WebGPU::fromAPI(handle));
}

inline QuerySet& fromAPI(WGPUQuerySet handle)
{
    return static_cast<QuerySet&>(WebGPU::fromAPI(handle));
}

inline Queue& fromAPI(WGPUQueue handle)
{
    return static_cast<Queue&>(WebGPU::fromAPI(handle));
}

inline RenderBundle& fromAPI(WGPURenderBundle handle)
{
    return static_cast<RenderBundle&>(WebGPU::fromAPI(handle));
}

inline RenderBundleEncoder& fromAPI(WGPURenderBundleEncoder handle)
{
    return static_cast<RenderBundleEncoder&>(WebGPU::fromAPI(handle));
}

inline RenderPassEncoder& fromAPI(WGPURenderPassEncoder handle)
{
    return static_cast<RenderPassEncoder&>(WebGPU::fromAPI(handle));
}

inline RenderPipeline& fromAPI(WGPURenderPipeline handle)
{
    return static_cast<RenderPipeline&>(WebGPU::fromAPI(handle));
}

inline Sampler& fromAPI(WGPUSampler handle)
{
    return static_cast<Sampler&>(WebGPU::fromAPI(handle));
}

inline ShaderModule& fromAPI(WGPUShaderModule handle)
{
    return static_cast<ShaderModule&>(WebGPU::fromAPI(handle));
}

inline PresentationContext& fromAPI(WGPUSurface handle)
{
    return static_cast<PresentationContext&>(WebGPU::fromAPI(handle));
}

inline Texture& fromAPI(WGPUTexture handle)
{
    return static_cast<Texture&>(WebGPU::fromAPI(handle));
}

inline TextureView& fromAPI(WGPUTextureView handle)
{
    return static_cast<TextureView&>(WebGPU::fromAPI(handle));
}

// Literals have static storage, so the view can borrow them freely.
inline WGPUStringView toAPI(ASCIILiteral literal)
{
    return { literal.characters(), literal.length() };
}

// The view borrows the UTF-8 string.
inline WGPUStringView toAPI(const UTF8CString& string LIFETIME_BOUND)
{
    auto bytes = byteCast<char>(string.span());
    return { bytes.data(), bytes.size() };
}

// A string that the caller of the C API frees, with fastFree() of its data. The *FreeMembers
// functions free the strings of the structs they are for.
inline WGPUStringView toAPIAllocated(const String& string)
{
    auto utf8 = string.utf8();
    auto source = utf8.spanIncludingNullTerminator();
    auto* data = static_cast<char*>(fastMalloc(source.size()));
    memcpySpan(unsafeMakeSpan(data, source.size()), source);
    return { data, utf8.length() };
}

inline void freeAllocated(WGPUStringView string)
{
    fastFree(const_cast<char*>(string.data));
}

// A future of the C API on an instance. complete() is called after the callback it stands for
// has run. Without an instance, the future is WGPU_FUTURE_INIT, which nothing waits on.
class CAPIFuture {
public:
    explicit CAPIFuture(RefPtr<Instance>&& instance)
        : m_instance(WTF::move(instance))
        , m_id(m_instance ? m_instance->createFuture() : 0)
    {
    }

    WGPUFuture future() const { return { m_id }; }
    void complete() const
    {
        if (m_instance)
            m_instance->completeFuture(m_id);
    }

private:
    RefPtr<Instance> m_instance;
    uint64_t m_id { 0 };
};

template<typename R, typename... Args>
inline BlockPtr<R (Args...)> fromAPI(R (^ __strong &&block)(Args...))
{
    return makeBlockPtr(WTF::move(block));
}

// The handle of a new reference, for the C API. The handle of an object is its WebGPU::X.
template <typename T>
inline auto releaseToAPI(Ref<T>&& pointer)
{
    return WebGPU::toAPI(pointer.leakRef());
}

template <typename T>
inline auto releaseToAPI(RefPtr<T>&& pointer) -> decltype(WebGPU::toAPI(*pointer))
{
    // FIXME: We shouldn't need this, because invalid objects should be created instead of returning nullptr.
    if (pointer)
        return WebGPU::toAPI(*pointer.leakRef());
    return nullptr;
}

// For the WebGPU::X objects that a WebGPU::Metal object creates, which are WebGPU::Metal::X objects.
template <typename T, typename U>
inline auto releaseToAPIAs(RefPtr<U>&& pointer)
{
    return releaseToAPI(WTF::move(pointer));
}

template <typename T, typename U>
inline auto releaseToAPIAs(Ref<U>&& pointer)
{
    return releaseToAPI(WTF::move(pointer));
}

} // namespace WebGPU::Metal
