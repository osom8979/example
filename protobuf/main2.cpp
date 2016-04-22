
#include <cstdio>

#include <iostream>
#include <vector>
#include <limits>

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include <test.pb.h>

using namespace std;

int main(int argc, char ** argv)
{
    example::DemoHeader h1;
    h1.set_id(1);
    h1.set_size(std::numeric_limits<int>::max());
    h1.set_ext1(3);

    int h1_size = h1.ByteSize();

    vector<char> h1_vector;
    h1_vector.resize(h1_size);

    // Serialize.
    google::protobuf::io::ArrayOutputStream os1(&h1_vector[0], h1_size);
    h1.SerializeToZeroCopyStream(&os1);

    example::DemoHeader h1_output;

    // De-serialization.
    google::protobuf::io::ArrayInputStream is1(&h1_vector[0], h1_size);
    h1_output.ParseFromZeroCopyStream(&is1);

    printf("H1[%d](%d,%d,%d,%d)\n", h1_size, h1_output.id(), h1_output.size(), h1_output.ext1(), h1_output.ext2());

    for (auto cursor : h1_vector) {
        cout << bitset<8>(cursor) << endl;
    }

    return 0;
}

