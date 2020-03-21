#include "socket_client.hpp"

#include <iostream>

int main()
{
    UnixSocketClient::init("/tmp/ar.sock");

    while (true) {
        std::cout << UnixSocketClient::read() << std::endl;
        std::string hoge;
        std::cin >> hoge;
        UnixSocketClient::write(hoge);
    }

    return 0;
}
