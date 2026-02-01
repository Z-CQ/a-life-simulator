#include "headers/Zone.h"

double Zone::GenerateInRange(double min, double max)
{
    std::uniform_real_distribution<double> dist(min, max);
    return dist(eng);
}

void Zone::Populate(int Stalkers, int Mutants, int Deviation)
{
    Factions::InitFactions();


    int finalStalkerCount = Stalkers + GenerateInRange(-Deviation, Deviation);
    int finalMutantCount = Mutants + GenerateInRange(-Deviation, Deviation);

    for(int i = 0; i < finalStalkerCount; i++)
    {
        AlifeAgent* agent = new AlifeAgent();
    }

}

void Zone::Update()
{

}