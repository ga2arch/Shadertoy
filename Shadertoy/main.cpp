//
//  main.cpp
//  Shadertoy
//
//  Created by Gabriele on 04/10/14.
//  Copyright (c) 2014 Ga2 & co. All rights reserved.
//

#include "Shadertoy.h"

#include <iostream>
#include <thread>

void callback(ConstFSEventStreamRef stream,
              void *callbackInfo,
              size_t numEvents,
              void *evPaths,
              const FSEventStreamEventFlags evFlags[],
              const FSEventStreamEventId evIds[]) {
    
    recompile = true;
}


void watch(CFRunLoopRef* loop) {
    auto arg = CFStringCreateWithCString(kCFAllocatorDefault, "shaders",
                                         kCFStringEncodingUTF8);
    
    auto paths = CFArrayCreate(nullptr, (const void**)&arg, 1, nullptr);
    CFAbsoluteTime latency = .5;
    
    auto stream = FSEventStreamCreate(nullptr,
                                 &callback,
                                 nullptr,
                                 paths,
                                 kFSEventStreamEventIdSinceNow,
                                 latency,
                                 kFSEventStreamCreateFlagNone);
    
    *loop = CFRunLoopGetCurrent();
    FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);
    
    CFRunLoopRun();
    
    FSEventStreamStop(stream);
    FSEventStreamInvalidate(stream);
    FSEventStreamRelease(stream);
}

int main(int argc, const char * argv[])
{
    CFRunLoopRef loop;
    thread watcher (watch, &loop);
    
    Shadertoy toy;
    toy.play();
    
    CFRunLoopStop(loop);
    watcher.join();
}