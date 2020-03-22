#include "socket_server.hpp"

#include <boost/asio.hpp>

#include <iostream>
#include <memory>

inline namespace Basic
{

namespace UnixSocketServer
{

class Socket
{
private:
    boost::asio::io_service io_service;
    boost::asio::local::stream_protocol::endpoint endpoint;
    boost::asio::local::stream_protocol::acceptor acceptor;
    boost::asio::local::stream_protocol::socket socket;

public:
    Socket(const std::string& bind_path)
        : endpoint{bind_path},
          acceptor{io_service, endpoint},
          socket{io_service}
    {
        acceptor.accept(socket);
    }

    ~Socket()
    {
        socket.close();
        io_service.stop();
    }

public:
    void write(const std::string& str)
    {
        boost::asio::write(socket, boost::asio::buffer(str));
    }

    std::string read()
    {
        boost::asio::streambuf buf;
        boost::asio::read(socket, buf, boost::asio::transfer_at_least(1));
        return boost::asio::buffer_cast<const char*>(buf.data());
    }
};

std::unique_ptr<Socket> socket;

void init(const std::string& bind_path)
{
    // 既存のバインドを切る
    ::unlink(bind_path.c_str());
    socket = std::make_unique<Socket>(bind_path);
}

void write(const std::string& str)
{
    socket->write(str);
}

std::string read()
{
    return socket->read();
}

}  // namespace UnixSocketServer
}  // namespace Basic
