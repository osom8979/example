
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <iostream>

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

static int64_t g_counter = 0;

// idling.
void wait_for_a_while(uv_idle_t* handle)
{
    g_counter++;

    if (g_counter >= 10e6) {
        uv_idle_stop(handle);
    }
}

int idle_event()
{
    uv_idle_t idler;

    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, wait_for_a_while);

    printf("Idling...\n");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    uv_loop_close(uv_default_loop());

    printf("Done.\n");
    return 0;
}

void create_dir(const char* name)
{
    int r;
    uv_fs_t req;
    r = uv_fs_mkdir(NULL, &req, name, 0755, NULL);
    assert(r == 0 || r == UV_EEXIST);
    uv_fs_req_cleanup(&req);
}

int main(int argc, char ** argv)
{
    //return event_loop();
    //return idle_event();
    create_dir("temp");
    return 0;
}

