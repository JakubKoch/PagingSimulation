#pragma once

#include <array>

namespace SF::memory
{
    template <unsigned numberOfFrames, unsigned pageSize>
    class RandomAccessMemory
    {
    public:
        unsigned getFreeFrameNumber()
        {
            for (unsigned i{0}; i < numberOfFrames; ++i)
            {
                if (!takenFrames[i])
                {
                    takenFrames[i] = true;
                    return i;
                }
            }
            return 0;
        }

        unsigned readData(unsigned address)
        {
            return ram[address];
        }

        void writeData(unsigned address, unsigned data)
        {
            ram[address] = data;
        }

    private:
        std::array<bool, numberOfFrames> takenFrames{};
        std::array<unsigned, numberOfFrames * pageSize> ram{};
    };
}