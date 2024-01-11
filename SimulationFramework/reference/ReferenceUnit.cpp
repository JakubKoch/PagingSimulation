#include "ReferenceUnit.hpp"

namespace SF::reference
{
    ReferenceUnit::ReferenceUnit(const unsigned minAddress, const unsigned maxAddress, const double ratio)
        : addressGenerator{minAddress, maxAddress},
          referenceTypeGenerator{ratio}
    {
    }

    std::pair<unsigned, ReferenceType> ReferenceUnit::generateReference()
    {
        const auto address{addressGenerator.generate()};
        const auto referenceType{referenceTypeGenerator.generate()};

        return std::make_pair(address, referenceType);
    }
}