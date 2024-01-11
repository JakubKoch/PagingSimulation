#pragma once

#include <utility>

#include "AddressGenerator.hpp"
#include "ReferenceTypeGenerator.hpp"

namespace SF::reference
{
    class ReferenceUnit
    {
        static constexpr double modifiedRatio{0.8};

        public:
            ReferenceUnit(const unsigned minAddress = 0, const unsigned maxAddress = 0, const double ratio = modifiedRatio);

            std::pair<unsigned, ReferenceType> generateReference();

        private:
            ReferenceTypeGenerator referenceTypeGenerator;
            AddressGenerator addressGenerator;
    };
}