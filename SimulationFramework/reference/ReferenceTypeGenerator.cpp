#include "ReferenceTypeGenerator.hpp"

namespace SF::reference
{
    ReferenceTypeGenerator::ReferenceTypeGenerator(const double ratio)
        : ratio{ratio}
    {
    }

    ReferenceType ReferenceTypeGenerator::generate()
    {
        const double generatedNumber{distribution(generator)};

        if(generatedNumber < ratio)
        {
            return ReferenceType::Read;
        }
        else
        {
            return ReferenceType::Write;
        }
    }
}