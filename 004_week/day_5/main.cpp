#include <iostream>
#include <chrono>
#include <iomanip>
#include <cstdint>

static inline int64_t max_subarray_sum(int n, uint32_t seed, int min_val, int max_val) {
    const uint32_t range = static_cast<uint32_t>(max_val - min_val + 1);
    uint32_t inner_seed = seed;
    
    int64_t max_sum = -1e18;
    int64_t current_sum = 0;
    
    for (int i = 0; i < n; ++i) {
        inner_seed = inner_seed * 1664525u + 1013904223u;
        int val = static_cast<int>(inner_seed % range) + min_val;
        
        if (i == 0) {
            current_sum = val;
            max_sum = val;
        } else {
            current_sum = (current_sum > 0) ? (current_sum + val) : val;
            if (current_sum > max_sum) {
                max_sum = current_sum;
            }
        }
    }
    return max_sum;
}

static inline int64_t total_max_subarray_sum(int n, uint32_t initial_seed, int min_val, int max_val) {
    int64_t total_sum = 0;
    uint32_t outer_seed = initial_seed;
    
    for (int i = 0; i < 20; ++i) {
        outer_seed = outer_seed * 1664525u + 1013904223u;
        total_sum += max_subarray_sum(n, outer_seed, min_val, max_val);
    }
    return total_sum;
}

int main() {
    int n = 10000;
    uint32_t initial_seed = 42;
    int min_val = -10;
    int max_val = 10;

    auto start_time = std::chrono::high_resolution_clock::now();
    int64_t result = total_max_subarray_sum(n, initial_seed, min_val, max_val);
    auto end_time = std::chrono::high_resolution_clock::now();

    double duration = std::chrono::duration<double>(end_time - start_time).count();

    std::cout << "Total Maximum Subarray Sum (20 runs): " << result << "\n";
    std::cout << "Execution Time: " << std::fixed << std::setprecision(6) << duration << " seconds\n";

    return 0;
}