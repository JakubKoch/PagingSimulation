#pragma once

#include <chrono>
#include <string>

#include "FileManager.hpp"

namespace SF::measurement
{
    class MeasurementHandler
    {
        using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

        template <typename TimeType>
        struct TimeMeasurements
        {
            TimePoint startTime{};
            TimePoint stopTime{};
            TimeType totalTime{0};
        };

    public:
        MeasurementHandler();

        void startAlgorithmTimeMeasurement();
        void endAlgorithmTimeMeasurement();
        void startUpdateTimeMeasurements();
        void endUpdateTimeMeasurements();
        void addPageFault();
        void saveData();

    private:
        template <typename TimeType>
        void calculateTime(TimeMeasurements<TimeType> &timeMeasurements)
        {
            auto duration = duration_cast<TimeType>(timeMeasurements.stopTime - timeMeasurements.startTime);
            timeMeasurements.totalTime += duration;
        }

        TimeMeasurements<std::chrono::nanoseconds> algorithmTimeMeasurements;
        TimeMeasurements<std::chrono::microseconds> runTimeMeasurements;
        unsigned numberOfPageFaults{0};
        unsigned numberOfIteration{0};

        const std::string filename{"Measurements.csv"};
        FileManager fileManager{filename};
    };
}