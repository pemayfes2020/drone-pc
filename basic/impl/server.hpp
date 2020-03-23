#include "impl/base.hpp"

#include <boost/asio.hpp>

#include <string>


inline namespace Basic
{

namespace UnixSocket
{

class Server : public Base
{
private:
    boost::asio::local::stream_protocol::acceptor acceptor;
    std::string bind_path;

public:
    Server(const std::string& bind_path)
        : Base(bind_path),
          acceptor{io_service, endpoint}, bind_path{bind_path}
    {
        acceptor.accept(socket);
    }

    ~Server()
    {
        ::unlink(bind_path.c_str());
    }
};


}  // namespace UnixSocket
}  // namespace Basic
