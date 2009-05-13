void transport_open (Transport *tpt);
void transport_close (Transport *tpt);
void transport_accept (Transport *tpt, Transport *atpt);
void transport_read_buffer (Transport *tpt, const u8 *buffer, int length);
void transport_write_buffer (Transport *tpt, const u8 *buffer, int length);
int transport_readable (Transport *tpt);
void transport_open_listener(Transport *tpt, int port);


int transport_open_connection(lua_State *L, Handle *handle);

static void transport_connect (Transport *tpt, u32 ip_address, u16 ip_port);
static void transport_bind (Transport *tpt, u32 ip_address, u16 ip_port);
static void transport_listen (Transport *tpt, int maxcon);