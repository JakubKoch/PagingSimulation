#pragma once

#include <iostream>

#include "PagingLib/PageManager.hpp"
#include "SimulationFramework/memory/MemoryHandler.hpp"
#include "SimulationFramework/reference/ReferenceUnitStorage.hpp"
#include "SimulationFramework/measurement/MeasurementHandler.hpp"
#include "SimulationFramework/measurement/OptimalDataHandler.hpp"

namespace SF
{
    template <
        unsigned numberOfReferenceUnits,
        unsigned numberOfRecords,
        unsigned pageSize,
        unsigned numberOfFrames,
        unsigned numberOfIteration,
        typename Algorithm>
    class MainComponent
    {
        static constexpr unsigned numberOfIterationForReferenceChange{1000};

        using PageManager = PL::PageManager<numberOfRecords, pageSize, Algorithm>;
        using MemoryHandler = SF::memory::MemoryHandler<numberOfFrames, numberOfRecords, pageSize>;
        using ReferenceUnitStorage = SF::reference::ReferenceUnitStorage<numberOfReferenceUnits, numberOfRecords, pageSize>;
        using MeasurementHandler = SF::measurement::MeasurementHandler;
        using OptimalDataHandler = SF::measurement::OptimalDataHandler;

    public:
        void run()
        {
            for (unsigned it{0}; it < numberOfIteration; ++it)
            {
                std::cout << "{SF} MainComponent: Running " << it << " iteration: \n";
                measurementHandler.startUpdateTimeMeasurements();

                checkReferenceChange(it);
                runIteration();
                pageManager.updateFrameReplacementAtEnd();

                measurementHandler.endUpdateTimeMeasurements();
                measurementHandler.saveData();
            }
        }

    private:
        void checkReferenceChange(const unsigned iteration)
        {
            if ((iteration % numberOfIterationForReferenceChange) == 0)
            {
                referenceUnitStorage.changeReferences(iteration);
            }
        }

        void runIteration()
        {
            const auto &references{referenceUnitStorage.getReferences()};

            for (auto &reference : references)
            {
                unsigned physicalAddress{};
                const auto &virtualAddress{reference.first};
                const auto &isModified{static_cast<bool>(reference.second)};
                const auto &physicalAddressOpt{pageManager.getPhysicalAddress(virtualAddress, isModified)};

                std::cout << "{SF} MainComponent: Reference address: " << virtualAddress << " modified: " << isModified << "\n";

                if (!physicalAddressOpt.has_value())
                {
                    std::cout << "{SF} MainComponent: PAGE FAULT\n";

                    measurementHandler.addPageFault();
                    handlePageFault(virtualAddress);
                    physicalAddress = pageManager.getPhysicalAddress(virtualAddress, isModified).value();
                }
                else
                {
                    physicalAddress = physicalAddressOpt.value();
                }

                std::cout << "{SF} MainComponent: physicalAddress: " << physicalAddress << "\n";

                pageManager.updateFrameReplacementAfterOneIteration();
                optimalDataHandler.saveData(pageManager.getPageIndexFromAddress(virtualAddress));
            }
        }

        void handlePageFault(const unsigned virtualAddress)
        {
            const auto &freedPageIndex{getFreedPageIndex(virtualAddress)};

            if (freedPageIndex.has_value())
            {
                const unsigned index{freedPageIndex.value()};

                if (pageManager.isPageModified(index))
                {
                    std::cout << "{SF} MainComponent: saving\n";
                    
                    savePageOnDisk(index);
                }

                replacePageFrame(virtualAddress, index);
            }
            else
            {
                addPageFrame(virtualAddress);
            }
        }

        std::optional<unsigned> getFreedPageIndex(const unsigned virtualAddress)
        {
            measurementHandler.startAlgorithmTimeMeasurement();
            const auto &freedPageIndex{pageManager.handlePageFault(virtualAddress)};
            measurementHandler.endAlgorithmTimeMeasurement();

            return freedPageIndex;
        }

        void addPageFrame(const unsigned virtualAddress)
        {
            const unsigned pageIndex{pageManager.getPageIndexFromAddress(virtualAddress)};
            const unsigned frameIndex{memoryHandler.getFreeFrameNumber()};

            std::cout << "{SF} MainComponent: add page: " << frameIndex << "\n";

            memoryHandler.replaceFrame(frameIndex, pageIndex);
            pageManager.addPageFrameNumberToAddressPage(virtualAddress, frameIndex);
        }

        void savePageOnDisk(const unsigned pageToSave)
        {
            const unsigned frameIndex{pageManager.getPageFrameNumber(pageToSave)};

            memoryHandler.saveFrame(frameIndex, pageToSave);
            pageManager.resetPageModification(pageToSave);
            pageManager.resetPageReference(pageToSave);
        }

        void replacePageFrame(const unsigned virtualAddress, const unsigned pageToReplace)
        {
            const unsigned frameIndex{pageManager.getPageFrameNumber(pageToReplace)};

            std::cout << "{SF} MainComponent: replace frameIndex: " << frameIndex << " pageToSave: " << pageToReplace << "\n";

            memoryHandler.replaceFrame(frameIndex, pageToReplace);
            pageManager.replacePageFrameFromAddress(virtualAddress, pageToReplace);
            pageManager.resetPageReference(pageToReplace);
        }

        PageManager pageManager{};
        ReferenceUnitStorage referenceUnitStorage{};
        OptimalDataHandler optimalDataHandler{};
        MemoryHandler memoryHandler{};
        MeasurementHandler measurementHandler{};
    };
}