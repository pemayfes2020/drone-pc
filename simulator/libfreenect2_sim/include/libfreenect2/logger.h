#pragma once

#include <iostream>
#include <string>

namespace libfreenect2
{

class Logger
{
public:
    enum Level {
        Info,
        Error,
        Debug
    };

protected:
    Level _level;

public:
    Logger()
    {
    }
    Logger(Level level) : _level(level)
    {
    }

    static std::string level2str(Level level)
    {
        switch (level) {
        case Info:
            return "Info";
        case Error:
            return "Error";
        case Debug:
            return "Debug";
        default:
            return "Undefined";
        }
    }

    bool good() { return true; }

    virtual void log(Logger::Level level, const std::string& message)
    {
        _level = level;
        std::cout << "[" << libfreenect2::Logger::level2str(level) << "] " << message << std::endl;
    }

    virtual void log(const std::string& message)
    {
        std::cout << "[" << libfreenect2::Logger::level2str(_level) << "] " << message << std::endl;
    }
};


Logger* createConsoleLogger(Logger::Level level);
void setGlobalLogger(Logger* logger);

}  // namespace libfreenect2
