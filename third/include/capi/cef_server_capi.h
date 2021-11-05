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
// $hash=0653420628236b2bc16541d65b75742aba4e4613$
//

#ifndef CEF_INCLUDE_CAPI_CEF_SERVER_CAPI_H_
#define CEF_INCLUDE_CAPI_CEF_SERVER_CAPI_H_
#pragma once

#include "include/capi/cef_base_capi.h"
#include "include/capi/cef_callback_capi.h"
#include "include/capi/cef_request_capi.h"
#include "include/capi/cef_task_capi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _cef_server_handler_t;

///
// Structure representing a server that supports HTTP and WebSocket requests.
// Server capacity is limited and is intended to handle only a small number of
// simultaneous connections (e.g. for communicating between applications on
// localhost). The functions of this structure are safe to call from any thread
// in the brower process unless otherwise indicated.
///
typedef struct _cef_server_t {
  ///
  // Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  // Returns the task runner for the dedicated server thread.
  ///
  struct _cef_task_runner_t*(CEF_CALLBACK* get_task_runner)(
      struct _cef_server_t* self);

  ///
  // Stop the server and shut down the dedicated server thread. See
  // cef_server_handler_t::OnServerCreated documentation for a description of
  // server lifespan.
  ///
  void(CEF_CALLBACK* shutdown)(struct _cef_server_t* self);

  ///
  // Returns true (1) if the server is currently running and accepting incoming
  // connections. See cef_server_handler_t::OnServerCreated documentation for a
  // description of server lifespan. This function must be called on the
  // dedicated server thread.
  ///
  int(CEF_CALLBACK* is_running)(struct _cef_server_t* self);

  ///
  // Returns the server address including the port number.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t(CEF_CALLBACK* get_address)(struct _cef_server_t* self);

  ///
  // Returns true (1) if the server currently has a connection. This function
  // must be called on the dedicated server thread.
  ///
  int(CEF_CALLBACK* has_connection)(struct _cef_server_t* self);

  ///
  // Returns true (1) if |connection_id| represents a valid connection. This
  // function must be called on the dedicated server thread.
  ///
  int(CEF_CALLBACK* is_valid_connection)(struct _cef_server_t* self,
                                         int connection_id);

  ///
  // Send an HTTP 200 "OK" response to the connection identified by
  // |connection_id|. |content_type| is the response content type (e.g.
  // "text/html"), |data| is the response content, and |data_size| is the size
  // of |data| in bytes. The contents of |data| will be copied. The connection
  // will be closed automatically after the response is sent.
  ///
  void(CEF_CALLBACK* send_http200response)(struct _cef_server_t* self,
                                           int connection_id,
                                           const cef_string_t* content_type,
                                           const void* data,
                                           size_t data_size);

  ///
  // Send an HTTP 404 "Not Found" response to the connection identified by
  // |connection_id|. The connection will be closed automatically after the
  // response is sent.
  ///
  void(CEF_CALLBACK* send_http404response)(struct _cef_server_t* self,
                                           int connection_id);

  ///
  // Send an HTTP 500 "Internal Server Error" response to the connection
  // identified by |connection_id|. |error_message| is the associated error
  // message. The connection will be closed automatically after the response is
  // sent.
  ///
  void(CEF_CALLBACK* send_http500response)(struct _cef_server_t* self,
                                           int connection_id,
                                           const cef_string_t* error_message);

  ///
  // Send a custom HTTP response to the connection identified by
  // |connection_id|. |response_code| is the HTTP response code sent in the
  // status line (e.g. 200), |content_type| is the response content type sent as
  // the "Content-Type" header (e.g. "text/html"), |content_length| is the
  // expected content length, and |extra_headers| is the map of extra response
  // headers. If |content_length| is >= 0 then the "Content-Length" header will
  // be sent. If |content_length| is 0 then no content is expected and the
  // connection will be closed automatically after the response is sent. If
  // |content_length| is < 0 then no "Content-Length" header will be sent and
  // the client will continue reading until the connection is closed. Use the
  // SendRawData function to send the content, if applicable, and call
  // CloseConnection after all content has been sent.
  ///
  void(CEF_CALLBACK* send_http_response)(struct _cef_server_t* self,
                                         int connection_id,
                                         int response_code,
                                         const cef_string_t* content_type,
                                         int64 content_length,
                                         cef_string_multimap_t extra_headers);

  ///
  // Send raw data directly to the connection identified by |connection_id|.
  // |data| is the raw data and |data_size| is the size of |data| in bytes. The
  // contents of |data| will be copied. No validation of |data| is performed
  // internally so the client should be careful to send the amount indicated by
  // the "Content-Length" header, if specified. See SendHttpResponse
  // documentation for intended usage.
  ///
  void(CEF_CALLBACK* send_raw_data)(struct _cef_server_t* self,
                                    int connection_id,
                                    const void* data,
                                    size_t data_size);

  ///
  // Close the connection identified by |connection_id|. See SendHttpResponse
  // documentation for intended usage.
  ///
  void(CEF_CALLBACK* close_connection)(struct _cef_server_t* self,
                                       int connection_id);

  ///
  // Send a WebSocket message to the connection identified by |connection_id|.
  // |data| is the response content and |data_size| is the size of |data| in
  // bytes. The contents of |data| will be copied. See
  // cef_server_handler_t::OnWebSocketRequest documentation for intended usage.
  ///
  void(CEF_CALLBACK* send_web_socket_message)(struct _cef_server_t* self,
                                              int connection_id,
                                              const void* data,
                                              size_t data_size);
} cef_server_t;

///
// Create a new server that binds to |address| and |port|. |address| must be a
// valid IPv4 or IPv6 address (e.g. 127.0.0.1 or ::1) and |port| must be a port
// number outside of the reserved range (e.g. between 1025 and 65535 on most
// platforms). |backlog| is the maximum number of pending connections. A new
// thread will be created for each CreateServer call (the "dedicated server
// thread"). It is therefore recommended to use a different cef_server_handler_t
// instance for each CreateServer call to avoid thread safety issues in the
// cef_server_handler_t implementation. The
// cef_server_handler_t::OnServerCreated function will be called on the
// dedicated server thread to report success or failure. See
// cef_server_handler_t::OnServerCreated documentation for a description of
// server lifespan.
///
CEF_EXPORT void cef_server_create(const cef_string_t* address,
                                  uint16 port,
                                  int backlog,
                                  struct _cef_server_handler_t* handler);

///
// Implement this structure to handle HTTP server requests. A new thread will be
// created for each cef_server_t::CreateServer call (the "dedicated server
// thread"), and the functions of this structure will be called on that thread.
// It is therefore recommended to use a different cef_server_handler_t instance
// for each cef_server_t::CreateServer call to avoid thread safety issues in the
// cef_server_handler_t implementation.
///
typedef struct _cef_server_handler_t {
  ///
  // Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  // Called when |server| is created. If the server was started successfully
  // then cef_server_t::IsRunning will return true (1). The server will continue
  // running until cef_server_t::Shutdown is called, after which time
  // OnServerDestroyed will be called. If the server failed to start then
  // OnServerDestroyed will be called immediately after this function returns.
  ///
  void(CEF_CALLBACK* on_server_created)(struct _cef_server_handler_t* self,
                                        struct _cef_server_t* server);

  ///
  // Called when |server| is destroyed. The server thread will be stopped after
  // this function returns. The client should release any references to |server|
  // when this function is called. See OnServerCreated documentation for a
  // description of server lifespan.
  ///
  void(CEF_CALLBACK* on_server_destroyed)(struct _cef_server_handler_t* self,
                                          struct _cef_server_t* server);

  ///
  // Called when a client connects to |server|. |connection_id| uniquely
  // identifies the connection. Each call to this function will have a matching
  // call to OnClientDisconnected.
  ///
  void(CEF_CALLBACK* on_client_connected)(struct _cef_server_handler_t* self,
                                          struct _cef_server_t* server,
                                          int connection_id);

  ///
  // Called when a client disconnects from |server|. |connection_id| uniquely
  // identifies the connection. The client should release any data associated
  // with |connection_id| when this function is called and |connection_id|
  // should no longer be passed to cef_server_t functions. Disconnects can
  // originate from either the client or the server. For example, the server
  // will disconnect automatically after a cef_server_t::SendHttpXXXResponse
  // function is called.
  ///
  void(CEF_CALLBACK* on_client_disconnected)(struct _cef_server_handler_t* self,
                                             struct _cef_server_t* server,
                                             int connection_id);

  ///
  // Called when |server| receives an HTTP request. |connection_id| uniquely
  // identifies the connection, |client_address| is the requesting IPv4 or IPv6
  // client address including port number, and |request| contains the request
  // contents (URL, function, headers and optional POST data). Call cef_server_t
  // functions either synchronously or asynchronusly to send a response.
  ///
  void(CEF_CALLBACK* on_http_request)(struct _cef_server_handler_t* self,
                                      struct _cef_server_t* server,
                                      int connection_id,
                                      const cef_string_t* client_address,
                                      struct _cef_request_t* request);

  ///
  // Called when |server| receives a WebSocket request. |connection_id| uniquely
  // identifies the connection, |client_address| is the requesting IPv4 or IPv6
  // client address including port number, and |request| contains the request
  // contents (URL, function, headers and optional POST data). Execute
  // |callback| either synchronously or asynchronously to accept or decline the
  // WebSocket connection. If the request is accepted then OnWebSocketConnected
  // will be called after the WebSocket has connected and incoming messages will
  // be delivered to the OnWebSocketMessage callback. If the request is declined
  // then the client will be disconnected and OnClientDisconnected will be
  // called. Call the cef_server_t::SendWebSocketMessage function after
  // receiving the OnWebSocketConnected callback to respond with WebSocket
  // messages.
  ///
  void(CEF_CALLBACK* on_web_socket_request)(struct _cef_server_handler_t* self,
                                            struct _cef_server_t* server,
                                            int connection_id,
                                            const cef_string_t* client_address,
                                            struct _cef_request_t* request,
                                            struct _cef_callback_t* callback);

  ///
  // Called after the client has accepted the WebSocket connection for |server|
  // and |connection_id| via the OnWebSocketRequest callback. See
  // OnWebSocketRequest documentation for intended usage.
  ///
  void(CEF_CALLBACK* on_web_socket_connected)(
      struct _cef_server_handler_t* self,
      struct _cef_server_t* server,
      int connection_id);

  ///
  // Called when |server| receives an WebSocket message. |connection_id|
  // uniquely identifies the connection, |data| is the message content and
  // |data_size| is the size of |data| in bytes. Do not keep a reference to
  // |data| outside of this function. See OnWebSocketRequest documentation for
  // intended usage.
  ///
  void(CEF_CALLBACK* on_web_socket_message)(struct _cef_server_handler_t* self,
                                            struct _cef_server_t* server,
                                            int connection_id,
                                            const void* data,
                                            size_t data_size);
} cef_server_handler_t;

#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_CEF_SERVER_CAPI_H_
