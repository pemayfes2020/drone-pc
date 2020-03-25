#include "esp32.hpp"

#include <boost/asio.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>

namespace ESP32
{

void wait_ms(int duration_ms)
{
    auto start = std::chrono::system_clock::now();
    while (
        (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start)).count() < duration_ms) {
    }
}

static std::unique_ptr<boost::asio::io_service> io;
static std::unique_ptr<boost::asio::serial_port> serial;

std::optional<int> command;
std::mutex command_mutex;

void start(std::string serial_port)
{
    std::thread{[&io, &serial, &command, &serial_port]() mutable {
        {  // Setup

            command = std::nullopt;
            io = std::make_unique<boost::asio::io_service>();
            serial = std::make_unique<boost::asio::serial_port>(*io, serial_port);

            // Baudrate(bit per second)
            serial->set_option(boost::asio::serial_port_base::baud_rate(115200));

            // 通信が安定するまで5秒待つ
            std::this_thread::sleep_for(std::chrono::seconds(5));

            std::cout << "[Info] [ESP32] Bluetooth communication is now ready" << std::endl;
        }

        while (true) {
            wait_ms(10);
            std::lock_guard<std::mutex> lock{command_mutex};
            if (command) {
                std::cout << "[Info] [ESP32] Send a command '"
                          << command.value() << "'"
                          << std::endl;
                boost::asio::write(*serial, boost::asio::buffer(std::to_string(command.value())));

                command = std::nullopt;
            }
        }
    }}
        .detach();
}

void setCommand(const int _command)
{
    std::lock_guard<std::mutex> lock{command_mutex};
    command = std::make_optional(_command);
}

}  // namespace ESP32
