#include "impl/base.hpp"

#include <boost/asio.hpp>


inline namespace Basic
{

namespace UnixSocket
{

class Client : public Base
{

public:
    Client(const std::string& bind_path)
        : Base(bind_path)
    {
        socket.connect(endpoint);
    }

    ~Client() = default;
};


}  // namespace UnixSocket
}  // namespace Basic
