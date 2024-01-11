#pragma once

#include "PageReplacementI.hpp"

namespace PL::replacement
{
    template <unsigned numberOfFrames, typename PageTable>
    class LeastRecentlyUsed : public PageReplacementI
    {
        struct FrameNode
        {
            bool isFree{true};
            FrameNode *next{nullptr};
            unsigned pageIndex{0};
        };

    public:
        LeastRecentlyUsed(PageTable &pageTable) : pageTable{pageTable}
        {
            for (unsigned i{0}; i < (numberOfFrames - 1); ++i)
            {
                nodes[i].next = nodes + i + 1;
            }

            head = nodes;
        }

        std::optional<unsigned> getFrameForReplacement(const unsigned pageIndex) override
        {

            FrameNode *current{head};

            while (current->next != nullptr)
            {
                if (current->isFree)
                {
                    return getFreeFrame(current, pageIndex);
                }

                current = current->next;
            }

            if (current->isFree)
            {
                return getFreeFrame(current, pageIndex);
            }

            return getOccupiedFrame(current, pageIndex);
        }

        void updateStatusAfterOneIteration() override
        {
            FrameNode *current{head};
            FrameNode *nodeWithPage{nullptr};
            const auto pageIndexOpt(pageTable.getFirstReferencedPage());

            if (!pageIndexOpt.has_value())
            {
                return;
            }

            const auto pageIndex(pageIndexOpt.value());

            while (current->next != nullptr)
            {
                if (current->next->pageIndex == pageIndex)
                {
                    nodeWithPage = current;
                }

                current = current->next;
            }

            if ((current->pageIndex == pageIndex) || (nodeWithPage == nullptr))
            {
                pageTable.resetPageReference(pageIndex);
                return;
            }

            current->next = nodeWithPage->next;
            nodeWithPage->next = current->next->next;
            current->next->next = nullptr;

            pageTable.resetPageReference(pageIndex);
        }

    private:
        std::optional<unsigned> getFreeFrame(FrameNode *node, const unsigned pageIndex)
        {
            node->isFree = false;
            node->pageIndex = pageIndex;

            return std::nullopt;
        }

        std::optional<unsigned> getOccupiedFrame(FrameNode *node, const unsigned pageIndex)
        {
            const unsigned indexForErasedPage{head->pageIndex};

            head->pageIndex = pageIndex;
            node->next = head;
            head = head->next;
            node->next->next = nullptr;

            return std::make_optional<unsigned>(indexForErasedPage);
        }

        FrameNode *head{nullptr};
        FrameNode nodes[numberOfFrames];
        PageTable &pageTable;
    };
}