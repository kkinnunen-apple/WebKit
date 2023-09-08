/*
 * Copyright (C) 2015-2021 Apple Inc. All rights reserved.
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
#include "WebGLVertexArrayObjectBase.h"

#if ENABLE(WEBGL)

#include "WebCoreOpaqueRootInlines.h"
#include "WebGLRenderingContextBase.h"
#include <JavaScriptCore/AbstractSlotVisitorInlines.h>
#include <wtf/Locker.h>

namespace WebCore {

WebGLVertexArrayObjectBase::WebGLVertexArrayObjectBase(WebGLRenderingContextBase& context, PlatformGLObject object, Type type)
    : WebGLObject(context, object)
    , m_type(type)
{
    m_vertexAttribState.resize(context.getMaxVertexAttribs());
}

WebGLBuffer* WebGLVertexArrayObjectBase::getElementArrayBuffer() const
{
    ExclusiveSharedLocker locker { m_lock };
    return m_boundElementArrayBuffer.get();
}

void WebGLVertexArrayObjectBase::setElementArrayBuffer(WebGLBuffer* buffer)
{
    if (buffer)
        buffer->onAttached();

    Locker locker { m_lock };
    if (m_boundElementArrayBuffer)
        m_boundElementArrayBuffer->onDetached(context()->graphicsContextGL());
    m_boundElementArrayBuffer = buffer;
    
}
void WebGLVertexArrayObjectBase::setVertexAttribEnabled(int index, bool flag)
{
    Locker locker { m_lock }; // Redundant but done to get thread-safety analysis.
    auto& state = m_vertexAttribState[index];
    if (state.enabled == flag)
        return;
    state.enabled = flag;
    if (!state.validateBinding())
        m_allEnabledAttribBuffersBoundCache = false;
    else
        m_allEnabledAttribBuffersBoundCache.reset();
}

const WebGLVertexArrayObjectBase::VertexAttribState& WebGLVertexArrayObjectBase::getVertexAttribState(int index)
{
    ExclusiveSharedLocker locker { m_lock };
    return m_vertexAttribState[index];
}

void WebGLVertexArrayObjectBase::setVertexAttribState(GCGLuint index, GCGLsizei bytesPerElement, GCGLint size, GCGLenum type, GCGLboolean normalized, GCGLsizei stride, GCGLintptr offset, bool isInteger, WebGLBuffer* buffer)
{
    Locker locker { m_lock };
    auto& state = m_vertexAttribState[index];
    bool bindingWasValid = state.validateBinding();
    if (buffer)
        buffer->onAttached();
    if (state.bufferBinding)
        state.bufferBinding->onDetached(context()->graphicsContextGL());
    state.bufferBinding = buffer;
    if (!state.validateBinding())
        m_allEnabledAttribBuffersBoundCache = false;
    else if (!bindingWasValid)
        m_allEnabledAttribBuffersBoundCache.reset();
    state.bytesPerElement = bytesPerElement;
    state.size = size;
    state.type = type;
    state.normalized = normalized;
    state.stride = stride ? stride : bytesPerElement;
    state.originalStride = stride;
    state.offset = offset;
    state.isInteger = isInteger;
}

bool WebGLVertexArrayObjectBase::hasArrayBuffer(WebGLBuffer* buffer)
{
    ExclusiveSharedLocker locker { m_lock };
    return m_vertexAttribState.containsIf([&](auto& item) { return item.bufferBinding == buffer; });
}
 
void WebGLVertexArrayObjectBase::unbindBuffer(WebGLBuffer& buffer)
{
    Locker locker { m_lock };
    if (m_boundElementArrayBuffer == &buffer) {
        m_boundElementArrayBuffer->onDetached(context()->graphicsContextGL());
        m_boundElementArrayBuffer = nullptr;
    }
    
    for (auto& state : m_vertexAttribState) {
        if (state.bufferBinding == &buffer) {
            buffer.onDetached(context()->graphicsContextGL());
            state.bufferBinding = nullptr;
            if (!state.validateBinding())
                m_allEnabledAttribBuffersBoundCache = false;
        }
    }
}

void WebGLVertexArrayObjectBase::setVertexAttribDivisor(GCGLuint index, GCGLuint divisor)
{
    Locker locker { m_lock }; // Redundant but done to get thread-safety analysis.
    m_vertexAttribState[index].divisor = divisor;
}

void WebGLVertexArrayObjectBase::addMembersToOpaqueRoots(JSC::AbstractSlotVisitor& visitor)
{
    SharedLocker locker { m_lock };
    addWebCoreOpaqueRoot(visitor, m_boundElementArrayBuffer.get());
    for (auto& state : m_vertexAttribState)
        addWebCoreOpaqueRoot(visitor, state.bufferBinding.get());
}

bool WebGLVertexArrayObjectBase::areAllEnabledAttribBuffersBound()
{
    if (!m_allEnabledAttribBuffersBoundCache) {
        m_allEnabledAttribBuffersBoundCache = [&] {
            Locker locker { m_lock };
            for (const auto& state : m_vertexAttribState) {
                if (!state.validateBinding())
                    return false;
            }
            return true;
        }();
    }
    return *m_allEnabledAttribBuffersBoundCache;
}

WebCoreOpaqueRoot root(WebGLVertexArrayObjectBase* array)
{
    return WebCoreOpaqueRoot { array };
}

}

#endif // ENABLE(WEBGL)
