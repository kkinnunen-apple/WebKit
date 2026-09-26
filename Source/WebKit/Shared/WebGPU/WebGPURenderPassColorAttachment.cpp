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
#include "WebGPURenderPassColorAttachment.h"

#if ENABLE(GPU_PROCESS)

#include "WebGPUConvertFromBackingContext.h"
#include "WebGPUConvertToBackingContext.h"
#include <WebCore/WebGPURenderPassColorAttachment.h>
#include <WebCore/WebGPUTextureView.h>

namespace WebKit::WebGPU {

static WebGPUIdentifier colorAttachmentViewIdentifier(ConvertToBackingContext& convertToBacking, const WebCore::WebGPU::RenderPassColorAttachmentView& view)
{
    return WTF::switchOn(view, [&](const auto& textureOrView) {
        return convertToBacking.convertToBacking(textureOrView.get());
    });
}
std::optional<RenderPassColorAttachment> ConvertToBackingContext::convertToBacking(const WebCore::WebGPU::RenderPassColorAttachment& renderPassColorAttachment)
{
    auto identifier = colorAttachmentViewIdentifier(*this, renderPassColorAttachment.view);

    std::optional<WebGPUIdentifier> resolveTarget;
    if (renderPassColorAttachment.resolveTarget)
        resolveTarget = colorAttachmentViewIdentifier(*this, *renderPassColorAttachment.resolveTarget);

    return { { identifier, renderPassColorAttachment.depthSlice, resolveTarget, renderPassColorAttachment.clearValue, renderPassColorAttachment.loadOp, renderPassColorAttachment.storeOp } };
}

std::optional<WebCore::WebGPU::RenderPassColorAttachment> ConvertFromBackingContext::convertFromBacking(const RenderPassColorAttachment& renderPassColorAttachment)
{
    RefPtr view = convertTextureViewFromBacking(renderPassColorAttachment.view);
    RefPtr texture = !view ? convertTextureFromBacking(renderPassColorAttachment.view) : nullptr;
    if (!view && !texture)
        return std::nullopt;

    std::optional<WebCore::WebGPU::RenderPassResolveAttachmentView> resolveTarget;
    if (renderPassColorAttachment.resolveTarget) {
        RefPtr view = convertTextureViewFromBacking(renderPassColorAttachment.resolveTarget.value());
        if (!view) {
            RefPtr texture = convertTextureFromBacking(renderPassColorAttachment.resolveTarget.value());
            if (!texture)
                return std::nullopt;

            resolveTarget = WebCore::WebGPU::RenderPassResolveAttachmentView { texture.releaseNonNull() };
        } else
            resolveTarget = WebCore::WebGPU::RenderPassResolveAttachmentView { view.releaseNonNull() };
    }

    WebCore::WebGPU::RenderPassColorAttachmentView viewTextureVariant = [&] -> WebCore::WebGPU::RenderPassColorAttachmentView {
        if (view)
            return view.releaseNonNull();
        return texture.releaseNonNull();
    }();

    return { { WTF::move(viewTextureVariant), renderPassColorAttachment.depthSlice, WTF::move(resolveTarget), renderPassColorAttachment.clearValue.value_or(WebCore::WebGPU::Color { }), renderPassColorAttachment.loadOp, renderPassColorAttachment.storeOp } };
}

} // namespace WebKit

#endif // ENABLE(GPU_PROCESS)
