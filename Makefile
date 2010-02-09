##############################################################################
# Lua-RPC library, Copyright (C) 2001 Russell L. Smith. All rights reserved. #
#   Email: russ@q12.org   Web: www.q12.org                                   #
# For documentation, see http://www.q12.org/lua. For the license agreement,  #
# see the file LICENSE that comes with this distribution.                    #
##############################################################################
 
# the path to lua
LUAINC=/usr/include/lua5.1/
LUALIB=/usr/local/lib
 
LIBTOOL=libtool --tag=CC --quiet
UNAME := $(shell uname)
 
# compiler, arguments and libs for GCC under unix
CFLAGS=-ansi -fpic -std=c99 -pedantic -g -DLUARPC_STANDALONE -DBUILD_RPC -DLUARPC_ENABLE_SOCKET
 
# compiler, arguments and libs for GCC under windows
#CC=gcc -Wall
#CFLAGS=-DWIN32
#LIB=-llua -llualib -lwsock32 -lm

##############################################################################
# don't change anything below this line
 
ifeq ($(UNAME), Linux)
all: linux
endif
ifeq ($(UNAME), Darwin)
all: osx
endif
 
core: luarpc.c luarpc_socket.c
	gcc $(CFLAGS) -I$(LUAINC) -c -o luarpc.o luarpc.c
	gcc $(CFLAGS) -I$(LUAINC) -c -o luarpc_serial.o luarpc_serial.c
	gcc $(CFLAGS) -I$(LUAINC) -c -o luarpc_socket.o luarpc_socket.c
	gcc $(CFLAGS) -I$(LUAINC) -c -o serial_posix.o serial_posix.c

linux: core
	gcc -O -shared -fpic -o rpc.so luarpc.o luarpc_socket.o luarpc_serial.o serial_posix.o

osx: core
	gcc -O -fpic -dynamiclib -undefined dynamic_lookup -o rpc.so luarpc.o luarpc_socket.o luarpc_serial.o serial_posix.o 
 
libtool: luarpc.c luarpc_socket.c
	$(LIBTOOL) --mode=compile cc $(CFLAGS) -I$(LUAINC) -c luarpc.c
	$(LIBTOOL) --mode=compile cc $(CFLAGS) -I$(LUAINC) -c luarpc_serial.c
	$(LIBTOOL) --mode=compile cc $(CFLAGS) -I$(LUAINC) -c serial_posix.c
	$(LIBTOOL) --mode=link cc -module -rpath $(LUALIB) -o libluarpc.la luarpc.lo serial_posix.lo luarpc_serial.lo
	(mv .libs/libluarpc.so.0.0.0 rpc.so || mv .libs/libluarpc.0.so rpc.so)
 
clean:
	-rm -rf *~ *.o *.lo *.la *.obj a.out .libs core
