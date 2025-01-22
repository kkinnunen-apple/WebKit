/*
 * Copyright (C) 2025 Apple Inc.  All rights reserved.
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

#include "config.h"

#include "NativeImageImage.h"
#include "GraphicsContext.h"

namespace WebCore {

Ref<Image> Image::create(Ref<NativeImage>&& image, ImageOrientation::Orientation orientation)
{
    return NativeImageImage::create(WTFMove(image), orientation);
}    

Ref<NativeImageImage> NativeImageImage::create(Ref<NativeImage>&& image, ImageOrientation::Orientation orientation)
{
    return adoptRef(*new NativeImageImage(WTFMove(image), orientation));
}    

NativeImageImage::NativeImageImage(Ref<NativeImage>&& nativeImage, ImageOrientation::Orientation orientation)
    : m_frame(WTFMove(nativeImage))
{
    if (orientation == ImageOrientation::Orientation::FromImage)
        orientation = ImageOrientation::Orientation::None;
    m_frame.setOrientation(orientation);

}

FloatSize NativeImageImage::size(ImageOrientation) const
{
    return m_frame.size();
}

bool NativeImageImage::currentFrameKnownToBeOpaque() const
{
    return !m_frame.hasAlpha();
}

RefPtr<NativeImage> NativeImageImage::currentNativeImage()
{
    return m_frame.nativeImage();
}

DestinationColorSpace NativeImageImage::colorSpace()
{
    return m_frame.nativeImage()->colorSpace();
}

ImageOrientation NativeImageImage::orientation() const
{
    return m_frame.orientation();
}

std::optional<Color> NativeImageImage::singlePixelSolidColor() const
{
    return m_frame.nativeImage()->singlePixelSolidColor();
}

ImageDrawResult NativeImageImage::draw(GraphicsContext& context, const FloatRect& destinationRect, const FloatRect& sourceRect, ImagePaintingOptions options)
{
    RefPtr nativeImage = m_frame.nativeImage(); 
    if (auto color = nativeImage->singlePixelSolidColor()) {
        fillWithSolidColor(context, destinationRect, *color, options.compositeOperator());
        return ImageDrawResult::DidDraw;
    }
    auto orientation = options.orientation();
    if (orientation == ImageOrientation::Orientation::FromImage)
        orientation = m_frame.orientation();
    auto headroom = options.headroom();
    if (headroom == Headroom::FromImage)
        headroom = m_frame.headroom();
    context.drawNativeImage(*m_frame.nativeImage(), destinationRect, sourceRect, { orientation, headroom });
    return ImageDrawResult::DidDraw;
}


}
