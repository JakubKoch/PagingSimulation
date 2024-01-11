#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <iostream>

#include "PageReplacementI.hpp"

namespace PL::replacement
{
    template <
        unsigned numberOfReferenceUnits,
        unsigned numberOfFrames,
        unsigned numberOfRecords,
        typename PageTable>
    class Optimal : public PageReplacementI
    {
        struct FrameRecord
        {
            bool isFree{true};
            unsigned pageIndex{0};
        };

        struct PageReferenceInfo
        {
            unsigned numberOfIteration{0};
            std::shared_ptr<PageReferenceInfo> next{nullptr};
        };

    public:
        Optimal(PageTable &pageTable) : pageTable{pageTable}
        {
            std::ifstream file{std::filesystem::current_path().parent_path().append(filename)};

            if (!file)
            {
                std::cout << "No optimal data file.\n";
                std::cout << "Run simulation with other algorihtm to generate it.\n";
            }
            else
            {
                readReferenceInfo(file);
            }

            file.close();
        }

        std::optional<unsigned> getFrameForReplacement(const unsigned pageIndex) override
        {
            unsigned latestIterationNumber{0};
            unsigned latestPageIndex{0};

            for (unsigned i{0}; i < numberOfFrames; ++i)
            {
                if (frames[i].isFree)
                {
                    frames[i].isFree = false;
                    frames[i].pageIndex = pageIndex;
                    return std::nullopt;
                }

                const auto &nextIterationNumber{getNextIterationNumber(i)};

                if (!nextIterationNumber.has_value())
                {
                    return swapPageFrame(i, pageIndex);
                }

                if (nextIterationNumber.value() > latestIterationNumber)
                {
                    latestIterationNumber = nextIterationNumber.value();
                    latestPageIndex = i;
                }
            }

            return swapPageFrame(latestPageIndex, pageIndex);
        }

        void updateStatusAtEnd() override
        {
            iterationNumber += 1;
        }

    private:
        void readReferenceInfo(std::istream &file)
        {
            unsigned pageNumber{0};
            unsigned iterationNumber{0};
            unsigned currentReferenceNumber{0};

            std::shared_ptr<PageReferenceInfo> currentNodes[numberOfRecords];

            for (unsigned i{0}; i < numberOfRecords; ++i)
            {
                pageReferenceInfo[i] = std::make_shared<PageReferenceInfo>(iterationNumber, nullptr);
                currentNodes[i] = pageReferenceInfo[i];
            }

            while (file >> pageNumber)
            {
                currentReferenceNumber += 1;

                currentNodes[pageNumber]->next = std::make_shared<PageReferenceInfo>(iterationNumber, nullptr);
                currentNodes[pageNumber] = currentNodes[pageNumber]->next;

                if (currentReferenceNumber == numberOfReferenceUnits)
                {
                    currentReferenceNumber = 0;
                    iterationNumber += 1;
                }
            }
        }

        std::optional<unsigned> getNextIterationNumber(const unsigned frameIndex)
        {
            const auto pageIndex{frames[frameIndex].pageIndex};

            while (pageReferenceInfo[pageIndex])
            {
                if (pageReferenceInfo[pageIndex]->numberOfIteration <= iterationNumber)
                {
                    pageReferenceInfo[pageIndex] = pageReferenceInfo[pageIndex]->next;
                }
                else
                {
                    return std::make_optional<unsigned>(pageReferenceInfo[pageIndex]->numberOfIteration);
                }
            }

            return std::nullopt;
        }

        std::optional<unsigned> swapPageFrame(const unsigned frameIndex, const unsigned pageIndex)
        {
            auto &frameForReplacement{frames[frameIndex]};
            const auto erasedPageIndex{frameForReplacement.pageIndex};

            frameForReplacement.pageIndex = pageIndex;

            return std::make_optional<unsigned>(erasedPageIndex);
        }

        const std::string filename{"OptimalData.txt"};
        unsigned iterationNumber{0};
        FrameRecord frames[numberOfFrames];
        std::shared_ptr<PageReferenceInfo> pageReferenceInfo[numberOfRecords];
        PageTable &pageTable;
    };
}