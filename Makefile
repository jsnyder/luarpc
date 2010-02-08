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
CFLAGS=-ansi -fPIC -std=c99 -pedantic -g -DLUARPC_STANDALONE

# compiler, arguments and libs for GCC under windows
#CC=gcc -Wall
#CFLAGS=-DWIN32
#LIB=-llua -llualib -lwsock32 -lm

##############################################################################
# don't change anything below this line

all: osx

osx: luarpc.c
	gcc $(CFLAGS) -c -o luarpc.o luarpc.c
	gcc $(CFLAGS) -c -o luarpc_serial.o luarpc_serial.c
	gcc $(CFLAGS) -c -o serial_posix.o serial_posix.c
	gcc $(CFLAGS) -c -p luarpc_socket.o luarpc_socket.c
	gcc -O -bundle -undefined dynamic_lookup -fPIC -o rpc.so luarpc.o luarpc_serial.o serial_posix.o luarpc_socket.o

linux: luarpc.c luarpc_socket.c
	$(LIBTOOL) --mode=compile cc $(CFLAGS) -I$(LUAINC) -c luarpc.c
	$(LIBTOOL) --mode=compile cc $(CFLAGS) -I$(LUAINC) -c luarpc_serial.c
	$(LIBTOOL) --mode=compile cc $(CFLAGS) -I$(LUAINC) -c serial_posix.c
	$(LIBTOOL) --mode=link cc -module -rpath $(LUALIB) -o libluarpc.la luarpc.lo serial_posix.lo luarpc_serial.lo
	(mv .libs/libluarpc.so.0.0.0 rpc.so || mv .libs/libluarpc.0.so rpc.so)

clean:
	-rm -rf *~ *.o *.lo *.la *.obj a.out .libs core
