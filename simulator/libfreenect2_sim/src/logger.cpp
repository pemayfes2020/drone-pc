#include "libfreenect2/logger.h"

#include <memory>

namespace libfreenect2
{

// 別に使わなくていいや

std::shared_ptr<Logger> logger;

Logger* createConsoleLogger(Logger::Level level)
{
    return nullptr;
}

void setGlobalLogger(Logger* logger) {}

}  // namespace libfreenect2
