#include "OptimalDataHandler.hpp"

namespace SF::measurement
{
    void OptimalDataHandler::saveData(const unsigned reference)
    {
        fileManager() << reference << " ";
    }
}