#include <mutex>
#include <vector>
#include <memory>
#include <stdexcept>

using spacename std;


class MetricsBuffer
{
public:
    MetricsBuffer();

    MetricsBuffer(const MetricsBuffer &) = delete;
    MetricsBuffer &operator=(const MetricsBuffer &) = delete;

    ~MetricsBuffer() = default;

    void push(int v);
    int latest() const;

private:
    std::vector<int> data_;
    std::mutex m_;
};


class MetricsRegistry {
public:
    struct Handle {
        size_t index;
    };

    Handle create() {
        std::lock_guard<std::mutex> lock(mutex_);

        buffers_.emplace_back(std::make_unique<MetricsBuffer>());
        return Handle{buffers_.size() - 1};
    }

    template <typename F>
    void with(Handle h, F&& fn){
        std::lock_guard<std::mutex> lock(mutex_);

        if (h.index >= buffers_.size()) {
            throw std::out_of_range("Invalid handle index");
        }

        fn(*buffers_[h.index]);
    }

private:
    std::vector<std::unique_ptr<MetricsBuffer>> buffers_;
};