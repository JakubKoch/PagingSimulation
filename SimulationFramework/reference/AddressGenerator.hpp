#pragma once

#include <random>

namespace SF::reference
{
    class AddressGenerator
    {
        static constexpr double addressRangeToDivRatio{0.085};

    public:
        AddressGenerator(const unsigned minAddress, const unsigned maxAddress);

        unsigned generate();

    private:
        std::mt19937 generator{};
        std::normal_distribution<double> distribution;

        unsigned minAddress;
        unsigned maxAddress;
    };
}