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

//
// mtl_device_spi.h:
//    Used to set Metal device wnership identity with SPI
//

#ifndef LIBANGLE_RENDERER_METAL_COMMAND_QUEUE_SPI_H_
#define LIBANGLE_RENDERER_METAL_COMMAND_QUEUE_SPI_H_

#import "common/apple/apple_platform.h"

#import <Metal/MTLResource_Private.h>
#import <Metal/Metal.h>
#import <mach/mach_types.h>

typedef NS_ENUM(NSUInteger, WebKitMTLGPUPriority) {
    MTLGPUPriorityLow = 2,
};

@protocol MTLCommandQueueSPI <MTLCommandQueue>
- (BOOL)setGPUPriority:(WebKitMTLGPUPriority)priority;
@end

namespace rx
{
namespace mtl
{
inline void setCommandQueuePriorityLow(id<MTLCommandQueue> queue)
{
    [(id<MTLCommandQueueSPI>)queue setGPUPriority:MTLGPUPriorityLow];
}
}
}

#endif /* LIBANGLE_RENDERER_METAL_COMMAND_QUEUE_SPI_H_ */
