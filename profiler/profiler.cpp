#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>
#include <exception>
#include <sstream>
#include <string>
#include <set>

#include "profiler.hpp"

#include "avl.hpp"
#include "splay.hpp"
#include "cartesian.hpp"

void write_csv(const std::string& csv_filename,
               const std::vector<profiler::profile_statistic>& result
)
{
    std::ofstream csv_file(csv_filename, std::ios::out | std::ios::trunc);
    if (csv_file.is_open())
    {
        csv_file << "tree_size,insert_time,find_time,erase_time\n";
        for (const auto& statistic : result)
        {
            csv_file << statistic.size        << "," <<
                     statistic.insert_time << "," <<
                     statistic.find_time   << "," <<
                     statistic.erase_time  << "\n";
        }
    }
    else
    {
        throw std::runtime_error("failed to open a file");
    }
}

void profile_avl()
{
    using profiler::profile;

    std::size_t size_start = 10000;
    std::size_t size_end = 1'000'000;
    std::size_t size_step = 10000;
    std::size_t operations_per_step = 1000;

    std::string filename_prefix = "results/";

    const auto results = profile<tree::avl<int>>(size_start, size_end, size_step,
                                                 operations_per_step);

    write_csv(filename_prefix + "avl.csv", results);
}

void profile_splay()
{
    using profiler::profile;

    std::size_t size_start = 10000;
    std::size_t size_end = 1'000'000;
    std::size_t size_step = 10000;
    std::size_t operations_per_step = 1000;

    std::string filename_prefix = "results/";

    const auto results = profile<tree::splay<int>>(size_start, size_end, size_step,
                                                   operations_per_step);

    write_csv(filename_prefix + "splay.csv", results);
}

void profile_cartesian()
{
    using profiler::profile;

    std::size_t size_start = 10000;
    std::size_t size_end = 1'000'000;
    std::size_t size_step = 10000;
    std::size_t operations_per_step = 1000;

    std::string filename_prefix = "results/";

    const auto results = profile<tree::cartesian<int>>(size_start, size_end, size_step,
                                                       operations_per_step);

    write_csv(filename_prefix + "cartesian.csv", results);
}

void profile_rb()
{
    using profiler::profile;

    std::size_t size_start = 10000;
    std::size_t size_end = 1'000'000;
    std::size_t size_step = 10000;
    std::size_t operations_per_step = 1000;

    std::string filename_prefix = "results/";

    const auto results = profile<std::set<int>>(size_start, size_end, size_step,
                                                operations_per_step);

    write_csv(filename_prefix + "set.csv", results);
}

int main(int argc, char* argv[])
{
    std::string what_tree;

    if(argc >= 2)
    {
        std::stringstream ss;
        ss << argv[1];
        ss >> what_tree;
    }
    else
    {
        std::cerr << "wrong parameters" << "\n";
        exit(1);
    }

    if (what_tree == "avl")
    {
        profile_avl();
    }
    else if(what_tree == "splay")
    {

        profile_splay();
    }
    else if(what_tree == "cartesian")
    {
        profile_cartesian();
    }
    else if(what_tree == "rb")
    {
        profile_rb();
    }
    else if(what_tree == "all")
    {
        profile_avl();
        profile_splay();
        profile_cartesian();
        profile_rb();
    }
    else
    {
        std::cout << "wrong tree" << "\n";
    }

    return 0;
}