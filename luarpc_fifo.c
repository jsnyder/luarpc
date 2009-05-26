#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "config.h"
#include "luarpc_rpc.h"

#ifdef LUARPC_ENABLE_FIFO

/* Setup Transport */
void transport_init (Transport *tpt)
{
	tpt->wrfd = INVALID_TRANSPORT;
	tpt->rdfd = INVALID_TRANSPORT;
}

/* Open Listener / Server */
void transport_open_listener(lua_State *L, ServerHandle *handle)
{
	
}

/* Open Connection / Client */
int transport_open_connection(lua_State *L, Handle *handle)
{
	
}

/* Accept Connection */
void transport_accept (Transport *tpt, Transport *atpt)
{
	
}

/* Read & Write to Transport */
void transport_read_buffer (Transport *tpt, const u8 *buffer, int length)
{
	
}

void transport_write_buffer (Transport *tpt, const u8 *buffer, int length)
{
	
}

/* Check if data is available on connection without reading:
 		- 1 = data available, 0 = no data available */
int transport_readable (Transport *tpt)
{
	
}

/* Check if transport is open:
		- 1 = connection open, 0 = connection closed */
int transport_is_open (Transport *tpt)
{
	
}

/* Shut down connection */
void transport_close (Transport *tpt)
{
	
}

#endif /* LUARPC_ENABLE_FIFO */
