// STD
#include <unistd.h>
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>

// OMP
#include <omp.h>

// Local
#include "prng.h"
#include "stats.h"

enum Methods
{
    METHOD1 = 1,
    METHOD2 = 2,
    METHOD4 = 4,
};

inline bool is_shiny(const uint32_t tid, const uint16_t sid, const uint16_t pid_u, const uint16_t pid_l)
{
    return (tid ^ sid ^ pid_u ^ pid_l) < 8;
}

int main(const int argc, char** argv)
{
    uint16_t TID = 0;
    uint16_t SID = 0;
    uint8_t method = METHOD1;
    std::string outputFileName;

    int c;

    while ((c = getopt(argc, argv, "t: s: o: m:")) != -1)
    {
        switch (c)
        {
            case 't':
                TID = static_cast<uint16_t>(std::atoi(optarg));
                break;

            case 's':
                SID = static_cast<uint16_t>(std::atoi(optarg));
                break;

            case 'o':
                outputFileName = std::string(optarg);
                break;

            case 'm':
                method = static_cast<uint8_t>(std::atoi(optarg));
                break;

            case ':':
            case '?':
                std::cerr << "Option -" << optopt << " requires an argument." << std::endl;
                break;

            default:
                std::cerr << "Unknown option `-" << optopt << "'." << std::endl;
                break;
        }
    }

    for (; optind < argc; optind++)
    {
        std::cerr << "Non-option argument `" << argv[optind] << "`." << std::endl;
    }

    const uint16_t TSV = (TID ^ SID) >> 3;

    std::cout << "Using TID/SID: " << TID << '/' << SID << '\n';

    if (outputFileName.empty())
    {
        outputFileName = std::to_string(TSV) + ".csv";
    }

    std::vector<Stats::ShinyResult> results;

    const auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        #pragma omp single
        {
            std::cout << "Running with " << omp_get_num_threads() << " threads.\n";
        }

        std::vector<Stats::ShinyResult> local_results;
        local_results.reserve(600000 / omp_get_num_threads());

        #pragma omp for
        for (uint64_t seed = 0; seed <= UINT32_MAX; seed++)
        {
            const auto s1 = static_cast<uint32_t>(seed);
            const uint32_t s2 = PRNG::prng(s1);

            const uint16_t pid_l = PRNG::seed_to_val(s1);
            const uint16_t pid_u = PRNG::seed_to_val(s2);
            const uint32_t fullPID = (static_cast<uint32_t>(pid_u) << 16) | pid_l;

            if (is_shiny(TID, SID, pid_u, pid_l))
            {
                Stats::ShinyResult result;
                result.seed = s1;
                result.PID = fullPID;
                result.natureID = result.PID % 25;

                switch (method)
                {
                case METHOD1:
                    //std::cout << "Using method 1\n";
                    Stats::get_method1_ivs(s1, result);
                    break;
                case METHOD2:
                    //std::cout << "Using method 2\n";
                    Stats::get_method2_ivs(s1, result);
                    break;
                case METHOD4:
                    //std::cout << "Using method 4\n";
                    Stats::get_method4_ivs(s1, result);
                    break;
                default:
                    std::cerr << "Unknown method `" << method << "`.\n";
                    break;
                }

                local_results.push_back(result);
            }
        }

        #pragma omp critical
        {
            results.insert(results.end(), local_results.begin(), local_results.end());
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Search complete!\n";
    std::cout << "Found: " << results.size() << " shiny PIDs.\n";
    std::cout << "Time: " << elapsed_seconds.count() << "s.\n";

    std::cout << "Writing results to " << outputFileName << '\n';

    std::ofstream outFile (outputFileName);
    outFile << "PID (Hex),Nature,HP,Atk,Def,SpA,SpD,Spe,Total,Seed\n";

    for (const Stats::ShinyResult& shiny : results)
    {
        outFile << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << shiny.PID << std::dec << ','
        << Stats::natures[shiny.natureID] << ','
        << shiny.IVs[0] << ',' << shiny.IVs[1] << ',' << shiny.IVs[2] << ','
        << shiny.IVs[4] << ',' << shiny.IVs[5] << ',' << shiny.IVs[3] << ','
        << (shiny.IVs[0] + shiny.IVs[1] + shiny.IVs[2] + shiny.IVs[3] + shiny.IVs[4] + shiny.IVs[5]) << ','
        << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << shiny.seed << '\n';
    }

    outFile.close();
    std::cout << "Finished writing PIDs to file." << std::endl;

    return 0;
}