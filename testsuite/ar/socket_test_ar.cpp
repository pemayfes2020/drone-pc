#include "socket_server.hpp"

#include <iostream>

int main()
{
    UnixSocketServer::init("/tmp/ar.sock");

    while (true) {
        std::string hoge;
        std::cin >> hoge;
        UnixSocketServer::write(hoge);
        std::cout << UnixSocketServer::read() << std::endl;
    }

    return 0;
}
