#include "message_types.hpp"
#include "safe_exit.hpp"
#include "socket.hpp"

#include <optional>
#include <thread>

namespace Drone
{


std::optional<Common::Drone::SendData> send_data;
std::optional<Common::Drone::ReceiveData> receive_data;

void start(const std::string socket_path)
{
    std::thread{
        [&socket_path, &send_data, &receive_data]() {
            try {
                ThreadRoom::enter();
                UnixSocket::Client client(socket_path);

                while (!ThreadRoom::toExit()) {
                }

            } catch (ThreadRoom::thread_abort) {
                ThreadRoom::exit();
                return;
            }
        }}
        .detach();
}

void command()
{
}

}  // namespace Drone
