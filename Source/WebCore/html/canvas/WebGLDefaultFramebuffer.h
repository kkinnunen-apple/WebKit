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

#if ENABLE(WEBGL)

#include "WebGLRenderingContextBase.h"

namespace WebCore {

// Implementation for the WebGL context default framebuffer.
class WebGLDefaultFramebuffer {
    WTF_MAKE_FAST_ALLOCATED;
    WTF_MAKE_NONCOPYABLE(WebGLDefaultFramebuffer);
public:
    static std::unique_ptr<WebGLDefaultFramebuffer> create(WebGLRenderingContextBase&, const WebGLContextAttributes&, IntSize);
    ~WebGLDefaultFramebuffer() = default;

    PlatformGLObject object() const { return m_object; }
    void reshape(IntSize);
    GCGLbitfield dirtyBuffers() const { return m_dirtyBuffers; }
    void markBuffersClear(GCGLbitfield clearBuffers);
    void markAllUnpreservedBuffersDirty();

private:
    WebGLDefaultFramebuffer(WebGLRenderingContextBase&, const WebGLContextAttributes&);

    WebGLRenderingContextBase& m_context;
    PlatformGLObject m_object { 0 };
    PlatformGLObject m_colorBuffer { 0 };
    PlatformGLObject m_depthStencilBuffer { 0 };
    GCGLenum m_internalDepthStencilFormat { 0 };
    GCGLbitfield m_unpreservedBuffers { 0 };
    GCGLbitfield m_dirtyBuffers { 0 };
    GCGLint m_msaaSampleCount { 0 }; // 0 means antialias == false.
    GCGLenum m_internalColorFormat { 0 };

};

}

#endif
