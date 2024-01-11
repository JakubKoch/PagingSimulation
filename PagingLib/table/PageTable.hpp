#pragma once

#include <cmath>
#include <optional>

#include "VirtualAddress.hpp"
#include "PageTableEntry.hpp"

namespace PL::table
{
    template <unsigned number>
    struct NumberOfBits
    {
        static const unsigned value = static_cast<unsigned>(std::ceil(std::log2(number)));
    };

    template <unsigned numberOfRecords, unsigned pageSize>
    class PageTable
    {
    public:
        using DefinedPageTableEntry = PageTableEntry<NumberOfBits<numberOfRecords>::value>;
        using DefinedVirtualAddress = VirtualAddress<NumberOfBits<numberOfRecords>::value, NumberOfBits<pageSize>::value>;

        std::optional<unsigned> getPhysicalAddress(const DefinedVirtualAddress &virtualAddress, const bool isModified)
        {
            const auto index{virtualAddress.getIndex()};

            if (isModified)
            {
                table[index].setModifiedBit();
            }

            if (!table[index].isPresent())
            {
                return std::nullopt;
            }

            return calculatePhysicalAddress(virtualAddress);
        }

        void addPageFrameNumberFromAddress(const DefinedVirtualAddress &virtualAddress, const unsigned frameNumber)
        {
            const auto index{virtualAddress.getIndex()};
            table[index].usePage(frameNumber);
        }

        void replacePageFrameFromAddress(const DefinedVirtualAddress &virtualAddress, const unsigned pageToReplaceIndex)
        {
            const auto newPageIndex{virtualAddress.getIndex()};
            const auto frameNumber{table[pageToReplaceIndex].getPageFrameNumber()};

            table[pageToReplaceIndex].freePage();
            table[newPageIndex].usePage(frameNumber);
        }

        bool isPageReferenced(const unsigned index)
        {
            return table[index].isReferenced();
        }

        bool isPageModified(const unsigned index)
        {
            return table[index].isModified();
        }

        void resetPageReference(const unsigned index)
        {
            table[index].resetReferencedBit();
        }

        void resetPageModification(const unsigned index)
        {
            table[index].resetModifiedBit();
        }

        void resetAllPageReferences()
        {
            for (unsigned i{0}; i < numberOfRecords; ++i)
            {
                resetPageReference(i);
            }
        }

        unsigned getPageFrameNumber(const unsigned index)
        {
            return table[index].getPageFrameNumber();
        }

        std::optional<unsigned> getFirstReferencedPage()
        {
            for (unsigned i{0}; i < numberOfRecords; ++i)
            {
                if (isPageReferenced(i))
                {
                    return std::make_optional<unsigned>(i);
                }
            }
            return std::nullopt;
        }

    private:
        std::optional<unsigned> calculatePhysicalAddress(const DefinedVirtualAddress &virtualAddress)
        {
            const auto index{virtualAddress.getIndex()};
            const auto offset{virtualAddress.getOffset()};
            const auto pageFrameNumber{table[index].getPageFrameNumber()};

            return std::make_optional<unsigned>((pageFrameNumber << NumberOfBits<pageSize>::value) + offset);
        }

        DefinedPageTableEntry table[numberOfRecords];
    };
}