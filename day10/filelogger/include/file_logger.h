#pragma once
#include <fstream>
#include <string>

class FileLogger
{
public:
    explicit FileLogger(const std::string &path);

    // 禁止 copy（独占资源）
    FileLogger(const FileLogger &) = delete;
    FileLogger &operator=(const FileLogger &) = delete;

    // 允许 move（转移所有权）
    FileLogger(FileLogger &&other) noexcept;
    FileLogger &operator=(FileLogger &&other) noexcept;

    // 释放资源
    ~FileLogger() noexcept = default;

    void log(const std::string &message);

    bool is_open() const noexcept;

private:
    std::ofstream file_;
};