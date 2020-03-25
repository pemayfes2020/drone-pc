#include "message_types.hpp"
#include "safe_exit.hpp"
#include "socket.hpp"

#include <condition_variable>
#include <mutex>
#include <optional>
#include <thread>

namespace Drone
{


std::optional<Common::Drone::SendData> send_data;
std::optional<Common::Drone::ReceiveData> receive_data;
std::mutex mutex_send, mutex_receive;
std::condition_variable cond_send, cond_receive;

void start(const std::string socket_path)
{
    std::thread{
        [&socket_path, &send_data, &receive_data,
            &mutex_send, &mutex_receive, &cond_send, &cond_receive]() {
            try {
                ThreadRoom::enter();
                UnixSocket::Client client(socket_path);

                while (!ThreadRoom::toExit()) {
                    {
                        std::unique_lock<std::mutex> lock{mutex_send};
                        while (!send_data || ThreadRoom::toExit()) {
                            cond_send.wait(lock);
                        }
                        client.write<Common::Drone::SendData>(
                            send_data.value());
                        send_data = std::nullopt;
                    }
                    ThreadRoom::toExit();
                    {
                        std::lock_guard<std::mutex> lock{mutex_receive};
                        receive_data = std::make_optional(
                            client.read<Common::Drone::ReceiveData>());
                        cond_receive.notify_one();
                    }
                }

            } catch (ThreadRoom::thread_abort) {
                ThreadRoom::exit();
                return;
            }
        }}
        .detach();
}

void send(Common::Drone::SendData _send_data)
{
    std::lock_guard<std::mutex> lock{mutex_send};
    send_data = std::make_optional(_send_data);
    cond_send.notify_one();
}

Common::Drone::ReceiveData read()
{
    std::unique_lock<std::mutex> lock{mutex_receive};
    while (!receive_data || ThreadRoom::toExit()) {
        cond_receive.wait(lock);
    }
    auto tmp = receive_data.value();
    receive_data = std::nullopt;
    return tmp;
}

}  // namespace Drone
