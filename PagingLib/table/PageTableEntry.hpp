#pragma once

namespace PL::table
{
    template <unsigned pageFrameBitNumber>
    class PageTableEntry
    {
        static_assert(pageFrameBitNumber <= 32,
                      "Number of page frame bits exceeds uint32 size");

    public:
        inline void resetReferencedBit()
        {
            referenced = false;
        }

        inline void resetModifiedBit()
        {
            modified = false;
        }

        inline void setModifiedBit()
        {
            modified = true;
        }

        inline bool isReferenced()
        {
            return referenced;
        }

        inline bool isModified()
        {
            return modified;
        }

        inline bool isPresent()
        {
            return present;
        }

        inline void usePage(const unsigned frameNumber)
        {
            pageFrameNumber = frameNumber;
            present = true;
            referenced = false;
            modified = false;
        }

        inline void freePage()
        {
            pageFrameNumber = 0;
            present = false;
            referenced = false;
            modified = false;
        }

        inline unsigned getPageFrameNumber()
        {
            referenced = true;

            return pageFrameNumber;
        }

    private:
        unsigned pageFrameNumber : pageFrameBitNumber;
        bool referenced{false};
        bool modified{false};
        bool present{false};
    };
}