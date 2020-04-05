#pragma once

#include <boost/asio.hpp>

#include <string>
#include <type_traits>

inline namespace Basic
{

namespace UnixSocket
{

template <class T>
using add_const_rvalueref_t = std::add_const_t<std::add_rvalue_reference_t<T>>;

template <class T>
using add_const_lvalueref_t = std::add_const_t<std::add_lvalue_reference_t<T>>;

template <class T>
using add_const_pointer_t = std::add_pointer_t<std::add_const_t<T>>;

class Base
{
protected:
    boost::asio::io_service io_service;
    boost::asio::local::stream_protocol::endpoint endpoint;
    boost::asio::local::stream_protocol::socket socket;

public:
    Base(const std::string& bind_path)
        : endpoint{bind_path},
          socket{io_service}
    {
    }

    ~Base()
    {
        socket.close();
        io_service.stop();
    }

    template <class T>
    void write(T value)
    {
        char bytes[sizeof(T)];
        *(reinterpret_cast<std::add_pointer_t<T>>(bytes)) = value;
        boost::asio::write(socket, boost::asio::buffer(bytes));
    }

    template <class T>
    T read()
    {
        constexpr int size = sizeof(T);
        boost::asio::streambuf buf;
        boost::asio::read(socket, buf, boost::asio::transfer_at_least(size));
        return *reinterpret_cast<add_const_pointer_t<T>>(
            boost::asio::buffer_cast<const char*>(buf.data()));
    }
};

}  // namespace UnixSocket

}  // namespace Basic
