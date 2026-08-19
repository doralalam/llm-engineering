
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <thread>
#include <arm_neon.h>

double calculate_chunk(long long start_i, long long end_i, double param1, double param2) {
    long long count = end_i - start_i;
    long long vec_count = count / 8;

    float64x2_t v_p1 = vdupq_n_f64(param1);
    float64x2_t v_p2 = vdupq_n_f64(param2);
    float64x2_t v_one = vdupq_n_f64(1.0);
    float64x2_t v_step8 = vdupq_n_f64(8.0);

    float64x2_t v_sum0 = vdupq_n_f64(0.0);
    float64x2_t v_sum1 = vdupq_n_f64(0.0);
    float64x2_t v_sum2 = vdupq_n_f64(0.0);
    float64x2_t v_sum3 = vdupq_n_f64(0.0);

    double cur_i = static_cast<double>(start_i);
    double init0[2] = {cur_i, cur_i + 1.0};
    double init1[2] = {cur_i + 2.0, cur_i + 3.0};
    double init2[2] = {cur_i + 4.0, cur_i + 5.0};
    double init3[2] = {cur_i + 6.0, cur_i + 7.0};

    float64x2_t v_i0 = vld1q_f64(init0);
    float64x2_t v_i1 = vld1q_f64(init1);
    float64x2_t v_i2 = vld1q_f64(init2);
    float64x2_t v_i3 = vld1q_f64(init3);

    for (long long k = 0; k < vec_count; ++k) {
        float64x2_t ip0 = vmulq_f64(v_i0, v_p1);
        float64x2_t j1_0 = vsubq_f64(ip0, v_p2);
        float64x2_t j2_0 = vaddq_f64(ip0, v_p2);
        float64x2_t r1_0 = vdivq_f64(v_one, j1_0);
        float64x2_t r2_0 = vdivq_f64(v_one, j2_0);
        v_sum0 = vaddq_f64(v_sum0, vsubq_f64(r2_0, r1_0));

        float64x2_t ip1 = vmulq_f64(v_i1, v_p1);
        float64x2_t j1_1 = vsubq_f64(ip1, v_p2);
        float64x2_t j2_1 = vaddq_f64(ip1, v_p2);
        float64x2_t r1_1 = vdivq_f64(v_one, j1_1);
        float64x2_t r2_1 = vdivq_f64(v_one, j2_1);
        v_sum1 = vaddq_f64(v_sum1, vsubq_f64(r2_1, r1_1));

        float64x2_t ip2 = vmulq_f64(v_i2, v_p1);
        float64x2_t j1_2 = vsubq_f64(ip2, v_p2);
        float64x2_t j2_2 = vaddq_f64(ip2, v_p2);
        float64x2_t r1_2 = vdivq_f64(v_one, j1_2);
        float64x2_t r2_2 = vdivq_f64(v_one, j2_2);
        v_sum2 = vaddq_f64(v_sum2, vsubq_f64(r2_2, r1_2));

        float64x2_t ip3 = vmulq_f64(v_i3, v_p1);
        float64x2_t j1_3 = vsubq_f64(ip3, v_p2);
        float64x2_t j2_3 = vaddq_f64(ip3, v_p2);
        float64x2_t r1_3 = vdivq_f64(v_one, j1_3);
        float64x2_t r2_3 = vdivq_f64(v_one, j2_3);
        v_sum3 = vaddq_f64(v_sum3, vsubq_f64(r2_3, r1_3));

        v_i0 = vaddq_f64(v_i0, v_step8);
        v_i1 = vaddq_f64(v_i1, v_step8);
        v_i2 = vaddq_f64(v_i2, v_step8);
        v_i3 = vaddq_f64(v_i3, v_step8);
    }

    float64x2_t total_v = vaddq_f64(vaddq_f64(v_sum0, v_sum1), vaddq_f64(v_sum2, v_sum3));
    double chunk_sum = vgetq_lane_f64(total_v, 0) + vgetq_lane_f64(total_v, 1);

    long long rem_start = start_i + vec_count * 8;
    for (long long i = rem_start; i < end_i; ++i) {
        double j1 = static_cast<double>(i) * param1 - param2;
        chunk_sum -= (1.0 / j1);
        double j2 = static_cast<double>(i) * param1 + param2;
        chunk_sum += (1.0 / j2);
    }

    return chunk_sum;
}

double calculate(long long iterations, double param1, double param2) {
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 8;
    if (iterations < 10000) num_threads = 1;

    std::vector<std::thread> threads(num_threads);
    std::vector<double> results(num_threads, 0.0);

    long long chunk_size = iterations / num_threads;

    for (unsigned int t = 0; t < num_threads; ++t) {
        long long start_i = 1 + t * chunk_size;
        long long end_i = (t == num_threads - 1) ? (iterations + 1) : (start_i + chunk_size);
        threads[t] = std::thread([start_i, end_i, param1, param2, &results, t]() {
            results[t] = calculate_chunk(start_i, end_i, param1, param2);
        });
    }

    double total_sum = 1.0;
    for (unsigned int t = 0; t < num_threads; ++t) {
        threads[t].join();
        total_sum += results[t];
    }

    return total_sum;
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    double result = calculate(200000000, 4.0, 1.0) * 4.0;

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end_time - start_time;

    std::cout << "Result: " << std::fixed << std::setprecision(12) << result << "\n";
    std::cout << "Execution Time: " << std::fixed << std::setprecision(6) << diff.count() << " seconds\n";

    return 0;
}
