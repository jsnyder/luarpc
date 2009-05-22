##############################################################################
# Lua-RPC library, Copyright (C) 2001 Russell L. Smith. All rights reserved. #
#   Email: russ@q12.org   Web: www.q12.org                                   #
# For documentation, see http://www.q12.org/lua. For the license agreement,  #
# see the file LICENSE that comes with this distribution.                    #
##############################################################################

# the path to lua
LUAINC=/usr/include/lua5.1/
LUALIB=/usr/lib

LIBTOOL=libtool --tag=CC --quiet

# compiler, arguments and libs for GCC under unix
CFLAGS=-ansi -pedantic -g

# compiler, arguments and libs for GCC under windows
#CC=gcc -Wall
#CFLAGS=-DWIN32
#LIB=-llua -llualib -lwsock32 -lm

##############################################################################
# don't change anything below this line

all: module

module: luarpc.c luarpc_socket.c
	$(LIBTOOL) --mode=compile cc $(CFLAGS) -I$(LUAINC) -c luarpc.c
	$(LIBTOOL) --mode=compile cc $(CFLAGS) -I$(LUAINC) -c luarpc_socket.c
	$(LIBTOOL) --mode=link cc -module -rpath $(LUALIB) -o libluarpc.la luarpc.lo luarpc_socket.lo
	(mv .libs/libluarpc.so.0.0.0 luarpc.so || mv .libs/libluarpc.0.so luarpc.so)


clean-unix:
	-rm -f *~ *.o *.obj a.out rpctest rpctest.exe core

clean-win:
	-del   *~ *.o *.obj a.out rpctest rpctest.exe core
