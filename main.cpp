#include "SimulationFramework/Simulation.hpp"

#define numberOfReferenceUnits 5
#define numberOfRecords 128
#define pageSize 64
#define numberOfFrames 32
#define numberOfIteration 5000

int main()
{
    SF::Simulation<
        numberOfReferenceUnits, numberOfRecords, pageSize, numberOfFrames, numberOfIteration>()
        .setWSClock()
        .run();

    return 0;
}