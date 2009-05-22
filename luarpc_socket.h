void transport_close (Transport *tpt);

/*   */
void transport_accept (Transport *tpt, Transport *atpt);

/*    */
void transport_read_buffer (Transport *tpt, const u8 *buffer, int length);
void transport_write_buffer (Transport *tpt, const u8 *buffer, int length);
int transport_readable (Transport *tpt);

void transport_open_listener(Transport *tpt, int port);
int transport_open_connection(lua_State *L, Handle *handle);
