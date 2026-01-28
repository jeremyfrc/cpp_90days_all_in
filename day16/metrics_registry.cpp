#include <mutex>
#include <vector>
#include <memory>
#include <stdexcept>
#include <unordered_map>

using namespace std;


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


class SafeRegistry {
public:
    
    struct Handle {size_t id;};

    /*
    struct Handle {
        private:
            size_t id;
            friend class SafeRegistry;
    };
    */

    Handle create()
    {
        std::lock_guard<std::mutex> lock(m_);

        size_t id = next_id_++;
        data_.emplace(id, MetricsBuffer{});
        // or use data_.try_empalce(id);
        return Handle{id};
    };

    template <typename F>
    void with(Handle h, F&& fn){
        std::lock_guard<std::mutex> lock(m_);

        auto it = data_.find(h.id);
        if (it == data_.end()){
            throw std::out_of_range("Invalid handle id");
        }
        fn(it->second);

        /*
        高级上锁
        MetricsBuffer* ptr;
        {
            std::lock_guard lock(m_);
            ptr = &it->second;
        }
        fn(*ptr); // 锁外执行
        */
    };

private:
    std::mutex m_;
    std::unordered_map<size_t, MetricsBuffer> data_;
    size_t next_id_ = 0;
};