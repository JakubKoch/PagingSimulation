#pragma once

#include "DiskMemory.hpp"
#include "RandomAccessMemory.hpp"

namespace SF::memory
{
    template <
        unsigned numberOfFrames,
        unsigned numberOfRecords,
        unsigned pageSize>
    class MemoryHandler
    {
        using DefinedDiskMemory = DiskMemory<numberOfRecords, pageSize>;
        using DefinedRandomAccessMemory = RandomAccessMemory<numberOfFrames, pageSize>;

    public:
        void replaceFrame(const unsigned frameIndex, const unsigned pageIndex)
        {
            for (unsigned i{0}; i < pageSize; ++i)
            {
                const auto dataFromDisk{disk.readData(pageIndex * pageSize + i)};
                ram.writeData(frameIndex * pageSize + i, dataFromDisk);
            }
        }

        void saveFrame(const unsigned frameIndex, const unsigned pageIndex)
        {
            for (unsigned i{0}; i < pageSize; ++i)
            {
                const auto dataFromRam{ram.readData(frameIndex * pageSize + i)};
                disk.writeData(pageIndex * pageSize + i, dataFromRam);
            }
        }

        unsigned getFreeFrameNumber()
        {
            return ram.getFreeFrameNumber();
        }

    private:
        DefinedDiskMemory disk{};
        DefinedRandomAccessMemory ram{};
    };
}