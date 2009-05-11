/*****************************************************************************
* Lua-RPC library, Copyright (C) 2001 Russell L. Smith. All rights reserved. *
*   Email: russ@q12.org   Web: www.q12.org                                   *
* For documentation, see http://www.q12.org/lua. For the license agreement,  *
* see the file LICENSE that comes with this distribution.                    *
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "lua.h"
#include "lualib.h"
#include "luarpc.h"
#include "lauxlib.h"

/****************************************************************************/
/* error handling */

static void errorMessage (char *msg, va_list ap)
{
  fflush (stdout);
  fflush (stderr);
  fprintf (stderr,"\nError: ");
  vfprintf (stderr,msg,ap);
  fprintf (stderr,"\n\n");
  fflush (stderr);
}


static void panic (char *msg, ...)
{
  va_list ap;
  va_start (ap,msg);
  errorMessage (msg,ap);
  exit (1);
}

/****************************************************************************/
/* main */

int main (int argc, char *argv[])
{
  const char *filename;
  lua_State *L;
  int ret;

#ifndef WIN32
  signal (SIGPIPE,SIG_IGN);		/* @@@ move this into library? */
#endif

  if (argc != 2) panic ("usage: %s <file.lua>",argv[0]);
  filename = argv[1];
		
  /* start lua and connect to all the standard libraries */
  L = lua_open (); /* used to pass 4096 for stack size */
	luaL_openlibs(L);
  luaopen_luarpc (L);

  ret = luaL_dofile (L,filename);
	if (ret) {
		printf("%s\n", lua_tostring(L, -1));
	}

  return 0;
}
