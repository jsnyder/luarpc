// eLua platform configuration

#ifndef __PLATFORM_CONF_H__
#define __PLATFORM_CONF_H__

// BUILDING FOR ELUA
#ifdef LUA_RPC
#include "auxmods.h"

#define BUILD_RPC
#define LUARPC_ENABLE_SERIAL

#define LUA_PLATFORM_LIBS_ROM \
  _ROM( AUXLIB_RPC, luaopen_rpc, rpc_map )

#endif


// BUILDING FOR STANDALONE MODULE
#ifdef LUARPC_STANDALONE

#define BUILD_RPC

//#define LUARPC_ENABLE_SERIAL
#define LUARPC_ENABLE_SOCKET

#endif

#endif // #ifndef __PLATFORM_CONF_H__
