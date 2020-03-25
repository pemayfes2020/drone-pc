#include "socket.hpp"

#include <iostream>

struct DataType {
    int32_t x;
    int32_t y;
    int32_t z;
};

int main()
{
    UnixSocket::Client client("/tmp/ar.sock");

    auto data = client.read<DataType>();
    std::cout << data.x << ' ' << data.y << ' ' << data.z << std::endl;
    client.write<DataType>(DataType{1, 2, 3});

    return 0;
}
