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
#include "WebGPUCompilationMessage.h"
#include "WebGPUComputePassTimestampWrites.h"
#include "WebGPUError.h"
#include "WebGPUExtent3D.h"
#include "WebGPUIdentifier.h"
#include "WebGPUOrigin2D.h"
#include "WebGPUOrigin3D.h"
#include "WebGPURenderPassTimestampWrites.h"
#include "WebGPUTextureViewDescriptor.h"
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
#include <WebCore/WebGPUComputePassTimestampWrites.h>
#include <WebCore/WebGPUComputePipeline.h>
#include <WebCore/WebGPUError.h>
#include <WebCore/WebGPUExtent3D.h>
#include <WebCore/WebGPUExternalTexture.h>
#include <WebCore/WebGPUExternalTextureBindingLayout.h>
#include <WebCore/WebGPUImageDataLayout.h>
#include <WebCore/WebGPUMultisampleState.h>
#include <WebCore/WebGPUOrigin2D.h>
#include <WebCore/WebGPUOrigin3D.h>
#include <WebCore/WebGPUPipelineLayout.h>
#include <WebCore/WebGPUPipelineLayoutDescriptor.h>
#include <WebCore/WebGPUPrimitiveState.h>
#include <WebCore/WebGPUQuerySet.h>
#include <WebCore/WebGPURenderBundle.h>
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
#include <wtf/RefCounted.h>

namespace WebCore::WebGPU {

class Adapter;
struct CanvasConfiguration;
struct ColorTargetState;
class CommandEncoder;
struct CommandEncoderDescriptor;
class CompositorIntegration;
struct ComputePassDescriptor;
class ComputePassEncoder;
struct ComputePipelineDescriptor;
struct DepthStencilState;
class Device;
struct DeviceDescriptor;
struct ExternalTextureDescriptor;
struct FragmentState;
class GPU;
struct Identifier;
struct ImageCopyBuffer;
struct ImageCopyExternalImage;
struct ImageCopyTexture;
struct ImageCopyTextureTagged;
class InternalError;
struct ObjectDescriptorBase;
class OutOfMemoryError;
struct PipelineDescriptorBase;
struct CanvasConfiguration;
class PresentationContext;
struct PresentationContextDescriptor;
struct ProgrammableStage;
class Queue;
class RenderBundleEncoder;
struct RenderBundleEncoderDescriptor;
struct RenderPassColorAttachment;
struct RenderPassDepthStencilAttachment;
struct RenderPassDescriptor;
class RenderPassEncoder;
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
struct CommandEncoderDescriptor;
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
struct PresentationContextDescriptor;
struct CanvasConfiguration;
struct PrimitiveState;
struct ProgrammableStage;
class RemoteCompositorIntegrationProxy;
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

class ConvertToBackingContext : public RefCounted<ConvertToBackingContext> {
public:
    virtual ~ConvertToBackingContext() = default;

    std::optional<BindGroupDescriptor> convertToBacking(const WebCore::WebGPU::BindGroupDescriptor&);
    std::optional<BindGroupEntry> convertToBacking(const WebCore::WebGPU::BindGroupEntry&);
    std::optional<BindGroupLayoutDescriptor> convertToBacking(const WebCore::WebGPU::BindGroupLayoutDescriptor&);
    std::optional<BindGroupLayoutEntry> convertToBacking(const WebCore::WebGPU::BindGroupLayoutEntry&);
    std::optional<BlendComponent> NODELETE convertToBacking(const WebCore::WebGPU::BlendComponent&);
    std::optional<BlendState> NODELETE convertToBacking(const WebCore::WebGPU::BlendState&);
    std::optional<BufferBinding> convertToBacking(const WebCore::WebGPU::BufferBinding&);
    std::optional<BufferBindingLayout> NODELETE convertToBacking(const WebCore::WebGPU::BufferBindingLayout&);
    std::optional<CanvasConfiguration> convertToBacking(const WebCore::WebGPU::CanvasConfiguration&);
    std::optional<ColorTargetState> convertToBacking(const WebCore::WebGPU::ColorTargetState&);
    std::optional<CommandEncoderDescriptor> convertToBacking(const WebCore::WebGPU::CommandEncoderDescriptor&);
    std::optional<ComputePassDescriptor> convertToBacking(const WebCore::WebGPU::ComputePassDescriptor&);
    std::optional<ComputePassTimestampWrites> convertToBacking(const WebCore::WebGPU::ComputePassTimestampWrites&);
    std::optional<ComputePipelineDescriptor> convertToBacking(const WebCore::WebGPU::ComputePipelineDescriptor&);
    std::optional<DepthStencilState> convertToBacking(const WebCore::WebGPU::DepthStencilState&);
    std::optional<DeviceDescriptor> convertToBacking(const WebCore::WebGPU::DeviceDescriptor&);
    std::optional<Error> convertToBacking(const WebCore::WebGPU::Error&);
    std::optional<ExternalTextureBindingLayout> NODELETE convertToBacking(const WebCore::WebGPU::ExternalTextureBindingLayout&);
    std::optional<ExternalTextureDescriptor> convertToBacking(const WebCore::WebGPU::ExternalTextureDescriptor&);
    std::optional<FragmentState> convertToBacking(const WebCore::WebGPU::FragmentState&);
    std::optional<Identifier> convertToBacking(const WebCore::WebGPU::Identifier&);
    std::optional<ImageCopyBuffer> convertToBacking(const WebCore::WebGPU::ImageCopyBuffer&);
    std::optional<ImageCopyExternalImage> convertToBacking(const WebCore::WebGPU::ImageCopyExternalImage&);
#if PLATFORM(COCOA) && ENABLE(VIDEO)
    std::optional<ImageCopyExternalImageVideoSource> convertToBackingVideoSource(const WebCore::WebGPU::ImageCopyExternalImage&);
#endif
    std::optional<ImageCopyTexture> convertToBacking(const WebCore::WebGPU::ImageCopyTexture&);
    std::optional<ImageCopyTextureTagged> convertToBacking(const WebCore::WebGPU::ImageCopyTextureTagged&);
    std::optional<ImageDataLayout> NODELETE convertToBacking(const WebCore::WebGPU::ImageDataLayout&);
    std::optional<InternalError> NODELETE convertToBacking(const WebCore::WebGPU::InternalError&);
    std::optional<MultisampleState> NODELETE convertToBacking(const WebCore::WebGPU::MultisampleState&);
    std::optional<ObjectDescriptorBase> NODELETE convertToBacking(const WebCore::WebGPU::ObjectDescriptorBase&);
    std::optional<OutOfMemoryError> NODELETE convertToBacking(const WebCore::WebGPU::OutOfMemoryError&);
    std::optional<PipelineDescriptorBase> convertToBacking(const WebCore::WebGPU::PipelineDescriptorBase&);
    std::optional<PipelineLayoutDescriptor> convertToBacking(const WebCore::WebGPU::PipelineLayoutDescriptor&);
    std::optional<PresentationContextDescriptor> convertToBacking(const WebCore::WebGPU::PresentationContextDescriptor&);
    std::optional<PrimitiveState> NODELETE convertToBacking(const WebCore::WebGPU::PrimitiveState&);
    std::optional<ProgrammableStage> convertToBacking(const WebCore::WebGPU::ProgrammableStage&);
    std::optional<RenderBundleEncoderDescriptor> convertToBacking(const WebCore::WebGPU::RenderBundleEncoderDescriptor&);
    std::optional<RenderPassColorAttachment> convertToBacking(const WebCore::WebGPU::RenderPassColorAttachment&);
    std::optional<RenderPassDepthStencilAttachment> convertToBacking(const WebCore::WebGPU::RenderPassDepthStencilAttachment&);
    std::optional<RenderPassDescriptor> convertToBacking(const WebCore::WebGPU::RenderPassDescriptor&);
    std::optional<RenderPassLayout> convertToBacking(const WebCore::WebGPU::RenderPassLayout&);
    std::optional<RenderPassTimestampWrites> convertToBacking(const WebCore::WebGPU::RenderPassTimestampWrites&);
    std::optional<RenderPipelineDescriptor> convertToBacking(const WebCore::WebGPU::RenderPipelineDescriptor&);
    std::optional<RequestAdapterOptions> NODELETE convertToBacking(const WebCore::WebGPU::RequestAdapterOptions&);
    std::optional<SamplerBindingLayout> NODELETE convertToBacking(const WebCore::WebGPU::SamplerBindingLayout&);
    std::optional<ShaderModuleCompilationHint> convertToBacking(const WebCore::WebGPU::ShaderModuleCompilationHint&);
    std::optional<ShaderModuleDescriptor> convertToBacking(const WebCore::WebGPU::ShaderModuleDescriptor&);
    std::optional<StencilFaceState> NODELETE convertToBacking(const WebCore::WebGPU::StencilFaceState&);
    std::optional<StorageTextureBindingLayout> NODELETE convertToBacking(const WebCore::WebGPU::StorageTextureBindingLayout&);
    std::optional<SupportedFeatures> convertToBacking(const WebCore::WebGPU::SupportedFeatures&);
    std::optional<SupportedLimits> NODELETE convertToBacking(const WebCore::WebGPU::SupportedLimits&);
    std::optional<TextureBindingLayout> NODELETE convertToBacking(const WebCore::WebGPU::TextureBindingLayout&);
    std::optional<TextureDescriptor> convertToBacking(const WebCore::WebGPU::TextureDescriptor&);
    std::optional<ValidationError> NODELETE convertToBacking(const WebCore::WebGPU::ValidationError&);
    std::optional<VertexAttribute> NODELETE convertToBacking(const WebCore::WebGPU::VertexAttribute&);
    std::optional<VertexBufferLayout> convertToBacking(const WebCore::WebGPU::VertexBufferLayout&);
    std::optional<VertexState> convertToBacking(const WebCore::WebGPU::VertexState&);

    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::Adapter&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::BindGroup&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::BindGroupLayout&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::Buffer&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::CommandBuffer&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::CommandEncoder&) = 0;
    virtual const RemoteCompositorIntegrationProxy& convertToRawBacking(const WebCore::WebGPU::CompositorIntegration&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::CompositorIntegration&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::ComputePassEncoder&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::ComputePipeline&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::Device&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::ExternalTexture&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::GPU&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::PipelineLayout&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::PresentationContext&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::QuerySet&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::Queue&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::RenderBundleEncoder&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::RenderBundle&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::RenderPassEncoder&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::RenderPipeline&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::Sampler&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::ShaderModule&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::Texture&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::TextureView&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::XRBinding&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::XRProjectionLayer&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::XRSubImage&) = 0;
    virtual WebGPUIdentifier convertToBacking(const WebCore::WebGPU::XRView&) = 0;
};

} // namespace WebKit::WebGPU

#endif // ENABLE(GPU_PROCESS)
