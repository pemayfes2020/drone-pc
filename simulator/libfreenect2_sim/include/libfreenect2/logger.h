#pragma once

#include "logger.hpp"

#include <iostream>
#include <string>

namespace libfreenect2
{

class Logger : public Basic::Logger::Logger
{
public:
    bool good() { return true; }
    virtual void log(Logger::Level level, const std::string& message)
    {
        write(level, message);
    }

    virtual void log(const std::string& message)
    {
        write(message);
    }

    Logger(std::ostream& out = std::cout)
        : Basic::Logger::Logger(out)
    {
    }
};

Logger* createConsoleLogger(Logger::Level level);
void setGlobalLogger(Logger* logger);

}  // namespace libfreenect2
