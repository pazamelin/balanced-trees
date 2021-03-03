#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>
#include <exception>
#include <set>

#include "profiler/profiler.hpp"

#include "avl.hpp"
#include "splay.hpp"

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

    std::string filename_prefix = "../profiler/results/";

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

	std::string filename_prefix = "../profiler/results/";

	const auto results = profile<tree::splay<int>>(size_start, size_end, size_step,
		operations_per_step);

	write_csv(filename_prefix + "splay.csv", results);
}

void profile_rb()
{
    using profiler::profile;

    std::size_t size_start = 10000;
    std::size_t size_end = 1'000'000;
    std::size_t size_step = 10000;
    std::size_t operations_per_step = 1000;

    std::string filename_prefix = "../profiler/results/";

    const auto results = profile<std::set<int>>(size_start, size_end, size_step,
                                                 operations_per_step);

    write_csv(filename_prefix + "set.csv", results);
}

int main()
{
    profile_avl();
	profile_splay();
    profile_rb();
    return 0;
}
