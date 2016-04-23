// Create directory.

#include <cassert>
#include <iostream>
#include <uv.h>

using namespace std;

void mkdir(const char* name)
{
    int r;
    uv_fs_t req;
    r = uv_fs_mkdir(NULL, &req, name, 0755, NULL);
    assert(r == 0 || r == UV_EEXIST);
    uv_fs_req_cleanup(&req);
}

int main(int argc, char ** argv)
{
    mkdir("temp");
    return 0;
}

