// idling.

#include <iostream>
#include <uv.h>

using namespace std;

static int64_t g_counter = 0;

void wait_for_a_while(uv_idle_t* handle)
{
    g_counter++;

    if (g_counter >= 10e6) {
        uv_idle_stop(handle);
    }
}

int main(int argc, char ** argv)
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

