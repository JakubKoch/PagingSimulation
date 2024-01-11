#pragma once

#include <random>

namespace SF::reference
{
    enum class ReferenceType
    {
        Read = 0,
        Write = 1
    };

    class ReferenceTypeGenerator
    {
    public:
        ReferenceTypeGenerator(const double ratio);

        ReferenceType generate();
    private:
        std::mt19937 generator{};
        std::uniform_real_distribution<double> distribution{0.0, 1.0};

        double ratio;
    };
}