#include "file_logger.h"
#include <vector>

int main()
{
    std::vector<FileLogger> logs;

    logs.emplace_back("a.log");
    logs.emplace_back("b.log");
    logs.emplace_back("c.log");

    logs[0].log("from a");
    logs[1].log("from b");
    logs[2].log("from c");

    return 1;
}