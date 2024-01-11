#pragma once

#include "PageReplacementI.hpp"

namespace PL::replacement
{
    template <unsigned numberOfFrames, typename PageTable>
    class WSClock : public PageReplacementI
    {
        static constexpr unsigned workingSetTime{200};

        struct FrameNode
        {
            bool isFree{true};
            bool isReferenced{false};
            FrameNode *next{nullptr};
            unsigned time{0};
            unsigned pageIndex{0};
        };

    public:
        WSClock(PageTable &pageTable) : pageTable{pageTable}
        {
            for (unsigned i{0}; i < (numberOfFrames - 1); ++i)
            {
                nodes[i].next = nodes + i + 1;
            }

            nodes[numberOfFrames - 1].next = nodes;

            tick = nodes;
        }

        std::optional<unsigned> getFrameForReplacement(const unsigned pageIndex) override
        {
            FrameNode *start{tick};

            unsigned lowestTimeValue{tick->time};
            FrameNode *lowestTimePageTick{tick};


            do
            {
                if (tick->isFree)
                {
                    return getFreeFrame(pageIndex);
                }

                if (tick->isReferenced)
                {
                    tick = tick->next;
                    continue;
                }

                if ((timer - tick->time) > workingSetTime)
                {
                    return getOccupiedFrame(pageIndex);
                }
                else
                {
                    if (tick->time < lowestTimeValue)
                    {
                        lowestTimeValue = tick->time;
                        lowestTimePageTick = tick;
                    }
                    tick = tick->next;
                }

            } while (tick != start);

            tick = lowestTimePageTick;
            return getOccupiedFrame(pageIndex);
        }

        void updateStatusAtEnd() override
        {
            FrameNode *start{tick};

            do
            {
                if (pageTable.isPageReferenced(tick->pageIndex))
                {
                    tick->time = timer;
                    tick->isReferenced = true;
                }
                else
                {
                    tick->isReferenced = false;
                }

                tick = tick->next;

            } while (tick != start);

            timer += 1;
            pageTable.resetAllPageReferences();
        }

    private:
        std::optional<unsigned> getFreeFrame(const unsigned pageIndex)
        {
            tick->isFree = false;
            tick->pageIndex = pageIndex;
            tick->isReferenced = true;
            tick->time = timer;
            tick = tick->next;

            return std::nullopt;
        }

        std::optional<unsigned> getOccupiedFrame(const unsigned pageIndex)
        {
            const unsigned indexForErasedPage{tick->pageIndex};

            tick->pageIndex = pageIndex;
            tick->isReferenced = true;
            tick = tick->next;

            return std::make_optional<unsigned>(indexForErasedPage);
        }

        FrameNode *tick{nullptr};
        unsigned timer{0};
        FrameNode nodes[numberOfFrames];
        PageTable &pageTable;
    };
}