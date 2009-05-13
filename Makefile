##############################################################################
# Lua-RPC library, Copyright (C) 2001 Russell L. Smith. All rights reserved. #
#   Email: russ@q12.org   Web: www.q12.org                                   #
# For documentation, see http://www.q12.org/lua. For the license agreement,  #
# see the file LICENSE that comes with this distribution.                    #
##############################################################################

# the path to lua
#LUA=/the/path/to/lua
#LUA=/home/russ/1/proj/ntest/ntest
LUA=/Users/jsnyder/Sources/lua-5.1.4
LUAINC=$(LUA)/include
LUALIB=$(LUA)/lib

LIBTOOL=libtool --tag=CC --silent

# compiler, arguments and libs for GCC under unix
CC=gcc -Wall
CFLAGS=-ansi -pedantic -g
LIB=-llua -lm

# compiler, arguments and libs for GCC under windows
#CC=gcc -Wall
#CFLAGS=-DWIN32
#LIB=-llua -llualib -lwsock32 -lm

##############################################################################
# don't change anything below this line

all: luarpc.o rpctest

rpctest: rpctest.c luarpc.o
	$(CC) $(CFLAGS) -I$(LUAINC) -o $@ rpctest.c luarpc.o \
	-L$(LUALIB) $(LIB)

luarpc.o: luarpc.c luarpc.h
	$(CC) -c $(CFLAGS) -I$(LUAINC) luarpc.c

module: luarpc.c luarpc_socket.c
	$(LIBTOOL) --mode=compile cc -c luarpc.c luarpc_socket.c
	$(LIBTOOL) --mode=link cc -rpath $(LUALIB) -o libluarpc.la luarpc.lo
	mv .libs/libluarpc.0.dylib luarpc.so

clean-unix:
	-rm -f *~ *.o *.obj a.out rpctest rpctest.exe core

clean-win:
	-del   *~ *.o *.obj a.out rpctest rpctest.exe core
