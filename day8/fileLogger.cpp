class FileLogger
{
public:
    FileLogger(const FileLogger &) = delete;
    FileLogger &operator=(const FileLogger &) = delete;

    FileLogger(FileLogger &&) = delete;
    FileLogger &&operator=(FileLogger &&) = delete;

    FileHandle release();
    staic FileLogger adopt(FileHandle);
}