#include "file_logger.h"

int main()
{
    FileLogger logger("app.log");
    logger.log("hello");

    FileLogger logger2 = std::move(logger);
    logger2.log("world!");

    return 0;
}