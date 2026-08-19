
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <vector>

static double calculate_chunk(long long start, long long end, double param1, double param2) {
    double sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    long long i = start;
    
    // Unroll loop to maximize instruction-level parallelism across FP pipelines
    for (; i <= end - 3; i += 4) {
        double j1_0 = (double)(i + 0) * param1 - param2;
        double j2_0 = (double)(i + 0) * param1 + param2;
        sum0 += (1.0 / j2_0) - (1.0 / j1_0);

        double j1_1 = (double)(i + 1) * param1 - param2;
        double j2_1 = (double)(i + 1) * param1 + param2;
        sum1 += (1.0 / j2_1) - (1.0 / j1_1);

        double j1_2 = (double)(i + 2) * param1 - param2;
        double j2_2 = (double)(i + 2) * param1 + param2;
        sum2 += (1.0 / j2_2) - (1.0 / j1_2);

        double j1_3 = (double)(i + 3) * param1 - param2;
        double j2_3 = (double)(i + 3) * param1 + param2;
        sum3 += (1.0 / j2_3) - (1.0 / j1_3);
    }
    
    for (; i <= end; ++i) {
        double j1 = (double)i * param1 - param2;
        double j2 = (double)i * param1 + param2;
        sum0 += (1.0 / j2) - (1.0 / j1);
    }
    
    return sum0 + sum1 + sum2 + sum3;
}

double calculate(long long iterations, double param1, double param2) {
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 8;
    if (iterations < num_threads) {
        num_threads = (iterations > 0) ? (unsigned int)iterations : 1;
    }

    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    std::vector<double> results(num_threads, 0.0);

    long long chunk_size = iterations / num_threads;

    for (unsigned int t = 0; t < num_threads; ++t) {
        long long start = t * chunk_size + 1;
        long long end = (t == num_threads - 1) ? iterations : (t + 1) * chunk_size;

        threads.emplace_back([start, end, param1, param2, &results, t]() {
            results[t] = calculate_chunk(start, end, param1, param2);
        });
    }

    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    double total = 1.0;
    for (double r : results) {
        total += r;
    }
    return total;
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    double result = calculate(200000000LL, 4.0, 1.0) * 4.0;
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Result: " << result << "\n";
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Execution Time: " << elapsed.count() << " seconds\n";

    return 0;
}
