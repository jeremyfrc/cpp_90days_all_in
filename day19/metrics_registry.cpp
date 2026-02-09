#include <mutex>
#include <vector>
#include <memory>
#include <stdexcept>
#include <unordered_map>

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


class Registry {
public:
    struct Handle{
        size_t id;
    };

    Handle create() {
        std::lock_guard<std::mutex> lock_(m_);
        size_t id = next_id_++;
        data_.try_emplace(id, std::make_shared<MetricsBuffer>());
        return {id};
    }

    void remove(Handle h){
        std::lock_guard<std::mutex> lock_(m_);
        data_.erase(h.id);
    }

    template<typename F>
    void with(Handle h, F&& fn) {
        std::shared_ptr<MetricsBuffer> bufferPtr;

        {
            std::lock_guard<std::mutex> lock(m_);
            auto it = data_.find(h.id);
            if (it == data_.end()){
                throw std::runtime_error("Invalid handle");
            }

            bufferPtr = it->second:
        }

        fn(*bufferPtr);
    }

private:
    std::mutex m_;
    std::unordered_map<size_t, std::shared_ptr<MetricsBuffer>> data_;
    size_t next_id_ = 0;
};
