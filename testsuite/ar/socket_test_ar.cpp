#include "socket.hpp"

#include <iostream>

struct DataType {
    int32_t x;
    int32_t y;
    int32_t z;
};


int main()
{
    UnixSocket::Server server("/tmp/ar.sock");

    server.write<DataType>(DataType{1, 2, 3});
    auto data = server.read<DataType>();
    std::cout << data.x << ' ' << data.y << ' ' << data.z << std::endl;

    return 0;
}
