#pragma once

#include <array>

namespace SF::memory
{
    template <unsigned numberOfRecords, unsigned pageSize>
    class DiskMemory
    {
    public:
        DiskMemory()
        {
            for (unsigned i{0}; i < numberOfRecords; ++i)
            {
                for (unsigned j{0}; j < pageSize; ++j)
                {
                    writeData((i * pageSize) + j, i);
                }   
            }
        }

        unsigned readData(unsigned address)
        {
            return disk[address];
        }

        void writeData(unsigned address, unsigned data)
        {
            disk[address] = data;
        }

    private:
        std::array<unsigned, numberOfRecords * pageSize> disk{};
    };
}