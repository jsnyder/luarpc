

static void transport_open (Transport *tpt);

static void transport_close (Transport *tpt);
static void transport_connect (Transport *tpt, u32 ip_address, u16 ip_port);
static void transport_bind (Transport *tpt, u32 ip_address, u16 ip_port);
static void transport_listen (Transport *tpt, int maxcon);
static void transport_accept (Transport *tpt, Socket *asock);

/*
	API THAT UNDERLYING TRANSPORT LAYER MUST PROVIDE
*/

/*  Key functions for reading and writing data to the transport  */
static void transport_read_buffer (Transport *tpt, const u8 *buffer, int length);
static void transport_write_buffer (Transport *tpt, const u8 *buffer, int length);
static int transport_readable (Transport *tpt);

/* Generic Universal Functions */
static void transport_init (Transport *tpt);
static int transport_is_open (Transport *tpt);


/*
	CONVENIENCE FUNCTIONS PROVIDED BY GENERIC TRANSPORT
*/

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


