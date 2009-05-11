/* Represent doubles as byte string */
union DoubleBytes {
  double d;
  u8 b[8];
};

/* Transport Connection Structure */
struct _Transport {
  SOCKTYPE fd;      /* INVALID_TRANSPORT if socket is closed */
};
typedef struct _Transport Transport;

#define INVALID_TRANSPORT (-1)

#define TRANSPORT_VERIFY_OPEN \
  if (tpt->fd == INVALID_TRANSPORT) THROW (ERR_CLOSED);

static void transport_open (Transport *tpt);

static void transport_close (Transport *tpt);
static void transport_connect (Transport *tpt, u32 ip_address, u16 ip_port);
static void transport_bind (Transport *tpt, u32 ip_address, u16 ip_port);
static void transport_listen (Transport *tpt, int maxcon);
static void transport_accept (Transport *tpt, Socket *asock);
static int transport_readable (Transport *tpt);

/*  Key functions for reading and writing data to the transport  */
static void transport_read_buffer (Transport *tpt, const u8 *buffer, int length);
static void transport_write_buffer (Transport *tpt, const u8 *buffer, int length);





/* Generic Universal Functions */
static void transport_init (Transport *tpt);
static int transport_is_open (Transport *tpt);

/* 
	Wrapper functions provided by the luarpc_transport transport abstraction.
	
	These use transport_read_buffer & transport_write_buffer functions to
	transmit bytes over the link.
*/
static void transport_read_string (Transport *tpt, const char *buffer, int length);
static void transport_write_string (Transport *tpt, const char *buffer, int length);
static u8 transport_read_u8 (Transport *tpt);
static void transport_write_u8 (Transport *tpt, u8 x);
static u32 transport_read_u32 (Transport *tpt);
static void transport_write_u32 (Transport *tpt, u32 x);
static double transport_read_double (Transport *tpt);
static void transport_write_double (Transport *tpt, double x);


