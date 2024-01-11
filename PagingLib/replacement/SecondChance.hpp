#pragma once

#include "PageReplacementI.hpp"

namespace PL::replacement
{
    template <unsigned numberOfFrames, typename PageTable>
    class SecondChance : public PageReplacementI
    {
        struct FrameNode
        {
            bool isFree{true};
            bool isReferenced{false};
            FrameNode *next{nullptr};
            unsigned pageIndex{0};
        };

    public:
        SecondChance(PageTable &pageTable) : pageTable{pageTable}
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

        void updateStatusAtEnd() override
        {
            FrameNode *current{head};

            while (current != nullptr)
            {
                if (!current->isReferenced)
                {
                    const auto index{current->pageIndex};
                    if (pageTable.isPageReferenced(index))
                    {
                        current->isReferenced = true;
                    }
                }

                current = current->next;
            }
            pageTable.resetAllPageReferences();
        }

    private:
        std::optional<unsigned> getFreeFrame(FrameNode *node, const unsigned pageIndex)
        {
            node->isFree = false;
            node->pageIndex = pageIndex;
            head->isReferenced = true;

            return std::nullopt;
        }

        std::optional<unsigned> getOccupiedFrame(FrameNode *node, const unsigned pageIndex)
        {

            while (head->isReferenced)
            {
                head->isReferenced = false;
                addNodeToEnd(node);
                node = node->next;
            }

            return getNotReferencedFrame(node, pageIndex);
        }

        std::optional<unsigned> getNotReferencedFrame(FrameNode *node, const unsigned pageIndex)
        {
            const unsigned indexForErasedPage{head->pageIndex};

            head->pageIndex = pageIndex;
            head->isReferenced = true;
            addNodeToEnd(node);

            return std::make_optional<unsigned>(indexForErasedPage);
        }

        void addNodeToEnd(FrameNode *node)
        {
            node->next = head;
            head = head->next;
            node->next->next = nullptr;
        }

        FrameNode *head{nullptr};
        FrameNode nodes[numberOfFrames];
        PageTable &pageTable;
    };
}