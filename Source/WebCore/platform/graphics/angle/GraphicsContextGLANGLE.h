/*
 * Copyright (C) 2009, 2014-2019 Apple Inc. All rights reserved.
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

#include "ANGLEUtilities.h"
#include "GCGLSpan.h"
#include "GraphicsContextGL.h"
#include "GraphicsContextGLState.h"
#include <memory>
#include <wtf/Function.h>

namespace WebCore {

// Base class calling into ANGLE.
class WEBCORE_EXPORT GraphicsContextGLANGLE {
public:
    virtual ~GraphicsContextGLANGLE();

    void setClient(GraphicsContextGL::Client* client);
    const GraphicsContextGLAttributes& contextAttributes() const;

    GCGLDisplay platformDisplay() const;
    GCGLConfig platformConfig() const;

    GCGLenum drawingBufferTextureTarget();
    static GCGLint EGLDrawingBufferTextureTargetForDrawingTarget(GCGLenum drawingTarget);

    enum class ReleaseThreadResourceBehavior {
        // Releases current context after GraphicsContextGLANGLE calls done in the thread.
        ReleaseCurrentContext,
        // Releases all thread resources after GraphicsContextGLANGLE calls done in the thread.
        ReleaseThreadResources,
        // Releases all global state. Should be used only after all depending objects have
        // been released.
        TerminateAndReleaseThreadResources
    };
    static bool releaseThreadResources(ReleaseThreadResourceBehavior);

    // Should be called once before calls to any public entry points that eventually call OpenGL.
    bool makeContextCurrent() WARN_UNUSED_RETURN;

    void activeTexture(GCGLenum texture);
    void attachShader(PlatformGLObject program, PlatformGLObject shader);
    void bindAttribLocation(PlatformGLObject, GCGLuint index, const String& name);
    void bindBuffer(GCGLenum target, PlatformGLObject);
    void bindFramebuffer(GCGLenum target, PlatformGLObject);
    void bindRenderbuffer(GCGLenum target, PlatformGLObject);
    void bindTexture(GCGLenum target, PlatformGLObject);
    void blendColor(GCGLclampf red, GCGLclampf green, GCGLclampf blue, GCGLclampf alpha);
    void blendEquation(GCGLenum mode);
    void blendEquationSeparate(GCGLenum modeRGB, GCGLenum modeAlpha);
    void blendFunc(GCGLenum sfactor, GCGLenum dfactor);
    void blendFuncSeparate(GCGLenum srcRGB, GCGLenum dstRGB, GCGLenum srcAlpha, GCGLenum dstAlpha);
    void bufferData(GCGLenum target, GCGLsizeiptr, GCGLenum usage);
    void bufferData(GCGLenum target, std::span<const uint8_t> data, GCGLenum usage);
    void bufferSubData(GCGLenum target, GCGLintptr offset, std::span<const uint8_t> data);
    GCGLenum checkFramebufferStatus(GCGLenum target);
    void clear(GCGLbitfield mask);
    void clearColor(GCGLclampf red, GCGLclampf green, GCGLclampf blue, GCGLclampf alpha);
    void clearDepth(GCGLclampf depth);
    void clearStencil(GCGLint s);
    void colorMask(GCGLboolean red, GCGLboolean green, GCGLboolean blue, GCGLboolean alpha);
    void compileShader(PlatformGLObject);
    void copyTexImage2D(GCGLenum target, GCGLint level, GCGLenum internalformat, GCGLint x, GCGLint y, GCGLsizei width, GCGLsizei height, GCGLint border);
    void copyTexSubImage2D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLint x, GCGLint y, GCGLsizei width, GCGLsizei height);
    void cullFace(GCGLenum mode);
    void depthFunc(GCGLenum func);
    void depthMask(GCGLboolean flag);
    void depthRange(GCGLclampf zNear, GCGLclampf zFar);
    void detachShader(PlatformGLObject, PlatformGLObject);
    void disable(GCGLenum cap);
    void disableVertexAttribArray(GCGLuint index);
    void drawArrays(GCGLenum mode, GCGLint first, GCGLsizei count);
    void drawElements(GCGLenum mode, GCGLsizei count, GCGLenum type, GCGLintptr offset);
    void enable(GCGLenum cap);
    void enableVertexAttribArray(GCGLuint index);
    void finish();
    void flush();
    void framebufferRenderbuffer(GCGLenum target, GCGLenum attachment, GCGLenum renderbuffertarget, PlatformGLObject);
    void framebufferTexture2D(GCGLenum target, GCGLenum attachment, GCGLenum textarget, PlatformGLObject, GCGLint level);
    void frontFace(GCGLenum mode);
    void generateMipmap(GCGLenum target);
    bool getActiveAttrib(PlatformGLObject program, GCGLuint index, GraphicsContextGLActiveInfo&);
    bool getActiveAttribImpl(PlatformGLObject program, GCGLuint index, GraphicsContextGLActiveInfo&);
    bool getActiveUniform(PlatformGLObject program, GCGLuint index, GraphicsContextGLActiveInfo&);
    bool getActiveUniformImpl(PlatformGLObject program, GCGLuint index, GraphicsContextGLActiveInfo&);
    void getAttachedShaders(PlatformGLObject program, GCGLsizei maxCount, GCGLsizei* count, PlatformGLObject* shaders);
    GCGLint getAttribLocation(PlatformGLObject, const String& name);
    void getBooleanv(GCGLenum pname, std::span<GCGLboolean> value);
    GCGLint getBufferParameteri(GCGLenum target, GCGLenum pname);
    GCGLErrorCodeSet getErrors();
    void getFloatv(GCGLenum pname, std::span<GCGLfloat> value);
    GCGLint getFramebufferAttachmentParameteri(GCGLenum target, GCGLenum attachment, GCGLenum pname);
    void getIntegerv(GCGLenum pname, std::span<GCGLint> value);
    void getIntegeri_v(GCGLenum pname, GCGLuint index, std::span<GCGLint, 4> value); // NOLINT
    GCGLint64 getInteger64(GCGLenum pname);
    GCGLint64 getInteger64i(GCGLenum pname, GCGLuint index);
    GCGLint getProgrami(PlatformGLObject program, GCGLenum pname);
    String getProgramInfoLog(PlatformGLObject);
    GCGLint getRenderbufferParameteri(GCGLenum target, GCGLenum pname);
    GCGLint getShaderi(PlatformGLObject, GCGLenum pname);
    String getShaderInfoLog(PlatformGLObject);
    void getShaderPrecisionFormat(GCGLenum shaderType, GCGLenum precisionType, std::span<GCGLint, 2> range, GCGLint* precision);
    String getShaderSource(PlatformGLObject);
    String getString(GCGLenum name);
    GCGLfloat getTexParameterf(GCGLenum target, GCGLenum pname);
    GCGLint getTexParameteri(GCGLenum target, GCGLenum pname);
    void getUniformfv(PlatformGLObject program, GCGLint location, std::span<GCGLfloat> value);
    void getUniformiv(PlatformGLObject program, GCGLint location, std::span<GCGLint> value);
    void getUniformuiv(PlatformGLObject program, GCGLint location, std::span<GCGLuint> value);
    GCGLint getUniformLocation(PlatformGLObject, const String& name);
    GCGLsizeiptr getVertexAttribOffset(GCGLuint index, GCGLenum pname);
    void hint(GCGLenum target, GCGLenum mode);
    GCGLboolean isBuffer(PlatformGLObject);
    GCGLboolean isEnabled(GCGLenum cap);
    GCGLboolean isFramebuffer(PlatformGLObject);
    GCGLboolean isProgram(PlatformGLObject);
    GCGLboolean isRenderbuffer(PlatformGLObject);
    GCGLboolean isShader(PlatformGLObject);
    GCGLboolean isTexture(PlatformGLObject);
    void lineWidth(GCGLfloat);
    void linkProgram(PlatformGLObject);
    void pixelStorei(GCGLenum pname, GCGLint param);
    void polygonOffset(GCGLfloat factor, GCGLfloat units);
    void readPixels(IntRect, GCGLenum format, GCGLenum type, std::span<uint8_t> data, GCGLint alignment, GCGLint rowLength);
    void readPixelsBufferObject(IntRect, GCGLenum format, GCGLenum type, GCGLintptr offset, GCGLint alignment, GCGLint rowLength);
    void renderbufferStorage(GCGLenum target, GCGLenum internalformat, GCGLsizei width, GCGLsizei height);
    void sampleCoverage(GCGLclampf value, GCGLboolean invert);
    void scissor(GCGLint x, GCGLint y, GCGLsizei width, GCGLsizei height);
    void shaderSource(PlatformGLObject, const String&);
    void stencilFunc(GCGLenum func, GCGLint ref, GCGLuint mask);
    void stencilFuncSeparate(GCGLenum face, GCGLenum func, GCGLint ref, GCGLuint mask);
    void stencilMask(GCGLuint mask);
    void stencilMaskSeparate(GCGLenum face, GCGLuint mask);
    void stencilOp(GCGLenum fail, GCGLenum zfail, GCGLenum zpass);
    void stencilOpSeparate(GCGLenum face, GCGLenum fail, GCGLenum zfail, GCGLenum zpass);
    void texImage2D(GCGLenum target, GCGLint level, GCGLenum internalformat, GCGLsizei width, GCGLsizei height, GCGLint border, GCGLenum format, GCGLenum type, std::span<const uint8_t> pixels);
    void texImage2D(GCGLenum target, GCGLint level, GCGLenum internalformat, GCGLsizei width, GCGLsizei height, GCGLint border, GCGLenum format, GCGLenum type, GCGLintptr offset);
    void texSubImage2D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLsizei width, GCGLsizei height, GCGLenum format, GCGLenum type, std::span<const uint8_t> pixels);
    void texSubImage2D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLsizei width, GCGLsizei height, GCGLenum format, GCGLenum type, GCGLintptr offset);
    void compressedTexImage2D(GCGLenum target, GCGLint level, GCGLenum internalformat, GCGLsizei width, GCGLsizei height, GCGLint border, GCGLsizei imageSize, std::span<const uint8_t> data);
    void compressedTexImage2D(GCGLenum target, GCGLint level, GCGLenum internalformat, GCGLsizei width, GCGLsizei height, GCGLint border, GCGLsizei imageSize, GCGLintptr offset);
    void compressedTexSubImage2D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLsizei width, GCGLsizei height, GCGLenum format, GCGLsizei imageSize, std::span<const uint8_t> data);
    void compressedTexSubImage2D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLsizei width, GCGLsizei height, GCGLenum format, GCGLsizei imageSize, GCGLintptr offset);
    void texParameterf(GCGLenum target, GCGLenum pname, GCGLfloat param);
    void texParameteri(GCGLenum target, GCGLenum pname, GCGLint param);
    void uniform1f(GCGLint location, GCGLfloat x);
    void uniform1fv(GCGLint location, std::span<const GCGLfloat> v);
    void uniform1i(GCGLint location, GCGLint x);
    void uniform1iv(GCGLint location, std::span<const GCGLint> v);
    void uniform2f(GCGLint location, GCGLfloat x, GCGLfloat y);
    void uniform2fv(GCGLint location, std::span<const GCGLfloat> v);
    void uniform2i(GCGLint location, GCGLint x, GCGLint y);
    void uniform2iv(GCGLint location, std::span<const GCGLint> v);
    void uniform3f(GCGLint location, GCGLfloat x, GCGLfloat y, GCGLfloat z);
    void uniform3fv(GCGLint location, std::span<const GCGLfloat> v);
    void uniform3i(GCGLint location, GCGLint x, GCGLint y, GCGLint z);
    void uniform3iv(GCGLint location, std::span<const GCGLint> v);
    void uniform4f(GCGLint location, GCGLfloat x, GCGLfloat y, GCGLfloat z, GCGLfloat w);
    void uniform4fv(GCGLint location, std::span<const GCGLfloat> v);
    void uniform4i(GCGLint location, GCGLint x, GCGLint y, GCGLint z, GCGLint w);
    void uniform4iv(GCGLint location, std::span<const GCGLint> v);
    void uniformMatrix2fv(GCGLint location, GCGLboolean transpose, std::span<const GCGLfloat> value);
    void uniformMatrix3fv(GCGLint location, GCGLboolean transpose, std::span<const GCGLfloat> value);
    void uniformMatrix4fv(GCGLint location, GCGLboolean transpose, std::span<const GCGLfloat> value);
    void useProgram(PlatformGLObject);
    void validateProgram(PlatformGLObject);
    void vertexAttrib1f(GCGLuint index, GCGLfloat x);
    void vertexAttrib1fv(GCGLuint index, std::span<const GCGLfloat, 1> values);
    void vertexAttrib2f(GCGLuint index, GCGLfloat x, GCGLfloat y);
    void vertexAttrib2fv(GCGLuint index, std::span<const GCGLfloat, 2> values);
    void vertexAttrib3f(GCGLuint index, GCGLfloat x, GCGLfloat y, GCGLfloat z);
    void vertexAttrib3fv(GCGLuint index, std::span<const GCGLfloat, 3> values);
    void vertexAttrib4f(GCGLuint index, GCGLfloat x, GCGLfloat y, GCGLfloat z, GCGLfloat w);
    void vertexAttrib4fv(GCGLuint index, std::span<const GCGLfloat, 4> values);
    void vertexAttribPointer(GCGLuint index, GCGLint size, GCGLenum type, GCGLboolean normalized, GCGLsizei stride, GCGLintptr offset);
    void viewport(GCGLint x, GCGLint y, GCGLsizei width, GCGLsizei height);
    void reshape(int width, int height);
    void drawArraysInstanced(GCGLenum mode, GCGLint first, GCGLsizei count, GCGLsizei primcount);
    void drawElementsInstanced(GCGLenum mode, GCGLsizei count, GCGLenum type, GCGLintptr offset, GCGLsizei primcount);
    void vertexAttribDivisor(GCGLuint index, GCGLuint divisor);
    PlatformGLObject createVertexArray();
    void deleteVertexArray(PlatformGLObject);
    GCGLboolean isVertexArray(PlatformGLObject);
    void bindVertexArray(PlatformGLObject);
    void copyBufferSubData(GCGLenum readTarget, GCGLenum writeTarget, GCGLintptr readOffset, GCGLintptr writeOffset, GCGLsizeiptr);
    void getBufferSubData(GCGLenum target, GCGLintptr offset, std::span<uint8_t> data);
    void blitFramebuffer(GCGLint srcX0, GCGLint srcY0, GCGLint srcX1, GCGLint srcY1, GCGLint dstX0, GCGLint dstY0, GCGLint dstX1, GCGLint dstY1, GCGLbitfield mask, GCGLenum filter);
    void framebufferTextureLayer(GCGLenum target, GCGLenum attachment, PlatformGLObject texture, GCGLint level, GCGLint layer);
    void invalidateFramebuffer(GCGLenum target, std::span<const GCGLenum> attachments);
    void invalidateSubFramebuffer(GCGLenum target, std::span<const GCGLenum> attachments, GCGLint x, GCGLint y, GCGLsizei width, GCGLsizei height);
    void readBuffer(GCGLenum src);
    void getInternalformativ(GCGLenum target, GCGLenum internalformat, GCGLenum pname, std::span<GCGLint> data);
    void renderbufferStorageMultisample(GCGLenum target, GCGLsizei samples, GCGLenum internalformat, GCGLsizei width, GCGLsizei height);
    void texStorage2D(GCGLenum target, GCGLsizei levels, GCGLenum internalformat, GCGLsizei width, GCGLsizei height);
    void texStorage3D(GCGLenum target, GCGLsizei levels, GCGLenum internalformat, GCGLsizei width, GCGLsizei height, GCGLsizei depth);
    void texImage3D(GCGLenum target, GCGLint level, GCGLint internalformat, GCGLsizei width, GCGLsizei height, GCGLsizei depth, GCGLint border, GCGLenum format, GCGLenum type, std::span<const uint8_t> pixels);
    void texImage3D(GCGLenum target, GCGLint level, GCGLint internalformat, GCGLsizei width, GCGLsizei height, GCGLsizei depth, GCGLint border, GCGLenum format, GCGLenum type, GCGLintptr offset);
    void texSubImage3D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLint zoffset, GCGLsizei width, GCGLsizei height, GCGLsizei depth, GCGLenum format, GCGLenum type, std::span<const uint8_t> pixels);
    void texSubImage3D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLint zoffset, GCGLsizei width, GCGLsizei height, GCGLsizei depth, GCGLenum format, GCGLenum type, GCGLintptr offset);
    void copyTexSubImage3D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLint zoffset, GCGLint x, GCGLint y, GCGLsizei width, GCGLsizei height);
    void compressedTexImage3D(GCGLenum target, GCGLint level, GCGLenum internalformat, GCGLsizei width, GCGLsizei height, GCGLsizei depth, GCGLint border, GCGLsizei imageSize, std::span<const uint8_t> data);
    void compressedTexImage3D(GCGLenum target, GCGLint level, GCGLenum internalformat, GCGLsizei width, GCGLsizei height, GCGLsizei depth, GCGLint border, GCGLsizei imageSize, GCGLintptr offset);
    void compressedTexSubImage3D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLint zoffset, GCGLsizei width, GCGLsizei height, GCGLsizei depth, GCGLenum format, GCGLsizei imageSize, std::span<const uint8_t> data);
    void compressedTexSubImage3D(GCGLenum target, GCGLint level, GCGLint xoffset, GCGLint yoffset, GCGLint zoffset, GCGLsizei width, GCGLsizei height, GCGLsizei depth, GCGLenum format, GCGLsizei imageSize, GCGLintptr offset);
    GCGLint getFragDataLocation(PlatformGLObject program, const String& name);
    void uniform1ui(GCGLint location, GCGLuint v0);
    void uniform2ui(GCGLint location, GCGLuint v0, GCGLuint v1);
    void uniform3ui(GCGLint location, GCGLuint v0, GCGLuint v1, GCGLuint v2);
    void uniform4ui(GCGLint location, GCGLuint v0, GCGLuint v1, GCGLuint v2, GCGLuint v3);
    void uniform1uiv(GCGLint location, std::span<const GCGLuint>);
    void uniform2uiv(GCGLint location, std::span<const GCGLuint>);
    void uniform3uiv(GCGLint location, std::span<const GCGLuint>);
    void uniform4uiv(GCGLint location, std::span<const GCGLuint>);
    void uniformMatrix2x3fv(GCGLint location, GCGLboolean transpose, std::span<const GCGLfloat> data);
    void uniformMatrix3x2fv(GCGLint location, GCGLboolean transpose, std::span<const GCGLfloat> data);
    void uniformMatrix2x4fv(GCGLint location, GCGLboolean transpose, std::span<const GCGLfloat> data);
    void uniformMatrix4x2fv(GCGLint location, GCGLboolean transpose, std::span<const GCGLfloat> data);
    void uniformMatrix3x4fv(GCGLint location, GCGLboolean transpose, std::span<const GCGLfloat> data);
    void uniformMatrix4x3fv(GCGLint location, GCGLboolean transpose, std::span<const GCGLfloat> data);
    void vertexAttribI4i(GCGLuint index, GCGLint x, GCGLint y, GCGLint z, GCGLint w);
    void vertexAttribI4iv(GCGLuint index, std::span<const GCGLint, 4> values);
    void vertexAttribI4ui(GCGLuint index, GCGLuint x, GCGLuint y, GCGLuint z, GCGLuint w);
    void vertexAttribI4uiv(GCGLuint index, std::span<const GCGLuint, 4> values);
    void vertexAttribIPointer(GCGLuint index, GCGLint size, GCGLenum type, GCGLsizei stride, GCGLintptr offset);
    void drawRangeElements(GCGLenum mode, GCGLuint start, GCGLuint end, GCGLsizei count, GCGLenum type, GCGLintptr offset);
    void drawBuffers(std::span<const GCGLenum> bufs);
    void clearBufferiv(GCGLenum buffer, GCGLint drawbuffer, std::span<const GCGLint> values);
    void clearBufferuiv(GCGLenum buffer, GCGLint drawbuffer, std::span<const GCGLuint> values);
    void clearBufferfv(GCGLenum buffer, GCGLint drawbuffer, std::span<const GCGLfloat> values);
    void clearBufferfi(GCGLenum buffer, GCGLint drawbuffer, GCGLfloat depth, GCGLint stencil);
    PlatformGLObject createQuery();
    void deleteQuery(PlatformGLObject query);
    GCGLboolean isQuery(PlatformGLObject query);
    void beginQuery(GCGLenum target, PlatformGLObject query);
    void endQuery(GCGLenum target);
    GCGLint getQuery(GCGLenum target, GCGLenum pname);
    GCGLuint getQueryObjectui(PlatformGLObject query, GCGLenum pname);
    PlatformGLObject createSampler();
    void deleteSampler(PlatformGLObject sampler);
    GCGLboolean isSampler(PlatformGLObject sampler);
    void bindSampler(GCGLuint unit, PlatformGLObject sampler);
    void samplerParameteri(PlatformGLObject sampler, GCGLenum pname, GCGLint param);
    void samplerParameterf(PlatformGLObject sampler, GCGLenum pname, GCGLfloat param);
    GCGLfloat getSamplerParameterf(PlatformGLObject sampler, GCGLenum pname);
    GCGLint getSamplerParameteri(PlatformGLObject sampler, GCGLenum pname);
    GCGLsync fenceSync(GCGLenum condition, GCGLbitfield flags);
    GCGLboolean isSync(GCGLsync);
    void deleteSync(GCGLsync);
    GCGLenum clientWaitSync(GCGLsync, GCGLbitfield flags, GCGLuint64 timeout);
    void waitSync(GCGLsync, GCGLbitfield flags, GCGLint64 timeout);
    GCGLint getSynci(GCGLsync, GCGLenum pname);
    PlatformGLObject createTransformFeedback();
    void deleteTransformFeedback(PlatformGLObject id);
    GCGLboolean isTransformFeedback(PlatformGLObject id);
    void bindTransformFeedback(GCGLenum target, PlatformGLObject id);
    void beginTransformFeedback(GCGLenum primitiveMode);
    void endTransformFeedback();
    void transformFeedbackVaryings(PlatformGLObject program, const Vector<String>& varyings, GCGLenum bufferMode);
    void getTransformFeedbackVarying(PlatformGLObject program, GCGLuint index, GraphicsContextGLActiveInfo&);
    void pauseTransformFeedback();
    void resumeTransformFeedback();
    void bindBufferBase(GCGLenum target, GCGLuint index, PlatformGLObject buffer);
    void bindBufferRange(GCGLenum target, GCGLuint index, PlatformGLObject buffer, GCGLintptr offset, GCGLsizeiptr);
    Vector<GCGLuint> getUniformIndices(PlatformGLObject program, const Vector<String>& uniformNames);
    Vector<GCGLint> getActiveUniforms(PlatformGLObject program, const Vector<GCGLuint>& uniformIndices, GCGLenum pname);
    GCGLuint getUniformBlockIndex(PlatformGLObject program, const String& uniformBlockName);
    String getActiveUniformBlockName(PlatformGLObject program, GCGLuint uniformBlockIndex);
    void uniformBlockBinding(PlatformGLObject program, GCGLuint uniformBlockIndex, GCGLuint uniformBlockBinding);
    void getActiveUniformBlockiv(GCGLuint program, GCGLuint uniformBlockIndex, GCGLenum pname, std::span<GCGLint> params);
    void destroyEGLImage(GCEGLImage);
    bool destroyEGLSync(GCEGLSync);
    void clientWaitEGLSyncWithFlush(GCEGLSync, uint64_t);
    void multiDrawArraysANGLE(GCGLenum mode, GCGLSpanTuple<const GCGLint, const GCGLsizei> firstsAndCounts);
    void multiDrawArraysInstancedANGLE(GCGLenum mode, GCGLSpanTuple<const GCGLint, const GCGLsizei, const GCGLsizei> firstsCountsAndInstanceCounts);
    void multiDrawElementsANGLE(GCGLenum mode, GCGLSpanTuple<const GCGLsizei, const GCGLsizei> countsAndOffsets, GCGLenum type);
    void multiDrawElementsInstancedANGLE(GCGLenum mode, GCGLSpanTuple<const GCGLsizei, const GCGLsizei, const GCGLsizei> countsOffsetsAndInstanceCounts, GCGLenum type);
    bool supportsExtension(const String&);
    void ensureExtensionEnabled(const String&);
    bool isExtensionEnabled(const String&);
    void drawBuffersEXT(std::span<const GCGLenum>);
    String getTranslatedShaderSourceANGLE(PlatformGLObject);
    PlatformGLObject createQueryEXT();
    void deleteQueryEXT(PlatformGLObject query);
    GCGLboolean isQueryEXT(PlatformGLObject query);
    void beginQueryEXT(GCGLenum target, PlatformGLObject query);
    void endQueryEXT(GCGLenum target);
    void queryCounterEXT(PlatformGLObject query, GCGLenum target);
    GCGLint getQueryiEXT(GCGLenum target, GCGLenum pname);
    GCGLint getQueryObjectiEXT(PlatformGLObject query, GCGLenum pname);
    GCGLuint64 getQueryObjectui64EXT(PlatformGLObject query, GCGLenum pname);
    GCGLint64 getInteger64EXT(GCGLenum pname);
    void enableiOES(GCGLenum target, GCGLuint index);
    void disableiOES(GCGLenum target, GCGLuint index);
    void blendEquationiOES(GCGLuint buf, GCGLenum mode);
    void blendEquationSeparateiOES(GCGLuint buf, GCGLenum modeRGB, GCGLenum modeAlpha);
    void blendFunciOES(GCGLuint buf, GCGLenum src, GCGLenum dst);
    void blendFuncSeparateiOES(GCGLuint buf, GCGLenum srcRGB, GCGLenum dstRGB, GCGLenum srcAlpha, GCGLenum dstAlpha);
    void colorMaskiOES(GCGLuint buf, GCGLboolean red, GCGLboolean green, GCGLboolean blue, GCGLboolean alpha);
    void drawArraysInstancedBaseInstanceANGLE(GCGLenum mode, GCGLint first, GCGLsizei count, GCGLsizei instanceCount, GCGLuint baseInstance);
    void drawElementsInstancedBaseVertexBaseInstanceANGLE(GCGLenum mode, GCGLsizei count, GCGLenum type, GCGLintptr offset, GCGLsizei instanceCount, GCGLint baseVertex, GCGLuint baseInstance);
    void multiDrawArraysInstancedBaseInstanceANGLE(GCGLenum mode, GCGLSpanTuple<const GCGLint, const GCGLsizei, const GCGLsizei, const GCGLuint> firstsCountsInstanceCountsAndBaseInstances);
    void multiDrawElementsInstancedBaseVertexBaseInstanceANGLE(GCGLenum mode, GCGLSpanTuple<const GCGLsizei, const GCGLsizei, const GCGLsizei, const GCGLint, const GCGLuint> countsOffsetsInstanceCountsBaseVerticesAndBaseInstances, GCGLenum type);
    void clipControlEXT(GCGLenum origin, GCGLenum depth);
    void provokingVertexANGLE(GCGLenum provokeMode);
    void polygonModeANGLE(GCGLenum face, GCGLenum mode);
    void polygonOffsetClampEXT(GCGLfloat factor, GCGLfloat units, GCGLfloat clamp);
    void renderbufferStorageMultisampleANGLE(GCGLenum target, GCGLsizei samples, GCGLenum internalformat, GCGLsizei width, GCGLsizei height);
    void blitFramebufferANGLE(GCGLint srcX0, GCGLint srcY0, GCGLint srcX1, GCGLint srcY1, GCGLint dstX0, GCGLint dstY0, GCGLint dstX1, GCGLint dstY1, GCGLbitfield mask, GCGLenum filter);

    PlatformGLObject createBuffer();
    PlatformGLObject createFramebuffer();
    PlatformGLObject createProgram();
    PlatformGLObject createRenderbuffer();
    PlatformGLObject createShader(GCGLenum);
    PlatformGLObject createTexture();
    void deleteBuffer(PlatformGLObject);
    void deleteFramebuffer(PlatformGLObject);
    void deleteProgram(PlatformGLObject);
    void deleteRenderbuffer(PlatformGLObject);
    void deleteShader(PlatformGLObject);
    void deleteTexture(PlatformGLObject);
    
    using SimulatedEventForTesting = GraphicsContextGL::SimulatedEventForTesting;
    using SurfaceBuffer = GraphicsContextGL::SurfaceBuffer;
    using FlipY = GraphicsContextGL::FlipY;
    void simulateEventForTesting(SimulatedEventForTesting);
    void drawSurfaceBufferToImageBuffer(SurfaceBuffer, ImageBuffer&);
    RefPtr<PixelBuffer> drawingBufferToPixelBuffer(FlipY);
    RefPtr<PixelBuffer> readRenderingResultsForPainting();

    std::tuple<GCGLenum, GCGLenum> drawingBufferTextureBindingPoint();
    virtual std::tuple<GCGLenum, GCGLenum> externalImageTextureBindingPoint();
    virtual std::optional<GCEGLImageAttachResult> createAndBindEGLImage(GCGLenum, GCEGLImageSource);
    virtual GCEGLSync createEGLSync(GCExternalEGLSyncEvent);
    virtual bool enableRequiredWebXRExtensions();
    virtual void setDrawingBufferColorSpace(const DestinationColorSpace&);
    virtual void withBufferAsNativeImage(SurfaceBuffer, Function<void(NativeImage&)>);
    virtual void setContextVisibility(bool);
    virtual void prepareForDisplay();

    // Reads pixels from positive pixel coordinates with tight packing.
    // Returns columns, rows of executed read on success.
    std::optional<IntSize> readPixelsWithStatus(IntRect, GCGLenum format, GCGLenum type, std::span<uint8_t> data);

    void addError(GCGLErrorCode);
protected:
    GraphicsContextGLANGLE(GraphicsContextGLAttributes);

    bool updateErrors();

    // Initializes the instance. Returns false if the instance should not be used.
    bool initialize();
    // Called first by initialize(). Subclasses should override to instantiate the platform specific bits of EGLContext.
    // FIXME: Currently platforms do not share the context creation. They should.
    virtual bool platformInitializeContext() = 0;
    // Called by initialize(). Subclasses should override to enable platform specific extensions.
    virtual bool platformInitializeExtensions();
    // Called by initialize(). Subclasses should override to instantiate platform specific state that depend on
    // the shared state.
    virtual bool platformInitialize();

    // Take into account the user's requested context creation attributes,
    // in particular stencil and antialias, and determine which could or
    // could not be honored based on the capabilities of the OpenGL
    // implementation.
    void validateDepthStencil(ASCIILiteral packedDepthStencilExtension);
    void validateAttributes();

    std::optional<IntSize> readPixelsImpl(IntRect, GCGLenum format, GCGLenum type, GCGLsizei bufSize, uint8_t* data, bool readingToPixelBufferObject);

    // Did the most recent drawing operation leave the GPU in an acceptable state?
    void checkGPUStatus();

    RefPtr<PixelBuffer> readRenderingResults();
    virtual RefPtr<PixelBuffer> readCompositedResults() = 0;
    RefPtr<PixelBuffer> readPixelsForPaintResults();

    bool reshapeFBOs(const IntSize&);
    void prepareTexture();
    void resolveMultisamplingIfNecessary(const IntRect& = IntRect());
    void attachDepthAndStencilBufferIfNeeded(GCGLuint internalDepthStencilFormat, int width, int height);
#if PLATFORM(COCOA)
    static bool makeCurrent(GCGLDisplay, GCGLContext);
#endif
    virtual bool reshapeDrawingBuffer() = 0;

    static void platformReleaseThreadResources();

    virtual void invalidateKnownTextureContent(GCGLuint);
    bool enableExtension(const String&) WARN_UNUSED_RETURN;
    void requestExtension(const String&);

    // Only for non-WebGL 2.0 contexts.
    GCGLenum adjustWebGL1TextureInternalFormat(GCGLenum internalformat, GCGLenum format, GCGLenum type);
    void setPackParameters(GCGLint alignment, GCGLint rowLength);

    HashSet<String> m_availableExtensions;
    HashSet<String> m_requestableExtensions;
    HashSet<String> m_enabledExtensions;
    bool m_webglColorBufferFloatRGB { false };
    bool m_webglColorBufferFloatRGBA { false };
    GCGLuint m_texture { 0 };
    GCGLuint m_fbo { 0 };
    GCGLuint m_depthStencilBuffer { 0 };
    GCGLuint m_internalColorFormat { 0 };
    GCGLuint m_internalDepthStencilFormat { 0 };
    GCGLuint m_multisampleFBO { 0 };
    GCGLuint m_multisampleDepthStencilBuffer { 0 };
    GCGLuint m_multisampleColorBuffer { 0 };
    // For preserveDrawingBuffer:true without multisampling.
    GCGLuint m_preserveDrawingBufferTexture { 0 };
    // Attaches m_texture when m_preserveDrawingBufferTexture is non-zero.
    GCGLuint m_preserveDrawingBufferFBO { 0 };
    // Queried at display startup.
    GCGLint m_drawingBufferTextureTarget { -1 };
    GCGLErrorCodeSet m_errors;
    bool m_isForWebGL2 { false };
    bool m_failNextStatusCheck { false };
    GraphicsContextGLState m_state;

    GCGLDisplay m_displayObj { nullptr };
    GCGLContext m_contextObj { nullptr };
    GCGLConfig m_configObj { nullptr };
#if USE(TEXTURE_MAPPER)
    GCEGLSuface m_surfaceObj { nullptr };
#endif
    GCGLint m_packAlignment { 4 };
    GCGLint m_packRowLength { 0 };
    GraphicsContextGL::Client* m_client { nullptr };
    GraphicsContextGLAttributes m_attributes;
    int m_currentWidth { 0 };
    int m_currentHeight { 0 };
};


inline GCGLDisplay GraphicsContextGLANGLE::platformDisplay() const
{
    return m_displayObj;
}

inline GCGLConfig GraphicsContextGLANGLE::platformConfig() const
{
    return m_configObj;
}

}

#endif
