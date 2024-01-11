#pragma once

namespace PL::table
{
    template <unsigned indexBitNumber, unsigned offsetBitNumber>
    class VirtualAddress
    {
        static_assert(indexBitNumber + offsetBitNumber <= 32,
                      "Sume of index and offset bits exceeds uint32 size");

    public:
        explicit VirtualAddress(const unsigned address)
            : index{setIndex(address)},
              offset{setOffset(address)}
        {
        }

        inline unsigned getIndex() const
        {
            return index;
        }

        inline unsigned getOffset() const
        {
            return offset;
        }

    private:

        unsigned getMask(const unsigned numberOfOnes) const 
        {
            if (numberOfOnes == 0)
            {
                return 0;
            }

            unsigned mask{1};

            for (unsigned i{0}; i < (numberOfOnes - 1); ++i)
            {
                mask <<= 1;
                mask += 1;
            }
            return mask;
        }

        unsigned setIndex(const unsigned address) const
        {
            unsigned mask{getMask(indexBitNumber)};
            mask <<= offsetBitNumber;

            unsigned indexValue{address & mask};
            indexValue >>= offsetBitNumber;
            return indexValue;
        }

        unsigned setOffset(const unsigned address) const
        {
            unsigned mask{getMask(offsetBitNumber)};
            unsigned offsetValue{address & mask};
            return offsetValue;
        }

        const unsigned index : indexBitNumber;
        const unsigned offset : offsetBitNumber;
    };
}