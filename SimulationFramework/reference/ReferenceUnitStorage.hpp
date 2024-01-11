#pragma once

#include <array>
#include <random>
#include <vector>
#include <algorithm>

#include "ReferenceUnit.hpp"

namespace SF::reference
{
    template <unsigned numberOfReferenceUnits, unsigned numberOfRecords, unsigned pageSize>
    class ReferenceUnitStorage
    {
    public:
        std::vector<std::pair<unsigned, ReferenceType>> getReferences()
        {
            std::vector<std::pair<unsigned, ReferenceType>> references{};

            for (auto &referenceUnit : referenceUnits)
            {
                references.push_back(referenceUnit.generateReference());
            }

            return references;
        }

        void changeReferences(const unsigned iteration)
        {
            referenceUnits.clear();
            addReferences(iteration);
        }

    private:
        void addReferences(const unsigned iteration)
        {
            unsigned minAddress{0};
            unsigned maxAddress{0};
            const auto &spaces{getDividedSpaces(iteration)};

            for (unsigned i{0}; i < numberOfReferenceUnits - 1; ++i)
            {
                maxAddress = spaces[i];
                const auto &referenceUnit{createReferenceUnit(minAddress, maxAddress)};
                minAddress = maxAddress;

                referenceUnits.push_back(referenceUnit);
            }

            const auto &lastReferenceUnit{createReferenceUnit(minAddress, numberOfRecords - 1)};
            referenceUnits.push_back(lastReferenceUnit);
        }

        ReferenceUnit createReferenceUnit(const unsigned minAddress, const unsigned maxAddress)
        {
            return ReferenceUnit(minAddress * pageSize, ((minAddress + (maxAddress - minAddress)) * pageSize) - 1);
        }

        std::vector<unsigned> getDividedSpaces(const unsigned iteration)
        {
            std::mt19937 generator{iteration};
            std::uniform_int_distribution<unsigned> distribution{1, numberOfRecords - 1};
            std::vector<unsigned> devidedSpaces{};

            for (unsigned i{0}; i < numberOfReferenceUnits - 1; ++i)
            {
                devidedSpaces.push_back(distribution(generator));
            }

            std::sort(devidedSpaces.begin(), devidedSpaces.end());
            return devidedSpaces;
        }

        std::vector<ReferenceUnit> referenceUnits{};
    };
}