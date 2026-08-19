
#include <cstdio>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <algorithm>
#include <arm_neon.h>

double calculate(long long iterations, double param1, double param2) {
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 8;

    double p1_sq = param1 * param1;
    double p2_sq = param2 * param2;
    double numerator = 2.0 * param2;

    const long long CHUNK_SIZE = 1000000;
    long long total_chunks = (iterations + CHUNK_SIZE - 1) / CHUNK_SIZE;
    std::atomic<long long> next_chunk(0);

    std::vector<double> thread_results(num_threads, 0.0);
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (unsigned int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&, t]() {
            double local_sum = 0.0;

            float64x2_t v_p1_sq = vdupq_n_f64(p1_sq);
            float64x2_t v_p2_sq = vdupq_n_f64(p2_sq);
            float64x2_t v_num   = vdupq_n_f64(numerator);
            float64x2_t v_four  = vdupq_n_f64(4.0);

            float64x2_t v_sum0  = vdupq_n_f64(0.0);
            float64x2_t v_sum1  = vdupq_n_f64(0.0);

            while (true) {
                long long chunk_idx = next_chunk.fetch_add(1, std::memory_order_relaxed);
                if (chunk_idx >= total_chunks) break;

                long long start = chunk_idx * CHUNK_SIZE + 1;
                long long end = std::min(start + CHUNK_SIZE - 1, iterations);

                long long i = start;
                long long simd_end = start + ((end - start + 1) / 4) * 4;

                if (simd_end > start) {
                    double s = static_cast<double>(start);
                    double init_i0[2] = { s, s + 1.0 };
                    double init_i1[2] = { s + 2.0, s + 3.0 };

                    float64x2_t v_i0 = vld1q_f64(init_i0);
                    float64x2_t v_i1 = vld1q_f64(init_i1);

                    for (; i < simd_end; i += 4) {
                        float64x2_t v_i0_sq = vmulq_f64(v_i0, v_i0);
                        float64x2_t v_i1_sq = vmulq_f64(v_i1, v_i1);

                        float64x2_t v_den0 = vsubq_f64(vmulq_f64(v_i0_sq, v_p1_sq), v_p2_sq);
                        float64x2_t v_den1 = vsubq_f64(vmulq_f64(v_i1_sq, v_p1_sq), v_p2_sq);

                        v_sum0 = vaddq_f64(v_sum0, vdivq_f64(v_num, v_den0));
                        v_sum1 = vaddq_f64(v_sum1, vdivq_f64(v_num, v_den1));

                        v_i0 = vaddq_f64(v_i0, v_four);
                        v_i1 = vaddq_f64(v_i1, v_four);
                    }
                }

                for (; i <= end; ++i) {
                    double double_i = static_cast<double>(i);
                    double den = double_i * double_i * p1_sq - p2_sq;
                    local_sum += numerator / den;
                }
            }

            float64x2_t v_sum = vaddq_f64(v_sum0, v_sum1);
            local_sum += vgetq_lane_f64(v_sum, 0) + vgetq_lane_f64(v_sum, 1);

            thread_results[t] = local_sum;
        });
    }

    for (auto& th : threads) {
        th.join();
    }

    double total_subtraction = 0.0;
    for (double res : thread_results) {
        total_subtraction += res;
    }

    return 1.0 - total_subtraction;
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    double result = calculate(200000000, 4, 1) * 4.0;
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end_time - start_time;

    std::printf("Result: %.12f\n", result);
    std::printf("Execution Time: %.6f seconds\n", elapsed.count());

    return 0;
}
