#include "AddressGenerator.hpp"

namespace SF::reference
{
    AddressGenerator::AddressGenerator(const unsigned minAddress, const unsigned maxAddress)
        : minAddress{minAddress},
          maxAddress{maxAddress}
    {
        const unsigned addressRange{(maxAddress - minAddress) / 2};
        const double mean{static_cast<double>(minAddress + addressRange)};
        const double dev{static_cast<double>(addressRange * addressRangeToDivRatio)};

        distribution = std::normal_distribution<double>{mean, dev};
    }

    unsigned AddressGenerator::generate()
    {
        const unsigned generatedAddress{static_cast<unsigned>(distribution(generator))};

        if (generatedAddress > maxAddress)
        {
            return maxAddress;
        }

        if (generatedAddress < minAddress)
        {
            return minAddress;
        }

        return generatedAddress;
    }
}