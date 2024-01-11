#pragma once

#include "PageReplacementI.hpp"

namespace PL::replacement
{
    template <unsigned numberOfFrames, typename PageTable>
    class Clock : public PageReplacementI
    {
        struct FrameNode
        {
            bool isFree{true};
            bool isReferenced{false};
            FrameNode *next{nullptr};
            unsigned pageIndex{0};
        };

    public:
        Clock(PageTable &pageTable) : pageTable{pageTable}
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
            while (true)
            {
                if (tick->isFree)
                {
                    return getFreeFrame(pageIndex);
                }

                if (tick->isReferenced)
                {
                    tick->isReferenced = false;
                    tick = tick->next;
                }
                else
                {
                    return getOccupiedFrame(pageIndex);
                }
            }
        }

        void updateStatusAtEnd() override
        {
            FrameNode *current{tick};

            do
            {
                current = current->next;
                if (!current->isReferenced)
                {
                    const auto index{current->pageIndex};
                    if (pageTable.isPageReferenced(index))
                    {
                        current->isReferenced = true;
                    }
                }

            } while (current != tick);
            pageTable.resetAllPageReferences();
        }

    private:
        std::optional<unsigned> getFreeFrame(const unsigned pageIndex)
        {
            tick->isFree = false;
            tick->pageIndex = pageIndex;
            tick->isReferenced = true;
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
        FrameNode nodes[numberOfFrames];
        PageTable &pageTable;
    };
}