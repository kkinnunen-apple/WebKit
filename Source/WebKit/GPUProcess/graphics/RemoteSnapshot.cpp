/*
 * Copyright (C) 2025 Apple Inc. All rights reserved.
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

#if ENABLE(GPU_PROCESS)
#include "RemoteSnapshot.h"

#include <wtf/TZoneMallocInlines.h>

namespace WebKit {
using namespace WebCore;

WTF_MAKE_TZONE_ALLOCATED_IMPL(RemoteSnapshot);

Ref<RemoteSnapshot> RemoteSnapshot::create()
{
    return adoptRef(*new RemoteSnapshot);
}

RemoteSnapshot::RemoteSnapshot() = default;

RemoteSnapshot::~RemoteSnapshot() = default;

bool RemoteSnapshot::addFrameReference(FrameIdentifier frameIdentifier)
{
    Locker locker(m_lock);
    m_referencedFrames++;
    auto result = m_frameDisplayLists.add(frameIdentifier, nullptr);
    if (result.isNewEntry)
        return true;
    // It is ok to setFrame win the race. It is not ok to have two addFrameReferences.
    return !result.iterator->value;
}

bool RemoteSnapshot::setFrame(FrameIdentifier frameIdentifier, Ref<const DisplayList::DisplayList>&& displayList)
{
    Locker locker(m_lock);
    m_completedFrames++;
    auto result = m_frameDisplayLists.add(frameIdentifier, RefPtr { displayList.ptr() });
    if (result.isNewEntry)
        return true;
    // It is ok to addFrameReference to win the race. It's not ok to have two setFrames.
    if (result.iterator->value)
        return false;
    result.iterator->value = WTFMove(displayList);
    return true;
}

bool RemoteSnapshot::applyFrame(FrameIdentifier frameIdentifier, GraphicsContext& context) const
{
    RefPtr<const DisplayList::DisplayList> displayList;
    {
        Locker locker(m_lock);
        displayList = m_frameDisplayLists.get(frameIdentifier);
    }
    if (!displayList)
        return false;
    context.drawDisplayList(*displayList);
    return true;
}

bool RemoteSnapshot::isComplete() const
{
    Locker locker(m_lock);
    return m_completedFrames == m_referencedFrames; // Duplicates are handled when the values are updated.
}

#if PLATFORM(COCOA)

std::optional<RefPtr<SharedBuffer>> RemoteSnapshot::drawToPDF(const IntSize& size, FrameIdentifier rootIdentifier)
{
    ASSERT(isComplete());
    RefPtr buffer = ImageBuffer::create(size, RenderingMode::PDFDocument, RenderingPurpose::Snapshot, 1, DestinationColorSpace::SRGB(), PixelFormat::BGRA8);
    if (!buffer)
        return nullptr;

    auto& context = buffer->context();

    if (!applyFrame(rootIdentifier, context))
        return std::nullopt;

    return ImageBuffer::sinkIntoPDFDocument(WTFMove(buffer));
}

#endif

}

#endif
