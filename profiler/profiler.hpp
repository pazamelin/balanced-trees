#pragma once

#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <sstream>
#include <random>
#include <limits>

namespace profiler
{
    class AccumulateDuration
    {
    public:
        explicit AccumulateDuration(double& accumulator)
                : accumulator{accumulator}, start{std::chrono::steady_clock::now()}
        { }

        ~AccumulateDuration()
        {
            auto finish = std::chrono::steady_clock::now();
            std::chrono::duration<double> duration = finish - start;
            accumulator += duration.count();
            int x = 1;
        }

    private:
        std::chrono::steady_clock::time_point start;
        double& accumulator;
    };

} // namespace profiler

#ifndef UNIQ_ID
#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)
#endif

#define ACCUMULATE_DURATION(accumulator) \
    profiler::AccumulateDuration UNIQ_ID(__LINE__){accumulator};

namespace profiler
{
    struct profile_statistic
    {
        std::size_t size;
        double insert_time;
        double find_time;
        double erase_time;
    };

    template <typename Tree>
    std::vector<profile_statistic> profile(std::size_t size_start,
                                           std::size_t size_end,
                                           std::size_t size_step,
                                           std::size_t operations_per_step
    )
    {
        std::random_device rd;
        const auto seed = rd();
        std::mt19937 gen(seed);

        int key_min = std::numeric_limits<int>::min();
        int key_max = std::numeric_limits<int>::max();
        std::uniform_int_distribution<> key_dist(key_min, key_max);
        auto get_random_key = [&]() { return key_dist(gen); };

        Tree tree;

        auto update_size = [&](std::size_t new_size)
        {
            while (tree.size() != new_size)
            {
                auto key = get_random_key();
                tree.insert(key);
            }
        };

        std::vector<profile_statistic> results;

        for (std::size_t size = size_start; size < size_end; size += size_step)
        {
            update_size(size);
            double total_insert_time = 0;
            double total_erase_time = 0;
            double total_find_time = 0;

            for (std::size_t i = 0; i < operations_per_step; i++)
            {
                auto key = get_random_key();

                {
                    ACCUMULATE_DURATION(total_insert_time);
                    tree.insert(key);
                }

                {
                    ACCUMULATE_DURATION(total_find_time);
                    tree.find(key);
                }

                {
                    ACCUMULATE_DURATION(total_erase_time);
                    tree.erase(key);
                }

            }
            double average_insert_time = total_insert_time / operations_per_step;
            double average_find_time = total_find_time / operations_per_step;
            double average_erase_time = total_erase_time / operations_per_step;

            results.push_back({size, average_insert_time, average_find_time, average_erase_time});
        }

        return results;
    }

} // namespace profiler