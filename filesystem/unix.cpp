
#include <iostream>
#include <sys/stat.h>

using namespace std;

int main(int argc, char ** argv)
{
    int result = mkdir("testdir", S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
    cout << "mkdir result: " << result << endl;
    return 0;
}

