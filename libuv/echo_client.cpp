// TCP echo client.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#define DEFAULT_PORT     7000
#define DEFAULT_BACKLOG  128

uv_loop_t * loop;
uv_tcp_t * tcp_client;
uv_write_t write_request;

char ECHO_STRING[] = "HELLO";
uv_buf_t buf = { ECHO_STRING, 4 };

void write_cb(uv_write_t * req, int status)
{
    uv_close((uv_handle_t*) req->handle, NULL);
}

void on_connect(uv_connect_t * req, int status)
{
    if (status < 0) {
        fprintf(stderr, "Connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_write(&write_request
           , (uv_stream_t *) tcp_client
           , &buf
           , 1
           , write_cb);
}

int main()
{
    loop = uv_default_loop();

    tcp_client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, tcp_client);

    uv_connect_t * connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    struct sockaddr_in dest;
    uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &dest);

    int r = uv_tcp_connect(connect, tcp_client, (const struct sockaddr*)&dest, on_connect);

    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }

    return uv_run(loop, UV_RUN_DEFAULT);
}

