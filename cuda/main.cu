
#include <iostream>

using namespace std;

int print_cuda_version()
{
    int count = 0;

    if (cudaSuccess != cudaGetDeviceCount(&count)) {
        return -1;
    }

    if (count == 0) {
        return -1;
    }

    for (int device = 0; device < count; ++device) {
        cudaDeviceProp prop;
        if (cudaSuccess == cudaGetDeviceProperties(&prop, device)) {
            std::cout << prop.major << "." << prop.minor << std::endl;
        }
    }

    return 0;
}

int main(int argc, char ** argv)
{
    return print_cuda_version();
}

