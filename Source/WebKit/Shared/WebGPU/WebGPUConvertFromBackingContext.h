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

#if ENABLE(GPU_PROCESS)

#include "WebGPUColor.h"
#include "WebGPUCommandEncoderDescriptor.h"
#include "WebGPUCompilationMessage.h"
#include "WebGPUComputePassTimestampWrites.h"
#include "WebGPUError.h"
#include "WebGPUExtent3D.h"
#include "WebGPUIdentifier.h"
#include "WebGPUOrigin2D.h"
#include "WebGPUOrigin3D.h"
#include "WebGPURenderPassTimestampWrites.h"
#include "WebGPUTextureViewDescriptor.h"
#include <WebCore/WebGPUAdapter.h>
#include <WebCore/WebGPUBindGroup.h>
#include <WebCore/WebGPUBindGroupDescriptor.h>
#include <WebCore/WebGPUBindGroupLayout.h>
#include <WebCore/WebGPUBindGroupLayoutDescriptor.h>
#include <WebCore/WebGPUBlendComponent.h>
#include <WebCore/WebGPUBlendState.h>
#include <WebCore/WebGPUBuffer.h>
#include <WebCore/WebGPUBufferBindingLayout.h>
#include <WebCore/WebGPUColor.h>
#include <WebCore/WebGPUCommandBuffer.h>
#include <WebCore/WebGPUCommandEncoder.h>
#include <WebCore/WebGPUComputePassDescriptor.h>
#include <WebCore/WebGPUComputePassEncoder.h>
#include <WebCore/WebGPUComputePassTimestampWrites.h>
#include <WebCore/WebGPUComputePipeline.h>
#include <WebCore/WebGPUDevice.h>
#include <WebCore/WebGPUError.h>
#include <WebCore/WebGPUExtent3D.h>
#include <WebCore/WebGPUExternalTexture.h>
#include <WebCore/WebGPUExternalTextureBindingLayout.h>
#include <WebCore/WebGPUImageCopyBuffer.h>
#include <WebCore/WebGPUImageCopyTexture.h>
#include <WebCore/WebGPUImageDataLayout.h>
#include <WebCore/WebGPUMultisampleState.h>
#include <WebCore/WebGPUOrigin2D.h>
#include <WebCore/WebGPUOrigin3D.h>
#include <WebCore/WebGPUPipelineLayout.h>
#include <WebCore/WebGPUPipelineLayoutDescriptor.h>
#include <WebCore/WebGPUPresentationContext.h>
#include <WebCore/WebGPUPrimitiveState.h>
#include <WebCore/WebGPUQuerySet.h>
#include <WebCore/WebGPUQueue.h>
#include <WebCore/WebGPURenderBundle.h>
#include <WebCore/WebGPURenderBundleEncoder.h>
#include <WebCore/WebGPURenderPassDescriptor.h>
#include <WebCore/WebGPURenderPassEncoder.h>
#include <WebCore/WebGPURenderPassTimestampWrites.h>
#include <WebCore/WebGPURenderPipeline.h>
#include <WebCore/WebGPUSampler.h>
#include <WebCore/WebGPUSamplerBindingLayout.h>
#include <WebCore/WebGPUShaderModule.h>
#include <WebCore/WebGPUStencilFaceState.h>
#include <WebCore/WebGPUStorageTextureBindingLayout.h>
#include <WebCore/WebGPUTexture.h>
#include <WebCore/WebGPUTextureBindingLayout.h>
#include <WebCore/WebGPUTextureDescriptor.h>
#include <WebCore/WebGPUVertexAttribute.h>
#include <optional>
#include <wtf/RefCounted.h>
#include <wtf/ThreadSafeWeakPtr.h>

#if ENABLE(VIDEO) && PLATFORM(COCOA)
typedef struct CF_BRIDGED_TYPE(id) __CVBuffer* CVPixelBufferRef;

namespace WebCore {
enum class VideoFrameRotation : uint16_t;
}
#endif

namespace WebCore::WebGPU {

struct CanvasConfiguration;
struct ColorTargetState;
class CompositorIntegration;
struct ComputePipelineDescriptor;
struct DepthStencilState;
struct DeviceDescriptor;
struct ExternalTextureDescriptor;
struct FragmentState;
class GPU;
struct Identifier;
struct ImageCopyExternalImage;
struct ImageCopyTextureTagged;
class InternalError;
struct ObjectDescriptorBase;
class OutOfMemoryError;
struct PipelineDescriptorBase;
struct CanvasConfiguration;
struct PresentationContextDescriptor;
struct ProgrammableStage;
struct RenderBundleEncoderDescriptor;
struct RenderPassLayout;
struct RenderPipelineDescriptor;
struct RequestAdapterOptions;
struct ShaderModuleCompilationHint;
struct ShaderModuleDescriptor;
class SupportedFeatures;
class SupportedLimits;
class ValidationError;
struct VertexBufferLayout;
struct VertexState;
class XRBinding;
class XRProjectionLayer;
class XRSubImage;
class XRView;

} // namespace WebCore::WebGPU

namespace WebKit::WebGPU {

struct BindGroupDescriptor;
struct BindGroupEntry;
struct BindGroupLayoutDescriptor;
struct BindGroupLayoutEntry;
struct BlendComponent;
struct BlendState;
struct BufferBinding;
struct BufferBindingLayout;
struct CanvasConfiguration;
struct ColorTargetState;
struct ComputePassDescriptor;
struct ComputePipelineDescriptor;
struct DepthStencilState;
struct DeviceDescriptor;
struct ExternalTextureBindingLayout;
struct ExternalTextureDescriptor;
struct FragmentState;
struct Identifier;
struct ImageCopyBuffer;
struct ImageCopyExternalImage;
#if PLATFORM(COCOA) && ENABLE(VIDEO)
struct ImageCopyExternalImageVideoSource;
#endif
struct ImageCopyTexture;
struct ImageCopyTextureTagged;
struct ImageDataLayout;
struct InternalError;
struct MultisampleState;
struct ObjectDescriptorBase;
struct OutOfMemoryError;
struct PipelineDescriptorBase;
struct PipelineLayoutDescriptor;
struct CanvasConfiguration;
struct PresentationContextDescriptor;
struct PrimitiveState;
struct ProgrammableStage;
struct RenderBundleEncoderDescriptor;
struct RenderPassColorAttachment;
struct RenderPassDepthStencilAttachment;
struct RenderPassDescriptor;
struct RenderPassLayout;
struct RenderPipelineDescriptor;
struct RequestAdapterOptions;
struct SamplerBindingLayout;
struct ShaderModuleCompilationHint;
struct ShaderModuleDescriptor;
struct StencilFaceState;
struct StorageTextureBindingLayout;
struct SupportedFeatures;
struct SupportedLimits;
struct TextureBindingLayout;
struct TextureDescriptor;
struct ValidationError;
struct VertexAttribute;
struct VertexBufferLayout;
struct VertexState;

// Owns the arrays that a converted WebGPU::RenderPipelineDescriptor borrows.
struct RenderPipelineDescriptorStorage {
    Vector<::WebGPU::ConstantEntry> vertexConstants;
    Vector<Vector<::WebGPU::VertexAttribute>> vertexAttributes;
    Vector<std::optional<::WebGPU::VertexBufferLayout>> vertexBuffers;
    Vector<::WebGPU::ConstantEntry> fragmentConstants;
    Vector<std::optional<::WebGPU::ColorTargetState>> fragmentTargets;
};

class ConvertFromBackingContext {
public:
    virtual ~ConvertFromBackingContext() = default;

    std::optional<WebCore::WebGPU::BindGroupDescriptor> convertFromBacking(const BindGroupDescriptor&, Vector<WebCore::WebGPU::BindGroupEntry>& entriesStorage);
    std::optional<WebCore::WebGPU::BindGroupEntry> convertFromBacking(const BindGroupEntry&);
    std::optional<WebCore::WebGPU::BindGroupLayoutDescriptor> convertFromBacking(const BindGroupLayoutDescriptor&, Vector<WebCore::WebGPU::BindGroupLayoutEntry>& entriesStorage);
    std::optional<WebCore::WebGPU::BindGroupLayoutEntry> convertFromBacking(const BindGroupLayoutEntry&);
    std::optional<WebCore::WebGPU::BlendComponent> NODELETE convertFromBacking(const BlendComponent&);
    std::optional<WebCore::WebGPU::BlendState> NODELETE convertFromBacking(const BlendState&);
    std::optional<WebCore::WebGPU::BufferBinding> convertFromBacking(const BufferBinding&);
    std::optional<WebCore::WebGPU::BufferBindingLayout> NODELETE convertFromBacking(const BufferBindingLayout&);
    std::optional<::WebGPU::CanvasConfiguration> convertFromBacking(const CanvasConfiguration&);
    std::optional<::WebGPU::ColorTargetState> convertFromBacking(const ColorTargetState&);
    std::optional<WebCore::WebGPU::ComputePassDescriptor> convertFromBacking(const ComputePassDescriptor&);
    std::optional<::WebGPU::ComputePipelineDescriptor> convertFromBacking(const ComputePipelineDescriptor&, Vector<::WebGPU::ConstantEntry>& constantsStorage, bool allowMissingPipelineLayout = false);
    std::optional<::WebGPU::DepthStencilState> convertFromBacking(const DepthStencilState&);
    std::optional<::WebGPU::DeviceDescriptor> convertFromBacking(const DeviceDescriptor&);
    std::optional<WebCore::WebGPU::Error> convertFromBacking(const Error&);
    std::optional<WebCore::WebGPU::ExternalTextureBindingLayout> NODELETE convertFromBacking(const ExternalTextureBindingLayout&);
#if ENABLE(VIDEO) && PLATFORM(COCOA)
    using PixelBufferType = RetainPtr<CVPixelBufferRef>;
#else
    using PixelBufferType = void*;
#endif
    std::optional<WebCore::WebGPU::ExternalTextureDescriptor> convertFromBacking(const ExternalTextureDescriptor&, PixelBufferType);
    std::optional<::WebGPU::FragmentState> convertFromBacking(const FragmentState&, RenderPipelineDescriptorStorage&);
    std::optional<WebCore::WebGPU::Identifier> convertFromBacking(const Identifier&);
    std::optional<WebCore::WebGPU::ImageCopyBuffer> convertFromBacking(const ImageCopyBuffer&);
    std::optional<WebCore::WebGPU::ImageCopyExternalImage> convertFromBacking(const ImageCopyExternalImage&);
#if PLATFORM(COCOA) && ENABLE(VIDEO)
    std::optional<WebCore::WebGPU::ImageCopyExternalImage> convertFromBacking(const ImageCopyExternalImageVideoSource&, PixelBufferType, WebCore::VideoFrameRotation, bool isMirrored);
#endif
    std::optional<WebCore::WebGPU::ImageCopyTexture> convertFromBacking(const ImageCopyTexture&);
    std::optional<WebCore::WebGPU::ImageCopyTextureTagged> convertFromBacking(const ImageCopyTextureTagged&);
    std::optional<WebCore::WebGPU::ImageDataLayout> NODELETE convertFromBacking(const ImageDataLayout&);
    RefPtr<WebCore::WebGPU::InternalError> convertFromBacking(const InternalError&);
    std::optional<WebCore::WebGPU::MultisampleState> NODELETE convertFromBacking(const MultisampleState&);
    std::optional<WebCore::WebGPU::ObjectDescriptorBase> NODELETE convertFromBacking(const ObjectDescriptorBase&);
    RefPtr<WebCore::WebGPU::OutOfMemoryError> convertFromBacking(const OutOfMemoryError&);
    // std::nullopt when the layout does not convert; nullptr when it is missing and allowed to be.
    std::optional<RefPtr<WebCore::WebGPU::PipelineLayout>> convertLayoutFromBacking(const PipelineDescriptorBase&, bool allowMissingPipelineLayout);
    std::optional<WebCore::WebGPU::PipelineLayoutDescriptor> convertFromBacking(const PipelineLayoutDescriptor&, Vector<Ref<WebCore::WebGPU::BindGroupLayout>>& bindGroupLayoutsStorage);
    std::optional<WebCore::WebGPU::PresentationContextDescriptor> convertFromBacking(const PresentationContextDescriptor&);
    std::optional<WebCore::WebGPU::PrimitiveState> NODELETE convertFromBacking(const PrimitiveState&);
    std::optional<::WebGPU::ProgrammableStage> convertFromBacking(const ProgrammableStage&, Vector<::WebGPU::ConstantEntry>& constantsStorage);
    std::optional<::WebGPU::RenderBundleEncoderDescriptor> convertFromBacking(const RenderBundleEncoderDescriptor&);
    std::optional<WebCore::WebGPU::RenderPassColorAttachment> convertFromBacking(const RenderPassColorAttachment&);
    std::optional<WebCore::WebGPU::RenderPassDepthStencilAttachment> convertFromBacking(const RenderPassDepthStencilAttachment&);
    std::optional<WebCore::WebGPU::RenderPassDescriptor> convertFromBacking(const RenderPassDescriptor&, Vector<std::optional<WebCore::WebGPU::RenderPassColorAttachment>>& colorAttachmentsStorage);
    std::optional<WebCore::WebGPU::RenderPassLayout> convertFromBacking(const RenderPassLayout&);
    std::optional<WebCore::WebGPU::RenderPassTimestampWrites> convertFromBacking(const RenderPassTimestampWrites&);
    std::optional<::WebGPU::RenderPipelineDescriptor> convertFromBacking(const RenderPipelineDescriptor&, RenderPipelineDescriptorStorage&, bool allowMissingPipelineLayout = false);
    std::optional<WebCore::WebGPU::RequestAdapterOptions> NODELETE convertFromBacking(const RequestAdapterOptions&);
    std::optional<WebCore::WebGPU::SamplerBindingLayout> NODELETE convertFromBacking(const SamplerBindingLayout&);
    std::optional<::WebGPU::ShaderModuleDescriptor> convertFromBacking(const ShaderModuleDescriptor&, Vector<::WebGPU::ShaderModuleCompilationHint>& hintsStorage);
    std::optional<WebCore::WebGPU::StencilFaceState> NODELETE convertFromBacking(const StencilFaceState&);
    std::optional<WebCore::WebGPU::StorageTextureBindingLayout> NODELETE convertFromBacking(const StorageTextureBindingLayout&);
    RefPtr<WebCore::WebGPU::SupportedFeatures> convertFromBacking(const SupportedFeatures&);
    std::optional<WebCore::WebGPU::TextureBindingLayout> NODELETE convertFromBacking(const TextureBindingLayout&);
    std::optional<WebCore::WebGPU::TextureDescriptor> convertFromBacking(const TextureDescriptor&);
    RefPtr<WebCore::WebGPU::ValidationError> convertFromBacking(const ValidationError&);
    std::optional<WebCore::WebGPU::VertexAttribute> NODELETE convertFromBacking(const VertexAttribute&);
    std::optional<::WebGPU::VertexBufferLayout> convertFromBacking(const VertexBufferLayout&, Vector<::WebGPU::VertexAttribute>& attributesStorage);
    std::optional<::WebGPU::VertexState> convertFromBacking(const VertexState&, RenderPipelineDescriptorStorage&);

    virtual RefPtr<WebCore::WebGPU::Adapter> convertAdapterFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::BindGroup> convertBindGroupFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::BindGroupLayout> convertBindGroupLayoutFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::Buffer> convertBufferFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::CommandBuffer> convertCommandBufferFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::CommandEncoder> convertCommandEncoderFromBacking(WebGPUIdentifier) = 0;
    virtual WeakPtr<WebCore::WebGPU::CompositorIntegration> convertCompositorIntegrationFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::ComputePassEncoder> convertComputePassEncoderFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::ComputePipeline> convertComputePipelineFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::Device> convertDeviceFromBacking(WebGPUIdentifier) = 0;
    virtual ThreadSafeWeakPtr<WebCore::WebGPU::ExternalTexture> convertExternalTextureFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::PipelineLayout> convertPipelineLayoutFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::QuerySet> convertQuerySetFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::Queue> convertQueueFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::RenderBundleEncoder> convertRenderBundleEncoderFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::RenderBundle> convertRenderBundleFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::RenderPassEncoder> convertRenderPassEncoderFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::RenderPipeline> convertRenderPipelineFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::Sampler> convertSamplerFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::ShaderModule> convertShaderModuleFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::PresentationContext> convertPresentationContextFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::Texture> convertTextureFromBacking(WebGPUIdentifier) = 0;
    virtual RefPtr<WebCore::WebGPU::TextureView> convertTextureViewFromBacking(WebGPUIdentifier) = 0;
    virtual WeakPtr<WebCore::WebGPU::XRBinding> convertXRBindingFromBacking(WebGPUIdentifier) = 0;
    virtual WeakPtr<WebCore::WebGPU::XRProjectionLayer> convertXRProjectionLayerFromBacking(WebGPUIdentifier) = 0;
    virtual WeakPtr<WebCore::WebGPU::XRSubImage> convertXRSubImageFromBacking(WebGPUIdentifier) = 0;
    virtual WeakPtr<WebCore::WebGPU::XRView> createXRViewFromBacking(WebGPUIdentifier) = 0;
};

} // namespace WebKit::WebGPU

#endif // ENABLE(GPU_PROCESS)
