#include <mutex>
#include <vector>

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
    MetricsRegistry() = default;
    ~MetricsRegistry() = default;

    MetricsBuffer& create(MetricsBuffer&& buffer){
        buffers_.emplace_back(std::make_unique<MetricsBuffer>());
        return *buffers_.back();
    };

    MetricsBuffer& get(size_t i) {
        return *buffers_.at(i);
    }

private:
    std::vector<std::unique_ptr<MetricsBuffer>> buffers_;
};