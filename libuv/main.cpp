
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <uv.h>

using namespace std;

// First Event loop.
int event_loop()
{
    uv_loop_t * loop = (uv_loop_t *) malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    printf("Now quitting.\n");
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);

    return 0;
}

int main(int argc, char ** argv)
{
    return event_loop();
}

