#pragma once

#include "PageReplacementI.hpp"

namespace PL::replacement
{
    template <unsigned numberOfFrames, unsigned numberOfRecords, typename PageTable>
    class NotFrequentlyUsed : public PageReplacementI
    {
        struct FrameRecord
        {
            bool isFree{true};
            unsigned pageIndex{0};
        };

    public:
        NotFrequentlyUsed(PageTable &pageTable) : pageTable{pageTable}
        {
            for (unsigned i{0}; i < numberOfRecords; ++i)
            {
                pageCounter[i] = 0;
            }
        }

        std::optional<unsigned> getFrameForReplacement(const unsigned pageIndex) override
        {
            unsigned lowestCounterValue{pageCounter[frames[0].pageIndex]};
            unsigned lowestCounterIndex{0};

            for (unsigned i{0}; i < numberOfFrames; ++i)
            {
                if (frames[i].isFree)
                {
                    frames[i].isFree = false;
                    frames[i].pageIndex = pageIndex;
                    return std::nullopt;
                }

                if (pageCounter[frames[i].pageIndex] < lowestCounterValue)
                {
                    lowestCounterValue = pageCounter[frames[i].pageIndex];
                    lowestCounterIndex = i;
                }
            }

            return swapPageFrame(lowestCounterIndex, pageIndex);
        }

        void updateStatusAtEnd() override
        {
            for (unsigned i{0}; i < numberOfRecords; ++i)
            {
                if (pageTable.isPageReferenced(i))
                {
                    pageCounter[i] += 1;
                    pageTable.resetPageReference(i);
                }
            }
        }

    private:
        std::optional<unsigned> swapPageFrame(const unsigned frameIndex, const unsigned pageIndex)
        {
            auto &frameForReplacement{frames[frameIndex]};
            const auto erasedPageIndex{frameForReplacement.pageIndex};

            frameForReplacement.pageIndex = pageIndex;

            return std::make_optional<unsigned>(erasedPageIndex);
        }

        FrameRecord frames[numberOfFrames];
        unsigned pageCounter[numberOfRecords];
        PageTable &pageTable;
    };
}