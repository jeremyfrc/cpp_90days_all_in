#include <mutex>
#include <vector>
#include <memory>
#include <stdexcept>
#include <unordered_map>

class Metrics {
public:
    void record(std::string_view key, double v);

    struct Stat {
        uint64_t count;
        double sum;
        double min;
        double max;
    };

    std::unordered_map<std::string, Stat> snapshot() const;
};