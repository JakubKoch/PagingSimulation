#include "MeasurementHandler.hpp"

namespace SF::measurement
{
    MeasurementHandler::MeasurementHandler()
    {
        fileManager() << "iteration,";
        fileManager() << "algorithmTime,";
        fileManager() << "runTime,";
        fileManager() << "numberOfPageFaults\n";
    }

    void MeasurementHandler::startAlgorithmTimeMeasurement()
    {
        algorithmTimeMeasurements.startTime = std::chrono::high_resolution_clock::now();
    }

    void MeasurementHandler::endAlgorithmTimeMeasurement()
    {
        algorithmTimeMeasurements.stopTime = std::chrono::high_resolution_clock::now();
        calculateTime(algorithmTimeMeasurements);
    }

    void MeasurementHandler::startUpdateTimeMeasurements()
    {
        runTimeMeasurements.startTime = std::chrono::high_resolution_clock::now();
    }

    void MeasurementHandler::endUpdateTimeMeasurements()
    {
        runTimeMeasurements.stopTime = std::chrono::high_resolution_clock::now();
        calculateTime(runTimeMeasurements);
    }

    void MeasurementHandler::addPageFault()
    {
        numberOfPageFaults += 1;
    }

    void MeasurementHandler::saveData()
    {
        fileManager() << numberOfIteration << ",";
        fileManager() << algorithmTimeMeasurements.totalTime.count() << ",";
        fileManager() << runTimeMeasurements.totalTime.count() << ",";
        fileManager() << numberOfPageFaults << "\n";

        numberOfIteration += 1;
    }
}