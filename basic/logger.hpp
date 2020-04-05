#pragma once

#include <iostream>

inline namespace Basic
{

namespace Logger
{

class Logger
{
public:
    enum Level {
        Critical,
        Error,
        Debug,
        Info,
        Data
    };

protected:
    std::ostream& out;
    Level _level;

public:
    Logger(Level level = Info, std::ostream& out = std::cout) : out{out}, _level{level}
    {
    }
    Logger(std::ostream& out = std::cout)
        : out(out), _level{Info}
    {
    }

    static std::string level2str(Level level)
    {
        switch (level) {
        case Info:
            return "[Info] ";
        case Error:
            return "[Error] ";
        case Debug:
            return "[Debug] ";
        case Critical:
            return "[Critical] ";
        case Data:
            return "";
        default:
            return "[Undefined] ";
        }
    }

    void setLevel(Logger::Level level)
    {
        _level = level;
    }

    template <class T>
    void write(Logger::Level level, T message)
    {
        _level = level;
        out << level2str(level) << message << std::endl;
    }

    template <class T>
    Logger& write(T message)
    {
        out << level2str(_level) << message << std::endl;
    }

    template <class T>
    Logger& operator<<(T message)
    {
        std::cout << level2str(_level) << message << std::endl;
        return *this;
    }
};

}  // namespace Logger

}  // namespace Basic
