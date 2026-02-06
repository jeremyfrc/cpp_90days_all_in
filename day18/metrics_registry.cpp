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


class SafeRegistry {
public:
    struct Handle { 
        size_t id; 
        size_t gen;
    };

    Handle create() {
        std::lock_guard lock(m_);
        size_t id = next_id_++;

        data_[id] = Entry{MetricsBuffer(), 0, true};
        return {id, 0};
    }

    void remove(Handle h) {
        std::lock_guard lock(m_);
        auto it = data_.find(h.id);
        
        if (it != data_.end() && it->second.gen == h.gen){
            it->second.valid = false;
            it->second.gen++;
        }
    }

    template<typename F>
    void with(Handle h, F&& fn) {
        std::lock_guard lock(m_);

        auto it = data_.find(h.id);
        if (it == data_.end() 
        || it->second.valid == false 
        || it->second.gen != h.gen){
            throw std::runtime_error("Invalid handle");
        } 
        fn(it->second.buffer);
    }

private:
    struct Entry {
        MetricsBuffer buffer;
        size_t gen;
        bool valid;
    };

    std::mutex m_;
    std::unordered_map<size_t, Entry> data_;
    size_t next_id_ = 0;
};
