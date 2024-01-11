#pragma once

#include "PageReplacementI.hpp"

namespace PL::replacement
{
    template <unsigned numberOfFrames, typename PageTable>
    class NotRecentlyUsed : public PageReplacementI
    {
        enum class FrameClass : unsigned
        {
            Class0 = 0,
            Class1 = 1,
            Class2 = 2,
            Class3 = 3
        };

        struct FrameData
        {
            bool isFree{true};
            FrameClass frameClass{FrameClass::Class0};
            unsigned pageIndex{0};
        };

    public:
        NotRecentlyUsed(PageTable &pageTable) : pageTable{pageTable}
        {
        }

        std::optional<unsigned> getFrameForReplacement(const unsigned pageIndex) override
        {
            unsigned indexForReplacement{0};
            FrameClass lowestFrameClass{frames[indexForReplacement].frameClass};

            for (unsigned i{0}; i < numberOfFrames; ++i)
            {
                if (frames[i].isFree)
                {
                    frames[i].isFree = false;
                    frames[i].pageIndex = pageIndex;
                    return std::nullopt;
                }

                if (lowestFrameClass > frames[i].frameClass)
                {
                    lowestFrameClass = frames[i].frameClass;
                    indexForReplacement = i;
                }
            }

            return swapPageFrame(indexForReplacement, pageIndex);
        }

        void updateStatusAtEnd() override
        {
            for (unsigned i{0}; i < numberOfFrames; ++i)
            {
                if (frames[i].isFree)
                {
                    continue;
                }
                updateFrameStatus(i);
            }
            pageTable.resetAllPageReferences();
        }

    private:
        void updateFrameStatus(const unsigned index)
        {
            const unsigned pageIndex = frames[index].pageIndex;

            const bool isReferenced{pageTable.isPageReferenced(pageIndex)};
            const bool isModified{pageTable.isPageModified(pageIndex)};

            if (isReferenced && isModified)
            {
                frames[index].frameClass = FrameClass::Class3;
            }
            else if (isReferenced && !isModified)
            {
                frames[index].frameClass = FrameClass::Class2;
            }
            else if (!isReferenced && isModified)
            {
                frames[index].frameClass = FrameClass::Class1;
            }
            else
            {
                frames[index].frameClass = FrameClass::Class0;
            }
        }

        std::optional<unsigned> swapPageFrame(const unsigned frameIndex, const unsigned pageIndex)
        {
            auto &frameForReplacement{frames[frameIndex]};
            const auto erasedPageIndex{frameForReplacement.pageIndex};

            frameForReplacement.pageIndex = pageIndex;
            frameForReplacement.frameClass = pageTable.isPageModified(pageIndex)
                                                 ? FrameClass::Class3
                                                 : FrameClass::Class2;

            return std::make_optional<unsigned>(erasedPageIndex);
        }

        FrameData frames[numberOfFrames];
        PageTable &pageTable;
    };
}