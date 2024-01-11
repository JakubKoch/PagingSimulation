#pragma once

#include <string>
#include <vector>

#include "SimulationFramework/reference/ReferenceTypeGenerator.hpp"
#include "FileManager.hpp"

namespace SF::measurement
{
    class OptimalDataHandler
    {
    public:
        void saveData(const unsigned reference);

        const std::string filename{"OptimalData.txt"};
        FileManager fileManager{filename};
    };
}