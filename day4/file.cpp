#include <iostream>

struct File
{
    FILE *f;

    File(const char *path)
    {
        f = fopen(path, "r");
    }

    ~File()
    {
        if (f)
            fclose(f);
    }

    File(const File &) = delete;
    File &operator=(const File &) = delete;

    File(File &&other) noexcept : f(other.f)
    {
        other.f = nullptr;
    }

    File &operator=(File &other) noexcept
    {
        if (this != &other)
        {
            if (f)
                fclose(f);
            f = other.f;
            other.f = nullptr;
        }
        return *this;
    }
};