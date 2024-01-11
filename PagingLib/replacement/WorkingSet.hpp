#pragma once

#include "PageReplacementI.hpp"

namespace PL::replacement
{
    template <unsigned numberOfFrames, typename PageTable>
    class WorkingSet : public PageReplacementI
    {

        static constexpr unsigned workingSetTime{200};

        struct FrameRecord
        {
            bool isFree{true};
            unsigned isReferenced{false};
            unsigned pageIndex{0};
            unsigned time{0};
        };

    public:
        WorkingSet(PageTable &pageTable) : pageTable{pageTable}
        {
        }

        std::optional<unsigned> getFrameForReplacement(const unsigned pageIndex) override
        {
            unsigned lowestTimeValue{frames[0].time};
            unsigned lowestTimeIndex{0};

            for (unsigned i{0}; i < numberOfFrames; ++i)
            {
                if (frames[i].isFree)
                {
                    return getFreeFrame(i, pageIndex);
                }

                if (frames[i].isReferenced)
                {
                    continue;
                }

                if ((timer - frames[i].time) > workingSetTime)
                {
                    return swapPageFrame(i, pageIndex);
                }
                else
                {
                    if (frames[i].time < lowestTimeValue)
                    {
                        lowestTimeValue = frames[i].time;
                        lowestTimeIndex = i;
                    }
                }
            }

            return swapPageFrame(lowestTimeIndex, pageIndex);
        }

        void updateStatusAtEnd() override
        {
            for (unsigned i{0}; i < numberOfFrames; ++i)
            {
                if (pageTable.isPageReferenced(frames[i].pageIndex))
                {
                    frames[i].time = timer;
                    frames[i].isReferenced = true;
                }
                else
                {
                    frames[i].isReferenced = false;
                }
            }

            timer += 1;
            pageTable.resetAllPageReferences();
        }

    private:
        std::optional<unsigned> getFreeFrame(const unsigned frameIndex, const unsigned pageIndex)
        {
            frames[frameIndex].isFree = false;
            frames[frameIndex].pageIndex = pageIndex;
            frames[frameIndex].time = timer;
            frames[frameIndex].isReferenced = true;

            return std::nullopt;
        }

        std::optional<unsigned> swapPageFrame(const unsigned frameIndex, const unsigned pageIndex)
        {
            const auto erasedPageIndex{frames[frameIndex].pageIndex};

            frames[frameIndex].pageIndex = pageIndex;
            frames[frameIndex].time = timer;
            frames[frameIndex].isReferenced = true;

            return std::make_optional<unsigned>(erasedPageIndex);
        }

        unsigned timer{0};
        FrameRecord frames[numberOfFrames];
        PageTable &pageTable;
    };
}