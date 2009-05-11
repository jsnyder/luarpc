/* initialize a transport struct */

static void transport_init (Transport *tpt)
{
  sock->fd = INVALID_TRANSPORT;
}

/* see if a socket is open */

static int transport_is_open (Transport *tpt)
{
  return (tpt->fd != INVALID_TRANSPORT);
}


/* read from the transport into a string buffer. */

static void transport_read_string (Transport *tpt, const char *buffer, int length)
{
  transport_read_buffer (tpt,(u8*) buffer,length);
}


/* write a string buffer to the transport */

static void transport_write_string (Transport *tpt, const char *buffer, int length)
{
  transport_write_buffer (tpt,(u8*) buffer,length);
}


/* read a u8 from the transport */

static u8 transport_read_u8 (Transport *tpt)
{
  u8 b;
  TRANSPORT_VERIFY_OPEN;
  transport_read_buffer (tpt,&b,1);
  return b;
}


/* write a u8 to the transport */

static void transport_write_u8 (Transport *tpt, u8 x)
{
  int n;
	u8 b;
  TRANSPORT_VERIFY_OPEN;
  n = write (tpt->fd,&x,1);
	transport_write_buffer (tpt,&b,1)
  /* FIXME: if (n != 1) THROW (tpt_errno); */
}


/* read a u32 from the transport */

static u32 transport_read_u32 (Transport *tpt)
{
  u8 b[4];
  u32 i;
  TRANSPORT_VERIFY_OPEN;
  transport_read_buffer (tpt,b,4);
  i = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
  return i;
}


/* write a u32 to the transport */

static void transport_write_u32 (Transport *tpt, u32 x)
{
  u8 b[4];
  int n;
  TRANSPORT_VERIFY_OPEN;
  b[0] = x >> 24;
  b[1] = x >> 16;
  b[2] = x >> 8;
  b[3] = x;
  transport_write_buffer (tpt,b,4)
}


/* read a double from the transport */

static double transport_read_double (Transport *tpt)
{
  union DoubleBytes double_bytes;
  TRANSPORT_VERIFY_OPEN;
  /* @@@ handle endianness */
  transport_read_buffer (tpt,double_bytes.b,8);
  return double_bytes.d;
}


/* write a double to the transport */

static void transport_write_double (Transport *tpt, double x)
{
  int n;
  union DoubleBytes double_bytes;
  TRANSPORT_VERIFY_OPEN;
  /* @@@ handle endianness */
  double_bytes.d = x;
	transport_read_buffer (tpt,double_bytes.b,8);
}