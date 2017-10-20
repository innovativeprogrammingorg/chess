#ifndef CONNECTION_H
#define CONNECTION_H

#include "../chess.h"

int create_socket();
void handle_connection();
void connection_listener();
int conn_response(libwebsock_client_state *state, libwebsock_message *msg);
int on_new_connection(libwebsock_client_state *state);
int on_close(libwebsock_client_state *state);

#endif