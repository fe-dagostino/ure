/**************************************************************************************************
 * 
 * Copyright 2022 https://github.com/fe-dagostino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
 * permit persons to whom the Software is furnished to do so, subject to the following 
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies 
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 *
 *************************************************************************************************/

#include "ure_websocket.h"

#include <core/utils.h>
#include <future>

#ifdef _USE_WEBSOCKETS
# include <libwebsockets.h>
#endif

namespace ure {

#ifdef _USE_WEBSOCKETS

/* defined in user_application_generic.cpp  */
extern struct lws_context*     g_context;


static int callback_text  (struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
static int callback_binary(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

struct lws_protocols    g_protocols[] = {
  { "text"  , callback_text  , 0, 0, 0, nullptr, 0 },
  { "binary", callback_binary, 0, 0, 0, nullptr, 0 },
  LWS_PROTOCOL_LIST_TERM
};

/***/
struct resource_t
{
  resource_t( WebSocket* ws, websocket_options::ws_mode_t mode )
    : instance(ws)
  {
  }

  void init( int use_ssl )
  {
    ws      = nullptr;
  }

  WebSocket*                       instance;
  struct lws *                     ws;
};

/* Callback function for the WebSocket protocol "text" */
static int callback_text(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
  switch (reason)
  {
    case LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED:
    {
    }; break;

    case LWS_CALLBACK_CLIENT_ESTABLISHED:
    {
    }; break;

    case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP:
    {
    }; break;

    case LWS_CALLBACK_CLIENT_RECEIVE:
    {
    };break;
    
    case LWS_CALLBACK_CLIENT_WRITEABLE:
    {
    }; break;

    case LWS_CALLBACK_CLIENT_CLOSED:
    {
    }; break;

    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
    {
    }; break;

    default:
    {
    }; break;

  }
  return 0;
}

/* Callback function for the WebSocket protocol "binary" */
static int callback_binary(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
  switch (reason)
  {
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
    {
    }; break;

    case LWS_CALLBACK_CLIENT_WRITEABLE:
    {
    }; break;

    case LWS_CALLBACK_CLIENT_CLOSED:
    {
    }; break;

    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
    {
    }; break;

    default:
    {
    }; break;
  }

  return 0;
}


bool_t WebSocket::open() noexcept(true)
{
  if ( m_data != nullptr )
    return false;

  m_data = new(std::nothrow)resource_t( this, options().mode() );
  if ( m_data == nullptr )
    return false;

  std::string  uri = options().url().data();
  int          use_ssl = 0;
  int          ietf_version = -1;
  int          test_post = 0;
  const char*  pprot;
  const char*  ppath;
  std::string  path;

  struct lws_client_connect_info ccinfo;
  memset(&ccinfo, 0, sizeof(ccinfo));
  /* set user data */
  ccinfo.userdata = m_data;

  if ( lws_parse_uri( uri.data(), &pprot, &ccinfo.address, &ccinfo.port, &ppath) != 0 )
  {
  }

  if ( ppath[0] != '/' )
  {
    path = core::utils::format( "/%s", ppath );
  }
  else
  {
    path = ppath;
  }

  static_cast<resource_t*>(m_data)->init( use_ssl );

  ccinfo.context        = g_context;
	ccinfo.ssl_connection = use_ssl;
	ccinfo.host           = ccinfo.address;
	ccinfo.origin         = ccinfo.address;
	ccinfo.ietf_version_or_minus_one = ietf_version;

	if (!strcmp(pprot, "http") || !strcmp(pprot, "https"))
  {
		lwsl_notice("using %s mode (non-ws)\n", pprot);
		if (test_post)
    {
			ccinfo.method = "POST";
			lwsl_notice("POST mode\n");
		}
		else
    { ccinfo.method = "GET"; }
		//do_ws = 0;
	}
  else
  {
		if (!strcmp(pprot, "raw"))
    {
			ccinfo.method = "RAW";
			ccinfo.protocol = "lws-test-raw-client";
			lwsl_notice("using RAW mode connection\n");
			//do_ws = 0;
		} 
    else
		{	lwsl_notice("using %s mode (ws)\n", pprot); }
  }

  static_cast<resource_t*>(m_data)->ws = lws_client_connect_via_info(&ccinfo);
  if ( static_cast<resource_t*>(m_data)->ws == nullptr )
  { 
    return false;
  }

  return true;
}

bool_t WebSocket::send( const uint8_t* data, uint32_t length ) noexcept(true)
{
  if ( static_cast<resource_t*>(m_data)->ws == nullptr )
    return false;

  return ( lws_write( static_cast<resource_t*>(m_data)->ws,
           const_cast<typename std::remove_const<uint8_t>::type *>(data), 
           length, static_cast<lws_write_protocol>(options().mode()) ) != -1 );
}

bool_t WebSocket::close() noexcept(true)
{
  if ( m_data == nullptr )
    return false;


  return true;
}

void WebSocket::release() noexcept(true)
{
  if ( m_data != nullptr )
  {
    delete static_cast<resource_t*>(m_data);
    m_data = nullptr;
  }
}

#endif /* _USE_WEBSOCKETS */

} /* namespace ure */
