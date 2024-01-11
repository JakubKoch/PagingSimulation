#pragma once

#include "PagingLib/replacement/Aging.hpp"
#include "PagingLib/replacement/Clock.hpp"
#include "PagingLib/replacement/FirstInFirstOut.hpp"
#include "PagingLib/replacement/LeastRecentlyUsed.hpp"
#include "PagingLib/replacement/NotFrequentlyUsed.hpp"
#include "PagingLib/replacement/NotRecentlyUsed.hpp"
#include "PagingLib/replacement/Optimal.hpp"
#include "PagingLib/replacement/SecondChance.hpp"
#include "PagingLib/replacement/WorkingSet.hpp"
#include "PagingLib/replacement/WSClock.hpp"

#include "MainComponent.hpp"

namespace SF
{
    template <
        unsigned numberOfReferenceUnits,
        unsigned numberOfRecords,
        unsigned pageSize,
        unsigned numberOfFrames,
        unsigned numberOfIteration>
    class Simulation
    {
        using PageTable = PL::table::PageTable<numberOfRecords, pageSize>;
        using Aging = PL::replacement::Aging<numberOfFrames, numberOfRecords, PageTable>;
        using Clock = PL::replacement::Clock<numberOfFrames, PageTable>;
        using FirstInFirstOut = PL::replacement::FirstInFirstOut<numberOfFrames, PageTable>;
        using LeastRecentlyUsed = PL::replacement::LeastRecentlyUsed<numberOfFrames, PageTable>;
        using NotFrequentlyUsed = PL::replacement::NotFrequentlyUsed<numberOfFrames, numberOfRecords, PageTable>;
        using NotRecentlyUsed = PL::replacement::NotRecentlyUsed<numberOfFrames, PageTable>;
        using Optimal = PL::replacement::Optimal<numberOfReferenceUnits, numberOfFrames, numberOfRecords, PageTable>;
        using SecondChance = PL::replacement::SecondChance<numberOfFrames, PageTable>;
        using WorkingSet = PL::replacement::WorkingSet<numberOfFrames, PageTable>;
        using WSClock = PL::replacement::WSClock<numberOfFrames, PageTable>;

        enum class Algorithms
        {
            Aging,
            Clock,
            FirstInFirstOut,
            LeastRecentlyUsed,
            NotFrequentlyUsed,
            NotRecentlyUsed,
            Optimal,
            SecondChance,
            WorkingSet,
            WSClock
        };

    public:
        void run()
        {
            chooseAlgorithm();
        }

        Simulation &setAging()
        {
            algoritm = Algorithms::Aging;
            return *this;
        }

        Simulation &setClock()
        {
            algoritm = Algorithms::Clock;
            return *this;
        }

        Simulation &setFirstInFirstOut()
        {
            algoritm = Algorithms::FirstInFirstOut;
            return *this;
        }

        Simulation &setLeastRecentlyUsed()
        {
            algoritm = Algorithms::LeastRecentlyUsed;
            return *this;
        }

        Simulation &setNotFrequentlyUsed()
        {
            algoritm = Algorithms::NotFrequentlyUsed;
            return *this;
        }

        Simulation &setNotRecentlyUsed()
        {
            algoritm = Algorithms::NotRecentlyUsed;
            return *this;
        }

        Simulation &setOptimal()
        {
            algoritm = Algorithms::Optimal;
            return *this;
        }

        Simulation &setSecondChance()
        {
            algoritm = Algorithms::SecondChance;
            return *this;
        }

        Simulation &setWorkingSet()
        {
            algoritm = Algorithms::WorkingSet;
            return *this;
        }

        Simulation &setWSClock()
        {
            algoritm = Algorithms::WSClock;
            return *this;
        }

    private:
        void chooseAlgorithm()
        {
            switch (algoritm)
            {
            case Algorithms::Aging:
                runMainComponent<Aging>();
                break;
            case Algorithms::Clock:
                runMainComponent<Clock>();
                break;
            case Algorithms::FirstInFirstOut:
                runMainComponent<FirstInFirstOut>();
                break;
            case Algorithms::LeastRecentlyUsed:
                runMainComponent<LeastRecentlyUsed>();
                break;
            case Algorithms::NotFrequentlyUsed:
                runMainComponent<NotFrequentlyUsed>();
                break;
            case Algorithms::NotRecentlyUsed:
                runMainComponent<NotRecentlyUsed>();
                break;
            case Algorithms::Optimal:
                runMainComponent<Optimal>();
                break;
            case Algorithms::SecondChance:
                runMainComponent<SecondChance>();
                break;
            case Algorithms::WorkingSet:
                runMainComponent<WorkingSet>();
                break;
            case Algorithms::WSClock:
                runMainComponent<WSClock>();
                break;
            }
        }

        template <typename Algorithm>
        void runMainComponent()
        {
            MainComponent<
                numberOfReferenceUnits, numberOfRecords, pageSize, numberOfFrames, numberOfIteration, Algorithm>()
                .run();
        }

        Algorithms algoritm{Algorithms::Clock};
    };
}