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
#include "RemoteAdapterProxy.h"

#if ENABLE(GPU_PROCESS)

#include "RemoteAdapterMessages.h"
#include "RemoteDeviceProxy.h"
#include "WebGPUConvertToBackingContext.h"
#include <wtf/TZoneMallocInlines.h>

namespace WebKit::WebGPU {

WTF_MAKE_TZONE_ALLOCATED_IMPL(RemoteAdapterProxy);

RemoteAdapterProxy::RemoteAdapterProxy(String&& name, WebCore::WebGPU::SupportedFeatures& features, WebCore::WebGPU::SupportedLimits& limits, bool isFallbackAdapter, bool xrCompatible, RemoteGPUProxy& parent, ConvertToBackingContext& convertToBackingContext, WebGPUIdentifier identifier, uint32_t subgroupMinSize, uint32_t subgroupMaxSize)
    : Adapter(WTF::move(name), features, limits, isFallbackAdapter, subgroupMinSize, subgroupMaxSize)
    , m_backing(identifier)
    , m_convertToBackingContext(convertToBackingContext)
    , m_parent(parent)
    , m_xrCompatible(xrCompatible)
{
}

RemoteAdapterProxy::~RemoteAdapterProxy()
{
    auto sendResult = send(Messages::RemoteAdapter::Destruct());
    UNUSED_VARIABLE(sendResult);
}

void RemoteAdapterProxy::requestDevice(const WebCore::WebGPU::DeviceDescriptor& descriptor, CompletionHandler<void(RefPtr<WebCore::WebGPU::Device>&&)>&& callback)
{
    Ref convertToBackingContext = m_convertToBackingContext;
    auto convertedDescriptor = convertToBackingContext->convertToBacking(descriptor);
    ASSERT(convertedDescriptor);
    if (!convertedDescriptor)
        return callback(nullptr);

    auto identifier = WebGPUIdentifier::generate();
    auto queueIdentifier = WebGPUIdentifier::generate();
    auto sendResult = sendSync(Messages::RemoteAdapter::RequestDevice(*convertedDescriptor, identifier, queueIdentifier));
    if (!sendResult.succeeded())
        return callback(nullptr);

    auto [features, supportedLimits] = sendResult.takeReply();
    if (!supportedLimits.maxTextureDimension2D) {
        callback(nullptr);
        return;
    }

    ::WebGPU::Limits limits {
        .maxTextureDimension1D = supportedLimits.maxTextureDimension1D,
        .maxTextureDimension2D = supportedLimits.maxTextureDimension2D,
        .maxTextureDimension3D = supportedLimits.maxTextureDimension3D,
        .maxTextureArrayLayers = supportedLimits.maxTextureArrayLayers,
        .maxBindGroups = supportedLimits.maxBindGroups,
        .maxBindGroupsPlusVertexBuffers = supportedLimits.maxBindGroupsPlusVertexBuffers,
        .maxBindingsPerBindGroup = supportedLimits.maxBindingsPerBindGroup,
        .maxDynamicUniformBuffersPerPipelineLayout = supportedLimits.maxDynamicUniformBuffersPerPipelineLayout,
        .maxDynamicStorageBuffersPerPipelineLayout = supportedLimits.maxDynamicStorageBuffersPerPipelineLayout,
        .maxSampledTexturesPerShaderStage = supportedLimits.maxSampledTexturesPerShaderStage,
        .maxSamplersPerShaderStage = supportedLimits.maxSamplersPerShaderStage,
        .maxStorageBuffersPerShaderStage = supportedLimits.maxStorageBuffersPerShaderStage,
        .maxStorageTexturesPerShaderStage = supportedLimits.maxStorageTexturesPerShaderStage,
        .maxUniformBuffersPerShaderStage = supportedLimits.maxUniformBuffersPerShaderStage,
        .maxUniformBufferBindingSize = supportedLimits.maxUniformBufferBindingSize,
        .maxStorageBufferBindingSize = supportedLimits.maxStorageBufferBindingSize,
        .minUniformBufferOffsetAlignment = supportedLimits.minUniformBufferOffsetAlignment,
        .minStorageBufferOffsetAlignment = supportedLimits.minStorageBufferOffsetAlignment,
        .maxVertexBuffers = supportedLimits.maxVertexBuffers,
        .maxBufferSize = supportedLimits.maxBufferSize,
        .maxVertexAttributes = supportedLimits.maxVertexAttributes,
        .maxVertexBufferArrayStride = supportedLimits.maxVertexBufferArrayStride,
        .maxInterStageShaderVariables = supportedLimits.maxInterStageShaderVariables,
        .maxColorAttachments = supportedLimits.maxColorAttachments,
        .maxColorAttachmentBytesPerSample = supportedLimits.maxColorAttachmentBytesPerSample,
        .maxComputeWorkgroupStorageSize = supportedLimits.maxComputeWorkgroupStorageSize,
        .maxComputeInvocationsPerWorkgroup = supportedLimits.maxComputeInvocationsPerWorkgroup,
        .maxComputeWorkgroupSizeX = supportedLimits.maxComputeWorkgroupSizeX,
        .maxComputeWorkgroupSizeY = supportedLimits.maxComputeWorkgroupSizeY,
        .maxComputeWorkgroupSizeZ = supportedLimits.maxComputeWorkgroupSizeZ,
        .maxComputeWorkgroupsPerDimension = supportedLimits.maxComputeWorkgroupsPerDimension,
        .maxStorageBuffersInFragmentStage = supportedLimits.maxStorageBuffersInFragmentStage,
        .maxStorageTexturesInFragmentStage = supportedLimits.maxStorageTexturesInFragmentStage,
        .maxStorageBuffersInVertexStage = supportedLimits.maxStorageBuffersInVertexStage,
        .maxStorageTexturesInVertexStage = supportedLimits.maxStorageTexturesInVertexStage,
    };
    auto result = RemoteDeviceProxy::create(WTF::move(features), limits, *this, convertToBackingContext, identifier, queueIdentifier);
    result->setLabel(WTF::move(convertedDescriptor->label));
    callback(WTF::move(result));
}

bool RemoteAdapterProxy::xrCompatible()
{
    return m_xrCompatible;
}

} // namespace WebKit::WebGPU

#endif // HAVE(GPU_PROCESS)
