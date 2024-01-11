#pragma once

#include <optional>

#include "PagingLib/table/PageTable.hpp"
#include "PagingLib/table/VirtualAddress.hpp"

namespace PL
{
    template <unsigned numberOfRecords, unsigned pageSize, typename PageReplacemant>
    class PageManager
    {
    public:
        using PageTable = table::PageTable<numberOfRecords, pageSize>;
        using DefinedVirtualAddress = PageTable::DefinedVirtualAddress;

        std::optional<unsigned> getPhysicalAddress(const unsigned address, const bool isModified)
        {
            const DefinedVirtualAddress virtualAddress{address};
            return pageTable.getPhysicalAddress(virtualAddress, isModified);
        }

        std::optional<unsigned> handlePageFault(const unsigned address)
        {
            const DefinedVirtualAddress virtualAddress{address};
            return pageReplacemant.getFrameForReplacement(virtualAddress.getIndex());
        }

        void addPageFrameNumberToAddressPage(const unsigned address, const unsigned pageFrameNumber)
        {
            const DefinedVirtualAddress virtualAddress{address};
            pageTable.addPageFrameNumberFromAddress(virtualAddress, pageFrameNumber);
        }

        void replacePageFrameFromAddress(const unsigned address, const unsigned pageToReplaceIndex)
        {
            const DefinedVirtualAddress virtualAddress{address};
            pageTable.replacePageFrameFromAddress(virtualAddress, pageToReplaceIndex);
        }

        unsigned getPageIndexFromAddress(const unsigned address)
        {
            const DefinedVirtualAddress virtualAddress{address};
            return virtualAddress.getIndex();
        }

        unsigned getPageFrameNumber(const unsigned index)
        {
            return pageTable.getPageFrameNumber(index);
        }

        void updateFrameReplacementAfterOneIteration()
        {
            return pageReplacemant.updateStatusAfterOneIteration();
        }

        void updateFrameReplacementAtEnd()
        {
            return pageReplacemant.updateStatusAtEnd();
        }

        void resetPageReference(const unsigned index)
        {
            pageTable.resetPageReference(index);
        }

        void resetPageModification(const unsigned index)
        {
            pageTable.resetPageModification(index);
        }

        bool isPageModified(const unsigned index)
        {
            return pageTable.isPageModified(index);
        }

    private:
        PageTable pageTable{};
        PageReplacemant pageReplacemant{pageTable};
    };
}