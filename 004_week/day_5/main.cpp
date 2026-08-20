#include <iostream>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <algorithm>

struct LCG {
    uint32_t value;
    static constexpr uint32_t a = 1664525u;
    static constexpr uint32_t c = 1013904223u;

    explicit LCG(uint32_t seed) : value(seed) {}

    inline uint32_t next() {
        value = a * value + c;
        return value;
    }
};

static int64_t max_subarray_sum(int n, uint32_t seed, int min_val, int max_val) {
    LCG lcg_gen(seed);
    const int range = max_val - min_val + 1;
    
    int64_t max_sum = -1e18;
    int64_t current_sum = 0;
    
    for (int i = 0; i < n; ++i) {
        int val = static_cast<int>(lcg_gen.next() % range) + min_val;
        current_sum += val;
        if (current_sum > max_sum) {
            max_sum = current_sum;
        }
        if (current_sum < 0) {
            current_sum = 0;
        }
    }
    return max_sum;
}

static int64_t total_max_subarray_sum(int n, uint32_t initial_seed, int min_val, int max_val) {
    int64_t total_sum = 0;
    LCG lcg_gen(initial_seed);
    for (int i = 0; i < 20; ++i) {
        uint32_t seed = lcg_gen.next();
        total_sum += max_subarray_sum(n, seed, min_val, max_val);
    }
    return total_sum;
}

int main() {
    const int n = 10000;
    const uint32_t initial_seed = 42;
    const int min_val = -10;
    const int max_val = 10;

    auto start_time = std::chrono::high_resolution_clock::now();
    int64_t result = total_max_subarray_sum(n, initial_seed, min_val, max_val);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end_time - start_time;

    std::cout << "Total Maximum Subarray Sum (20 runs): " << result << "\n";
    std::printf("Execution Time: %.6f seconds\n", diff.count());

    return 0;
}