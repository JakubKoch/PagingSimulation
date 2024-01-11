#pragma once

#include <optional>

namespace PL::replacement
{
    class PageReplacementI
    {
    public:
        virtual std::optional<unsigned> getFrameForReplacement(const unsigned pageIndex) { return std::nullopt; }
        virtual void updateStatusAfterOneIteration() {}
        virtual void updateStatusAtEnd() {}
    };
}