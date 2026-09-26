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

#include "config.h"
#include "WebGPUImpl.h"

#if HAVE(WEBGPU_IMPLEMENTATION)

#include "WebGPUCompositorIntegrationImpl.h"
#include "WebGPUExternalTextureDescriptor.h"
#include "WebGPUImageCopyExternalImage.h"
#include "WebGPUImageCopyTextureTagged.h"
#include "WebGPUPresentationContext.h"
#include "WebGPUPresentationContextDescriptor.h"
#include "WebGPUXRBinding.h"
#include "WebGPUXRProjectionLayer.h"
#include "WebGPUXRSubImage.h"
#include "WebGPUXRView.h"
#include <WebCore/ColorSpace.h>
#include <WebCore/GraphicsContext.h>
#include <WebCore/IOSurface.h>
#include <WebCore/ImageBuffer.h>
#include <WebCore/IntSize.h>
#include <WebCore/NativeImage.h>
#include <WebGPU/WebGPUCppBridge.h>
#include <WebGPU/WebGPUExt.h>
#include <wtf/BlockPtr.h>
#include <wtf/TZoneMallocInlines.h>

namespace WebCore::WebGPU {

WTF_MAKE_TZONE_ALLOCATED_IMPL(GPUImpl);

GPUImpl::GPUImpl(Ref<::WebGPU::Instance>&& instance)
    : m_backing(WTF::move(instance))
{
}

GPUImpl::~GPUImpl() = default;

void GPUImpl::requestAdapter(const RequestAdapterOptions& options, CompletionHandler<void(RefPtr<Adapter>&&)>&& callback)
{
    auto backingOptions = options;
#if CPU(X86_64)
    backingOptions.powerPreference = PowerPreference::HighPerformance;
#endif
    m_backing->requestAdapter(backingOptions, WTF::move(callback));
}

RefPtr<PresentationContext> GPUImpl::createPresentationContext(const PresentationContextDescriptor& presentationContextDescriptor)
{
    // Every WebCore::WebGPU::CompositorIntegration that GPUImpl creates is a CompositorIntegrationImpl.
    Ref compositorIntegration = static_cast<CompositorIntegrationImpl&>(presentationContextDescriptor.compositorIntegration.get());

    RefPtr result = m_backing->createPresentationContext({
        .registerCompositorIntegration = [&](auto&& renderBuffersWereRecreated, auto&& onSubmittedWorkScheduled) {
            compositorIntegration->registerCallbacks(WTF::move(renderBuffersWereRecreated), WTF::move(onSubmittedWorkScheduled));
        },
    });
    if (result)
        compositorIntegration->setPresentationContext(*result);
    return result;
}

RefPtr<CompositorIntegration> GPUImpl::createCompositorIntegration()
{
    return CompositorIntegrationImpl::create();
}

#if ENABLE(VIDEO)
static ::WebGPU::VideoFrameRotation NODELETE convertToAPI(VideoFrameRotation rotation)
{
    switch (rotation) {
    case VideoFrameRotation::None:
        return ::WebGPU::VideoFrameRotation::None;
    case VideoFrameRotation::Right:
        return ::WebGPU::VideoFrameRotation::Right;
    case VideoFrameRotation::UpsideDown:
        return ::WebGPU::VideoFrameRotation::UpsideDown;
    case VideoFrameRotation::Left:
        return ::WebGPU::VideoFrameRotation::Left;
    }

    ASSERT_NOT_REACHED();
    return ::WebGPU::VideoFrameRotation::None;
}
#endif

// The IOSurface format an accelerated ImageBuffer of this pixel format is backed by, expressed as the
// equivalent texture format, plus whether its alpha channel holds meaningful data. std::nullopt for
// the formats GPUQueue::copyExternalImageToTexture keeps on the CPU readback path.
struct SourceTextureFormat {
    ::WebGPU::TextureFormat format;
    bool hasAlpha;
};

static std::optional<SourceTextureFormat> NODELETE sourceTextureFormat(PixelFormat pixelFormat)
{
    switch (pixelFormat) {
    case PixelFormat::RGBA8:
        return SourceTextureFormat { ::WebGPU::TextureFormat::Rgba8unorm, true };
    case PixelFormat::BGRA8:
        return SourceTextureFormat { ::WebGPU::TextureFormat::Bgra8unorm, true };
    case PixelFormat::BGRX8:
        // IOSurface::Format::BGRX uses the same IOSurface pixel format as BGRA, but CoreGraphics
        // renders into it with kCGImageAlphaNoneSkipFirst, so the alpha byte is undefined.
        return SourceTextureFormat { ::WebGPU::TextureFormat::Bgra8unorm, false };
    case PixelFormat::RGBX8:
        return SourceTextureFormat { ::WebGPU::TextureFormat::Rgba8unorm, false };
#if ENABLE(PIXEL_FORMAT_RGBA16F)
    case PixelFormat::RGBA16F:
        return SourceTextureFormat { ::WebGPU::TextureFormat::Rgba16float, true };
#endif
#if ENABLE(PIXEL_FORMAT_RGBA16)
    case PixelFormat::RGBA16:
        return SourceTextureFormat { ::WebGPU::TextureFormat::Rgba16unorm, true };
#endif
#if ENABLE(PIXEL_FORMAT_RGB10)
    case PixelFormat::RGB10:
#endif
#if ENABLE(PIXEL_FORMAT_RGB10A8)
    case PixelFormat::RGB10A8:
#endif
#if ENABLE(PIXEL_FORMAT_RGB10) || ENABLE(PIXEL_FORMAT_RGB10A8)
        // Packed 10-bit surfaces have no single-plane MTLPixelFormat equivalent.
        return std::nullopt;
#endif
    }

    ASSERT_NOT_REACHED();
    return std::nullopt;
}

void GPUImpl::copyExternalImageToTexture(Queue& queue, const ImageCopyExternalImage& source, const ImageCopyTextureTagged& destination, const Extent3D& copySize)
{
    ::WebGPU::ImageCopyExternalImage backingSource {
        .origin = source.origin.value_or(Origin2D { }),
        .flipY = source.flipY,
        .hasAlpha = false,
        // An ImageBitmap created with premultiplyAlpha: "none" was put into its buffer straight, so
        // the caller has to say; a buffer a 2D context composited is premultiplied.
        .premultipliedAlpha = source.premultipliedAlpha,
    };

#if ENABLE(VIDEO)
    if (source.videoSource) {
        // The decoded frame the GPU process resolved for us. Its extent, its crop and its primaries
        // all travel with the frame, so the backing queue reads them off it rather than being told
        // here; and a decoded frame is opaque, so its alpha is replaced with 1 the way an external
        // texture's is.
        auto* pixelBuffer = std::get_if<RetainPtr<CVPixelBufferRef>>(&*source.videoSource);
        if (!pixelBuffer || !*pixelBuffer)
            return;

        backingSource.pixelBuffer = *pixelBuffer;
        // The display transform is the one thing about the frame its pixel buffer does not carry.
        backingSource.pixelBufferRotation = convertToAPI(source.videoSourceRotation);
        backingSource.pixelBufferIsMirrored = source.videoSourceIsMirrored;
        backingSource.premultipliedAlpha = true;
    } else
#endif
    {
        RefPtr sourceImageBuffer = source.imageBuffer;
        if (!sourceImageBuffer)
            return;

        // Only accelerated ImageBuffers have an IOSurface to wrap in an MTLTexture. GPUQueue rejects
        // unaccelerated sources before we get here, but the backing may have been dropped since.
        auto* surface = sourceImageBuffer->surface();
        if (!surface)
            return;

        auto sourceSize = sourceImageBuffer->truncatedLogicalSize();
        if (!sourceSize.width() || !sourceSize.height())
            return;

        auto sourceFormat = sourceTextureFormat(sourceImageBuffer->pixelFormat());
        if (!sourceFormat)
            return;

        backingSource.source = surface->surface();
        backingSource.sourceFormat = sourceFormat->format;
        backingSource.sourceSize = { static_cast<uint32_t>(sourceSize.width()), static_cast<uint32_t>(sourceSize.height()) };
        backingSource.hasAlpha = sourceFormat->hasAlpha;
        backingSource.colorSpace = sourceImageBuffer->colorSpace() == ColorSpace::DisplayP3() ? ::WebGPU::PredefinedColorSpace::DisplayP3 : ::WebGPU::PredefinedColorSpace::SRGB;
    }

    queue.copyExternalImageToTexture(backingSource, {
        .texture = destination.texture,
        .mipLevel = destination.mipLevel,
        .origin = destination.origin,
        .aspect = destination.aspect,
        .colorSpace = convertToAPI(destination.colorSpace),
        .premultipliedAlpha = destination.premultipliedAlpha,
    }, copySize);
}

RefPtr<WebCore::NativeImage> GPUImpl::nativeImage(Queue&, WebCore::VideoFrame&)
{
    // Only RemoteGPUProxy resolves a video frame to an image, through its video frame object heap.
    RELEASE_ASSERT_NOT_REACHED();
}

RefPtr<ExternalTexture> GPUImpl::importExternalTexture(Device& device, const ExternalTextureDescriptor& descriptor)
{
    auto* pixelBuffer = std::get_if<RetainPtr<CVPixelBufferRef>>(&descriptor.videoBacking);
    return device.importExternalTexture({
        .label = descriptor.label,
        .pixelBuffer = pixelBuffer ? *pixelBuffer : nullptr,
        .colorSpace = convertToAPI(descriptor.colorSpace),
        .visibleSize = {
            .width = static_cast<uint32_t>(std::max(0, descriptor.visibleSize.width())),
            .height = static_cast<uint32_t>(std::max(0, descriptor.visibleSize.height())),
        },
    });
}

#if PLATFORM(COCOA) && ENABLE(VIDEO)
void GPUImpl::updateExternalTexture(Device&, const ExternalTexture&, const WebCore::MediaPlayerIdentifier&)
{
    // Only RemoteGPUProxy names a media player; the GPU process resolves it to a pixel buffer.
    RELEASE_ASSERT_NOT_REACHED();
}
#endif

void GPUImpl::paintToCanvas(WebCore::NativeImage& image, const WebCore::IntSize& canvasSize, WebCore::GraphicsContext& context)
{
    auto imageSize = image.size();
    FloatRect canvasRect(FloatPoint(), canvasSize);
    GraphicsContextStateSaver stateSaver(context);
    context.setImageInterpolationQuality(InterpolationQuality::DoNotInterpolate);
    context.drawNativeImage(image, canvasRect, FloatRect(FloatPoint(), imageSize), { CompositeOperator::Copy });
}

bool GPUImpl::isValid(const CompositorIntegration&) const
{
    return true;
}

bool GPUImpl::isValid(const Buffer& buffer) const
{
    return buffer.isValid();
}

bool GPUImpl::isValid(const Adapter& adapter) const
{
    return adapter.isValid();
}

bool GPUImpl::isValid(const BindGroup& bindGroup) const
{
    return bindGroup.isValid();
}

bool GPUImpl::isValid(const BindGroupLayout& bindGroupLayout) const
{
    return bindGroupLayout.isValid();
}

bool GPUImpl::isValid(const CommandBuffer& commandBuffer) const
{
    return commandBuffer.isValid();
}

bool GPUImpl::isValid(const CommandEncoder& commandEncoder) const
{
    return commandEncoder.isValid();
}

bool GPUImpl::isValid(const ComputePassEncoder& computePassEncoder) const
{
    return computePassEncoder.isValid();
}

bool GPUImpl::isValid(const ComputePipeline& computePipeline) const
{
    return computePipeline.isValid();
}

bool GPUImpl::isValid(const Device& device) const
{
    return device.isValid();
}

bool GPUImpl::isValid(const ExternalTexture& externalTexture) const
{
    return externalTexture.isValid();
}

bool GPUImpl::isValid(const PipelineLayout& pipelineLayout) const
{
    return pipelineLayout.isValid();
}

bool GPUImpl::isValid(const PresentationContext& presentationContext) const
{
    return presentationContext.isValid();
}

bool GPUImpl::isValid(const QuerySet& querySet) const
{
    return querySet.isValid();
}

bool GPUImpl::isValid(const Queue& queue) const
{
    return queue.isValid();
}

bool GPUImpl::isValid(const RenderBundleEncoder& renderBundleEncoder) const
{
    return renderBundleEncoder.isValid();
}

bool GPUImpl::isValid(const RenderBundle& renderBundle) const
{
    return renderBundle.isValid();
}

bool GPUImpl::isValid(const RenderPassEncoder& renderPassEncoder) const
{
    return renderPassEncoder.isValid();
}

bool GPUImpl::isValid(const RenderPipeline& renderPipeline) const
{
    return renderPipeline.isValid();
}

bool GPUImpl::isValid(const Sampler& sampler) const
{
    return sampler.isValid();
}

bool GPUImpl::isValid(const ShaderModule& shaderModule) const
{
    return shaderModule.isValid();
}

bool GPUImpl::isValid(const Texture& texture) const
{
    return texture.isValid();
}

bool GPUImpl::isValid(const TextureView& textureView) const
{
    return textureView.isValid();
}

bool GPUImpl::isValid(const XRBinding& binding) const
{
    return binding.isValid();
}

bool GPUImpl::isValid(const XRSubImage& subImage) const
{
    return subImage.isValid();
}

bool GPUImpl::isValid(const XRProjectionLayer& layer) const
{
    return layer.isValid();
}

bool GPUImpl::isValid(const XRView& view) const
{
    return view.isValid();
}

} // namespace WebCore::WebGPU

#endif // HAVE(WEBGPU_IMPLEMENTATION)
