
#include <cstdio>

#include <iostream>
#include <vector>

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include <test.pb.h>

using namespace std;

int main(int argc, char ** argv)
{
    example::DemoHeader h1;
    h1.set_id(1);
    h1.set_size(2);
    h1.set_ext1(3);

    //cout << "ID: "   << h1.id() << endl;
    //cout << "SIZE: " << h1.size() << endl;
    //cout << "EXT1: " << h1.ext1() << endl;

    example::DemoHeader h2;
    h2.set_id(11);
    h2.set_size(22);
    h2.set_ext1(33);
    h2.set_ext2(44);

    int h1_size = h1.ByteSize();
    int h2_size = h2.ByteSize();

    vector<char> h1_vector;
    vector<char> h2_vector;
    h1_vector.resize(h1_size);
    h2_vector.resize(h2_size);

    // Serialize.
    google::protobuf::io::ArrayOutputStream os1(&h1_vector[0], h1_size);
    h1.SerializeToZeroCopyStream(&os1);
    google::protobuf::io::ArrayOutputStream os2(&h2_vector[0], h2_size);
    h2.SerializeToZeroCopyStream(&os2);

    example::DemoHeader h1_output;
    example::DemoHeader h2_output;

    // De-serialization.
    google::protobuf::io::ArrayInputStream is1(&h1_vector[0], h1_size);
    h1_output.ParseFromZeroCopyStream(&is1);
    google::protobuf::io::ArrayInputStream is2(&h2_vector[0], h2_size);
    h2_output.ParseFromZeroCopyStream(&is2);

    printf("H1[%d](%d,%d,%d,%d)\n", h1_size, h1_output.id(), h1_output.size(), h1_output.ext1(), h1_output.ext2());
    printf("H2[%d](%d,%d,%d,%d)\n", h2_size, h2_output.id(), h2_output.size(), h2_output.ext1(), h2_output.ext2());

    return 0;
}

