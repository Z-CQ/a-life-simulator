#include "headers/Zone.h"

double Zone::GenerateInRange(double min, double max)
{
    std::uniform_real_distribution<double> dist(min, max);
    return dist(eng);
}

void Zone::Populate(int Stalkers, int Mutants, int Deviation)
{
    Factions::InitFactions();

    // Determine how many stalkers & mutants we want.
    // int finalStalkerCount = Stalkers + GenerateInRange(-Deviation, Deviation);
    // int finalMutantCount = Mutants + GenerateInRange(-Deviation, Deviation);

    // // How many stalkers at most do we want per team?
    // int stalkersPerTeam = 4;

    // // How many mutants at most do we want per team?
    // int mutantsPerTeam = 3;

    // // Calculate how many teams we want.
    // int numStalkerTeams = (finalStalkerCount + stalkersPerTeam - 1) / stalkersPerTeam;


    // for(int i = 0; i < finalStalkerCount; i++)
    // {

    // }

}

void Zone::Update()
{

}