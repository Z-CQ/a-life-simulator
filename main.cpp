#include "src/headers/Zone.h"

#include <stdio.h>
#include <iostream>
#include <string>

int stringToInt(std::string s)
{
    try
    {
        return std::stoi(s);
    } catch(std::invalid_argument)
    {
        std::cerr << "Argument \"" + s + "\" could not be converted into an integer." << std::endl;
        exit(-1);
    } catch(std::out_of_range)
    {
        std::cerr << "Argument \"" + s + "\" out of range." << std::endl;
        exit(-1);
    }
}

int main(int argc, char** argv)
{
    Zone zone{};

    int StalkerCount = 48;
    int MutantCount = 12;
    int CampfireCount = 12;
    int BuildingCount = 70;
    int GlobalDeviation = 8;
    int StalkerTeamSize = 4;
    int MutantTeamSize = 3;

    int TICK_SPEED_MS = 100;

    for(int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if(arg == "--help" || arg == "--h")
        {
            std::cout <<
                "\nUsage: " << argv[0] << " [options]\n\n"
                "Options:\n"
                "  --help --h           Show this message and end program\n"
                "  --stalkers           Set the desired amount of stalkers (default: 48)\n"
                "  --mutants            Set the desired amount of mutants (default: 12)\n"
                "  --campfires          Set the desired amount of campfires (default: 12)\n"
                "  --buildings          Set the desired amount of buildings (default: 70)\n"
                "  --deviation          Set the random deviation (neg and pos) to each value above (default: 8)\n"
                "  --stalker-team-size  Set the desired team size for stalkers"
                "  --mutant-team-size   Set the desired team size for mutants"
                "  --tick-speed         Set the tick speed in milliseconds (default: 100)\n"
                "\nNaturally, deviation should be the lowest value in the list of arguments to avoid unwanted behavior.\n"
                "\nExample:\n"
                "  " << argv[0] << " --stalkers 32 --mutants 12 --deviation 4\n\n";
            return 0;
        } else if(arg == "--stalkers")
        {
            StalkerCount = stringToInt(argv[++i]);
        } else if(arg == "--mutants")
        {
            MutantCount = stringToInt(argv[++i]);
        } else if(arg == "--campfires")
        {
            CampfireCount = stringToInt(argv[++i]);
        } else if(arg == "--buildings")
        {
            BuildingCount = stringToInt(argv[++i]);
        } else if(arg == "--deviation")
        {
            GlobalDeviation = stringToInt(argv[++i]);
        } else if(arg == "--stalker-team-size")
        {
            StalkerTeamSize = stringToInt(argv[++i]);
        } else if(arg == "--mutant-team-size")
        {
            MutantTeamSize = stringToInt(argv[++i]);
        } else if(arg == "--tick-speed")
        {
            TICK_SPEED_MS = stringToInt(argv[++i]);
        } else {
            std::cerr << "Unknown argument: " << argv[i] << ". Terminating." << std::endl;
            exit(-1);
        }
    }

    zone.Populate(StalkerCount, MutantCount, CampfireCount, BuildingCount, GlobalDeviation, StalkerTeamSize, MutantTeamSize);

    // Simualation loop
    while (true) {
        zone.Update();

        std::this_thread::sleep_for(std::chrono::milliseconds(TICK_SPEED_MS));
    }
}