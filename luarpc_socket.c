/*****************************************************************************
* Lua-RPC library, Copyright (C) 2001 Russell L. Smith. All rights reserved. *
*   Email: russ@q12.org   Web: www.q12.org                                   *
* For documentation, see http://www.q12.org/lua. For the license agreement,  *
* see the file LICENSE that comes with this distribution.                    *
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>

#ifdef WIN32 /* BEGIN NEEDED INCLUDES FOR WIN32 W/ SOCKETS */

#include <windows.h>

#else /* BEGIN NEEDED INCLUDES FOR UNIX W/ SOCKETS */

#include <string.h>
#include <errno.h>
#include <alloca.h>
#include <signal.h>

/* for sockets */
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#endif /* END NEEDED INCLUDES W/ SOCKETS */

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "config.h"

/****************************************************************************/
/* parameters */

#define MAXCON 10 /* maximum number of waiting server connections */

/* a kind of silly way to get the maximum int, but oh well ... */
#define MAXINT ((int)((((unsigned int)(-1)) << 1) >> 1))

/****************************************************************************/
/* error handling */

/* allow special handling for GCC compiler */
#ifdef __GNUC__
#define DOGCC(x) x
#else
#define DOGCC(x) /* */
#endif


/* assertions */

#ifndef NDEBUG
#ifdef __GNUC__
#define MYASSERT(a) if (!(a)) debug ( \
  "assertion \"" #a "\" failed in %s() [%s]",__FUNCTION__,__FILE__);
#else
#define MYASSERT(a) if (!(a)) debug ( \
  "assertion \"" #a "\" failed in %s:%d",__FILE__,__LINE__);
#endif
#else
#define MYASSERT(a) ;
#endif


static void errorMessage (const char *msg, va_list ap)
{
  fflush (stdout);
  fflush (stderr);
  fprintf (stderr,"\nError: ");
  vfprintf (stderr,msg,ap);
  fprintf (stderr,"\n\n");
  fflush (stderr);
}


DOGCC(static void panic (const char *msg, ...)
      __attribute__ ((noreturn,unused));)
static void panic (const char *msg, ...)
{
  va_list ap;
  va_start (ap,msg);
  errorMessage (msg,ap);
  exit (1);
}


DOGCC(static void debug (const char *msg, ...)
      __attribute__ ((noreturn,unused));)
static void debug (const char *msg, ...)
{
  va_list ap;
  va_start (ap,msg);
  errorMessage (msg,ap);
  abort();
}

/****************************************************************************/
/* handle the differences between winsock and unix */

#ifdef WIN32  /*  BEGIN WIN32 SOCKET SETUP  */

#define close closesocket
#define read(fd,buf,len) recv ((fd),(buf),(len),0)
#define write(fd,buf,len) send ((fd),(buf),(len),0)
#define SOCKTYPE SOCKET
#define sock_errno (WSAGetLastError())


/* check some assumptions */
#if SOCKET_ERROR >= 0
#error need SOCKET_ERROR < 0
#endif


/* this should be called before any network operations */

static void net_startup()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;

  // startup WinSock version 2
  wVersionRequested = MAKEWORD(2,0);
  err = WSAStartup (wVersionRequested,&wsaData);
  if (err != 0) panic ("could not start winsock");

  // confirm that the WinSock DLL supports 2.0. note that if the DLL
  // supports versions greater than 2.0 in addition to 2.0, it will
  // still return 2.0 in wVersion since that is the version we requested.
  if (LOBYTE (wsaData.wVersion ) != 2 ||
      HIBYTE(wsaData.wVersion) != 0 ) {
    WSACleanup();
    panic ("bad winsock version (< 2)");
  }
}


/* WinSock does not seem to have a strerror() style function, so here it is. */

const char * transport_strerror (int n)
{
  switch (n) {
  case WSAEACCES: return "Permission denied.";
  case WSAEADDRINUSE: return "Address already in use.";
  case WSAEADDRNOTAVAIL: return "Cannot assign requested address.";
  case WSAEAFNOSUPPORT:
    return "Address family not supported by protocol family.";
  case WSAEALREADY: return "Operation already in progress.";
  case WSAECONNABORTED: return "Software caused connection abort.";
  case WSAECONNREFUSED: return "Connection refused.";
  case WSAECONNRESET: return "Connection reset by peer.";
  case WSAEDESTADDRREQ: return "Destination address required.";
  case WSAEFAULT: return "Bad address.";
  case WSAEHOSTDOWN: return "Host is down.";
  case WSAEHOSTUNREACH: return "No route to host.";
  case WSAEINPROGRESS: return "Operation now in progress.";
  case WSAEINTR: return "Interrupted function call.";
  case WSAEINVAL: return "Invalid argument.";
  case WSAEISCONN: return "Socket is already connected.";
  case WSAEMFILE: return "Too many open files.";
  case WSAEMSGSIZE: return "Message too long.";
  case WSAENETDOWN: return "Network is down.";
  case WSAENETRESET: return "Network dropped connection on reset.";
  case WSAENETUNREACH: return "Network is unreachable.";
  case WSAENOBUFS: return "No buffer space available.";
  case WSAENOPROTOOPT: return "Bad protocol option.";
  case WSAENOTCONN: return "Socket is not connected.";
  case WSAENOTSOCK: return "Socket operation on nonsocket.";
  case WSAEOPNOTSUPP: return "Operation not supported.";
  case WSAEPFNOSUPPORT: return "Protocol family not supported.";
  case WSAEPROCLIM: return "Too many processes.";
  case WSAEPROTONOSUPPORT: return "Protocol not supported.";
  case WSAEPROTOTYPE: return "Protocol wrong type for socket.";
  case WSAESHUTDOWN: return "Cannot send after socket shutdown.";
  case WSAESOCKTNOSUPPORT: return "Socket type not supported.";
  case WSAETIMEDOUT: return "Connection timed out.";
  case WSAEWOULDBLOCK: return "Resource temporarily unavailable.";
  case WSAHOST_NOT_FOUND: return "Host not found.";
  case WSANOTINITIALISED: return "Successful WSAStartup not yet performed.";
  case WSANO_DATA: return "Valid name, no data record of requested type.";
  case WSANO_RECOVERY: return "This is a nonrecoverable error.";
  case WSASYSNOTREADY: return "Network subsystem is unavailable.";
  case WSATRY_AGAIN: return "Nonauthoritative host not found.";
  case WSAVERNOTSUPPORTED: return "Winsock.dll version out of range.";
  case WSAEDISCON: return "Graceful shutdown in progress.";
  default: return "Unknown error.";

  /* OS dependent error numbers? */
  /*
  case WSATYPE_NOT_FOUND: return "Class type not found.";
  case WSA_INVALID_HANDLE: return "Specified event object handle is invalid.";
  case WSA_INVALID_PARAMETER: return "One or more parameters are invalid.";
  case WSAINVALIDPROCTABLE:
    return "Invalid procedure table from service provider.";
  case WSAINVALIDPROVIDER: return "Invalid service provider version number.";
  case WSA_IO_INCOMPLETE:
    return "Overlapped I/O event object not in signaled state.";
  case WSA_IO_PENDING: return "Overlapped operations will complete later.";
  case WSA_NOT_ENOUGH_MEMORY: return "Insufficient memory available.";
  case WSAPROVIDERFAILEDINIT:
    return "Unable to initialize a service provider.";
  case WSASYSCALLFAILURE: return "System call failure.";
  case WSA_OPERATION_ABORTED: return "Overlapped operation aborted.";
  */
  }
}

#else /* BEGIN UNIX SOCKET SETUP  */

#define SOCKTYPE int
#define net_startup() ;
#define sock_errno errno
#define transport_strerror strerror

#endif /* END UNIX SOCKET SETUP */

/****************************************************************************/
/* socket reading and writing stuff.
 * the socket functions throw exceptions if there are errors, so you must call
 * them from within a TRY block.
 */

/* open a socket */

static void socket_open (Socket *sock)
{
  sock->fd = socket (PF_INET,SOCK_STREAM,IPPROTO_TCP);
  if (sock->fd == INVALID_SOCKET) THROW (sock_errno);
}

/* close a socket */

static void socket_close (Socket *sock)
{
  if (sock->fd != INVALID_SOCKET) close (sock->fd);
  sock->fd = INVALID_SOCKET;
}


/* connect the socket to a host */

static void socket_connect (Socket *sock, u32 ip_address, u16 ip_port)
{
  struct sockaddr_in myname;
  TRANSPORT_VERIFY_OPEN;
  myname.sin_family = AF_INET;
  myname.sin_port = htons (ip_port);
  myname.sin_addr.s_addr = htonl (ip_address);
  if (connect (sock->fd, (struct sockaddr *) &myname, sizeof (myname)) != 0)
    THROW (sock_errno);
}


/* bind the socket to a given address/port. the address can be INADDR_ANY. */

static void socket_bind (Socket *sock, u32 ip_address, u16 ip_port)
{
  struct sockaddr_in myname;
  TRANSPORT_VERIFY_OPEN;
  myname.sin_family = AF_INET;
  myname.sin_port = htons (ip_port);
  myname.sin_addr.s_addr = htonl (ip_address);
  if (bind (sock->fd, (struct sockaddr *) &myname, sizeof (myname)) != 0)
    THROW (sock_errno);
}


/* listen for incoming connections, with up to `maxcon' connections
 * queued up.
 */

static void socket_listen (Socket *sock, int maxcon)
{
  TRANSPORT_VERIFY_OPEN;
  if (listen (sock->fd,maxcon) != 0) THROW (sock_errno);
}


/* accept an incoming connection, initializing `asock' with the new connection.
 */

static void socket_accept (Socket *sock, Socket *asock)
{
  struct sockaddr_in clientname;
  size_t namesize;
  TRANSPORT_VERIFY_OPEN;
  namesize = sizeof (clientname);
  asock->fd = accept (sock->fd, (struct sockaddr*) &clientname, &namesize);
  if (asock->fd == INVALID_SOCKET) THROW (sock_errno);
}


/* read from the socket into a buffer */

static void socket_read_buffer (Socket *sock, const u8 *buffer, int length)
{
  TRANSPORT_VERIFY_OPEN;
  while (length > 0) {
    int n = read (sock->fd,(void*) buffer,length);
    if (n == 0) THROW (ERR_EOF);
    if (n < 0) THROW (sock_errno);
    buffer += n;
    length -= n;
  }
}

/* write a buffer to the socket */

static void socket_write_buffer (Socket *sock, const u8 *buffer, int length)
{
  int n;
  TRANSPORT_VERIFY_OPEN;
  n = write (sock->fd,buffer,length);
  if (n != length) THROW (sock_errno);
}

/* see if there is any data to read from a socket, without actually reading
 * it. return 1 if data is available, on 0 if not. if this is a listening
 * socket this returns 1 if a connection is available or 0 if not.
 */

static int socket_readable (Socket *sock)
{
  fd_set set;
  struct timeval tv;
  int ret;
  if (sock->fd == INVALID_SOCKET) return 0;
  FD_ZERO (&set);
  FD_SET (sock->fd,&set);
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  ret = select (sock->fd + 1,&set,0,0,&tv);
  return (ret > 0);
}
 