/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
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
#include "config.h"

#if ENABLE(WEBGL)
#include "WebGLDefaultFramebuffer.h"

namespace WebCore {
constexpr int antialiasMaxSampleCount = 4;
using GL = GraphicsContextGL;

static GCGLint drawingBufferTextureTarget()
{
    return GL::TEXTURE_2D; //FIXME.
}

std::unique_ptr<WebGLDefaultFramebuffer> WebGLDefaultFramebuffer::create(WebGLRenderingContextBase& context, const WebGLContextAttributes& attributes, IntSize size)
{
    auto instance = std::unique_ptr<WebGLDefaultFramebuffer> { new WebGLDefaultFramebuffer(context, attributes) };
    instance->reshape(size);
    return instance;
}

static GCGLenum depthStencilAttachamentPoint(GCGLenum internalDepthStencilFormat)
{
    // WebGL 1.0's rules state that combined depth/stencil renderbuffers
    // have to be attached to the synthetic DEPTH_STENCIL_ATTACHMENT point.
    switch (internalDepthStencilFormat) {
    case GL::DEPTH24_STENCIL8:
        return GL::DEPTH_STENCIL_ATTACHMENT;
    case GL::STENCIL_INDEX8:
        return GL::STENCIL_ATTACHMENT;
    }
    ASSERT(internalDepthStencilFormat == GL::DEPTH_COMPONENT24 || internalDepthStencilFormat == GL::DEPTH_COMPONENT16);
    return GL::DEPTH_ATTACHMENT;
}

WebGLDefaultFramebuffer::WebGLDefaultFramebuffer(WebGLRenderingContextBase& context, const WebGLContextAttributes& attributes)
    : m_context(context)
{
    if (!attributes.preserveDrawingBuffer) {
        m_unpreservedBuffers = GL::COLOR_BUFFER_BIT;
        if (attributes.stencil)
            m_unpreservedBuffers |= GL::STENCIL_BUFFER_BIT;
        if (attributes.depth)
            m_unpreservedBuffers |= GL::DEPTH_BUFFER_BIT;
    }

    if (attributes.antialias)
        m_msaaSampleCount = std::min(antialiasMaxSampleCount, m_context.maxSamples());
    auto* gl = context.graphicsContextGL();
    m_object = gl->createFramebuffer();
    if (m_msaaSampleCount || attributes.preserveDrawingBuffer)
        m_colorBuffer = gl->createRenderbuffer();
    else {
        m_colorBuffer = gl->createTexture();
        GCGLenum textureTarget = drawingBufferTextureTarget();
        gl->bindTexture(textureTarget, m_colorBuffer);
        gl->texParameteri(textureTarget, GL::TEXTURE_MAG_FILTER, GL::LINEAR);
        gl->texParameteri(textureTarget, GL::TEXTURE_MIN_FILTER, GL::LINEAR);
        gl->texParameteri(textureTarget, GL::TEXTURE_WRAP_S, GL::CLAMP_TO_EDGE);
        gl->texParameteri(textureTarget, GL::TEXTURE_WRAP_T, GL::CLAMP_TO_EDGE);
    }
    if (attributes.depth || attributes.stencil) {
        m_depthStencilBuffer = gl->createRenderbuffer();
        if (attributes.depth && attributes.stencil)
            m_internalDepthStencilFormat = GL::DEPTH24_STENCIL8;
        else if (attributes.depth)
            m_internalDepthStencilFormat = gl->supportsExtension("GL_OES_depth24"_s) ? GL::DEPTH_COMPONENT24 : GL::DEPTH_COMPONENT16;
        else
            m_internalDepthStencilFormat = GL::STENCIL_INDEX8;
    }
    m_internalColorFormat = attributes.alpha ? GL::RGBA8 : GL::RGB8;
}

void WebGLDefaultFramebuffer::reshape(IntSize size)
{
    auto* gl = m_context.graphicsContextGL();
    if (m_context.m_framebufferBinding)
        gl->bindFramebuffer(GL::FRAMEBUFFER, m_object);

    if (m_depthStencilBuffer) {
        gl->bindRenderbuffer(GL::RENDERBUFFER, m_depthStencilBuffer);
        if (m_msaaSampleCount)
            gl->renderbufferStorageMultisampleANGLE(GL::RENDERBUFFER, m_msaaSampleCount, m_internalDepthStencilFormat, size.width(), size.height());
        else
            gl->renderbufferStorage(GL::RENDERBUFFER, m_internalDepthStencilFormat, size.width(), size.height());
            gl->framebufferRenderbuffer(GL::FRAMEBUFFER, depthStencilAttachamentPoint(m_internalDepthStencilFormat), GL::RENDERBUFFER, m_depthStencilBuffer);
    }
    if (m_msaaSampleCount || !m_unpreservedBuffers) {
        gl->bindRenderbuffer(GL::RENDERBUFFER, m_colorBuffer);
        if (m_msaaSampleCount)
            gl->renderbufferStorageMultisampleANGLE(GL::RENDERBUFFER, m_msaaSampleCount, m_internalColorFormat, size.width(), size.height());
        else
            gl->renderbufferStorage(GL::RENDERBUFFER, m_internalColorFormat, size.width(), size.height());
        gl->framebufferRenderbuffer(GL::FRAMEBUFFER, GL::COLOR_ATTACHMENT0, GL::RENDERBUFFER, m_colorBuffer);
    } else {
        GCGLuint colorFormat = m_internalColorFormat == GL::RGBA8 ? GL::RGBA : GL::RGB;
        gl->bindTexture(drawingBufferTextureTarget(), m_colorBuffer);
        gl->reshapeDrawingBuffer(drawingBufferTextureTarget(), m_internalColorFormat, colorFormat, size.width(), size.height());
        gl->framebufferTexture2D(GL::FRAMEBUFFER, GL::COLOR_ATTACHMENT0, drawingBufferTextureTarget(), m_colorBuffer, 0);
    }

    ASSERT(gl->checkFramebufferStatus(GL::FRAMEBUFFER) == GL::FRAMEBUFFER_COMPLETE);

    auto& textureUnit = m_context.m_textureUnits[m_context.m_activeTextureUnit];
    gl->bindTexture(GL::TEXTURE_2D, objectOrZero(textureUnit.texture2DBinding.get()));
    gl->bindRenderbuffer(GL::RENDERBUFFER, objectOrZero(m_context.m_renderbufferBinding.get()));
    if (m_context.m_framebufferBinding)
        gl->bindFramebuffer(GL::FRAMEBUFFER, m_context.m_framebufferBinding->object());
}

void WebGLDefaultFramebuffer::markBuffersClear(GCGLbitfield clearBuffers)
{
    m_dirtyBuffers &= ~clearBuffers;
}

void WebGLDefaultFramebuffer::markAllUnpreservedBuffersDirty()
{
    m_dirtyBuffers = m_unpreservedBuffers;
}

#if 0
ScopedRestoreFramebufferBinding WebGLDefaultFramebuffer::resolveIfNeeded(IntRect rect)
{
    if (!m_msaaSampleCount)
        return { };

    ScopedDisableScissorTest disableScissor { m_context };
    ScopedDisableDither disableDither { m_context };

    GLint boundFrameBuffer = 0;
    GLint boundReadFrameBuffer = 0;
    if (m_isForWebGL2) {
        GL_GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &boundFrameBuffer);
        GL_GetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &boundReadFrameBuffer);
    } else
        GL_GetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFrameBuffer);
    GL_BindFramebuffer(GL_READ_FRAMEBUFFER_ANGLE, m_multisampleFBO);
    GL_BindFramebuffer(GL_DRAW_FRAMEBUFFER_ANGLE, m_fbo);

    // FIXME: figure out more efficient solution for iOS.
    if (m_isForWebGL2) {
        // ES 3.0 has BlitFramebuffer.
        IntRect resolveRect = rect.isEmpty() ? IntRect { 0, 0, m_currentWidth, m_currentHeight } : rect;
        GL_BlitFramebuffer(resolveRect.x(), resolveRect.y(), resolveRect.maxX(), resolveRect.maxY(), resolveRect.x(), resolveRect.y(), resolveRect.maxX(), resolveRect.maxY(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    } else {
        // ES 2.0 has BlitFramebufferANGLE only.
        GL_BlitFramebufferANGLE(0, 0, m_currentWidth, m_currentHeight, 0, 0, m_currentWidth, m_currentHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    if (m_isForWebGL2) {
        GL_BindFramebuffer(GL_DRAW_FRAMEBUFFER, boundFrameBuffer);
        GL_BindFramebuffer(GL_READ_FRAMEBUFFER, boundReadFrameBuffer);
    } else
        GL_BindFramebuffer(GL_FRAMEBUFFER, boundFrameBuffer);
}

void WebGLDefaultFramebuffer::restoreAfterResolveIfNeeded()
{

}
#endif

}

#endif
