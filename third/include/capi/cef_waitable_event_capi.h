﻿// Copyright (c) 2018 Marshall A. Greenblatt. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//
// $hash=b3e6242ecc3961df564d2df85b57ae449ccf03fc$
//

#ifndef CEF_INCLUDE_CAPI_CEF_WAITABLE_EVENT_CAPI_H_
#define CEF_INCLUDE_CAPI_CEF_WAITABLE_EVENT_CAPI_H_
#pragma once

#include "include/capi/cef_base_capi.h"

#ifdef __cplusplus
extern "C" {
#endif

///
// WaitableEvent is a thread synchronization tool that allows one thread to wait
// for another thread to finish some work. This is equivalent to using a
// Lock+ConditionVariable to protect a simple boolean value. However, using
// WaitableEvent in conjunction with a Lock to wait for a more complex state
// change (e.g., for an item to be added to a queue) is not recommended. In that
// case consider using a ConditionVariable instead of a WaitableEvent. It is
// safe to create and/or signal a WaitableEvent from any thread. Blocking on a
// WaitableEvent by calling the *wait() functions is not allowed on the browser
// process UI or IO threads.
///
typedef struct _cef_waitable_event_t {
  ///
  // Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  // Put the event in the un-signaled state.
  ///
  void(CEF_CALLBACK* reset)(struct _cef_waitable_event_t* self);

  ///
  // Put the event in the signaled state. This causes any thread blocked on Wait
  // to be woken up.
  ///
  void(CEF_CALLBACK* signal)(struct _cef_waitable_event_t* self);

  ///
  // Returns true (1) if the event is in the signaled state, else false (0). If
  // the event was created with |automatic_reset| set to true (1) then calling
  // this function will also cause a reset.
  ///
  int(CEF_CALLBACK* is_signaled)(struct _cef_waitable_event_t* self);

  ///
  // Wait indefinitely for the event to be signaled. This function will not
  // return until after the call to signal() has completed. This function cannot
  // be called on the browser process UI or IO threads.
  ///
  void(CEF_CALLBACK* wait)(struct _cef_waitable_event_t* self);

  ///
  // Wait up to |max_ms| milliseconds for the event to be signaled. Returns true
  // (1) if the event was signaled. A return value of false (0) does not
  // necessarily mean that |max_ms| was exceeded. This function will not return
  // until after the call to signal() has completed. This function cannot be
  // called on the browser process UI or IO threads.
  ///
  int(CEF_CALLBACK* timed_wait)(struct _cef_waitable_event_t* self,
                                int64 max_ms);
} cef_waitable_event_t;

///
// Create a new waitable event. If |automatic_reset| is true (1) then the event
// state is automatically reset to un-signaled after a single waiting thread has
// been released; otherwise, the state remains signaled until reset() is called
// manually. If |initially_signaled| is true (1) then the event will start in
// the signaled state.
///
CEF_EXPORT cef_waitable_event_t* cef_waitable_event_create(
    int automatic_reset,
    int initially_signaled);

#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_CEF_WAITABLE_EVENT_CAPI_H_
