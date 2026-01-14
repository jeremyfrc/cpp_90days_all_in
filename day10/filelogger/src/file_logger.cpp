#include "file_logger.h"

FileLogger::FileLogger(const std::string &path) : file_(path, std::ios::app) {}

FileLogger::FileLogger(FileLogger &&other) noexcept : file_(std::move(other.file_)) {}

FileLogger &FileLogger::operator=(FileLogger &&other) noexcept
{
    if (this != &other)
    {
        file_ = std::move(other.file_);
    }
    return *this;
}

// FileLogger::~FileLogger() = default;

void FileLogger::log(const std::string &message)
{
    if (file_.is_open())
    {
        file_ << message << "\n";
        file_.flush();
    }
}

bool FileLogger::is_open() const noexcept
{
    return file_.is_open();
}