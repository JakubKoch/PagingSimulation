#pragma once

#include "PageReplacementI.hpp"

namespace PL::replacement
{
    template <unsigned numberOfFrames, unsigned numberOfRecords, typename PageTable>
    class Aging : public PageReplacementI
    {
        static constexpr unsigned numberOfRememberedIterations{30};

        struct FrameRecord
        {
            bool isFree{true};
            unsigned pageIndex{0};
        };

    public:
        Aging(PageTable &pageTable) : pageTable{pageTable}
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
                pageCounter[i] >>= 1;
                if (pageTable.isPageReferenced(i))
                {
                    unsigned refreshValue(1 << numberOfRememberedIterations);
                    pageCounter[i] += refreshValue;
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