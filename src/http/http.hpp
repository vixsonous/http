#ifndef HTTP_SERVER_DEFINITION
#define HTTP_SERVER_DEFINITION

void build_http_response(const char*, const char*, char*, size_t*);
int init_server(int port);
void *handle_client(void *args);

#endif