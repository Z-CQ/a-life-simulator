#include "Factions.h"

#include <iostream>

namespace Factions {

    Attitude relations[FactionCount][FactionCount];

    void InitFactions() {
        for (int i = 0; i < FactionCount; ++i)
            for (int j = 0; j < FactionCount; ++j)
                relations[i][j] = Attitude::Neutral;

        // Tell the mutants to hate everyone and vice versa.
        for(int i = 1; i < FactionCount; ++i) {
            relations[0][i] = Attitude::Hostile;
            relations[i][0] = Attitude::Hostile;
        }

        /* Loner Relations */
        relations[(int)Faction::Loner][(int)Faction::Bandit] = Attitude::Hostile;

        /* Bandit Relations */
        relations[(int)Faction::Bandit][(int)Faction::Loner] = Attitude::Hostile;
        relations[(int)Faction::Bandit][(int)Faction::Duty] = Attitude::Hostile;

        /* Freedom Relations */
        relations[(int)Faction::Freedom][(int)Faction::Duty] = Attitude::Hostile;

        /* Duty Relations */
        relations[(int)Faction::Duty][(int)Faction::Freedom] = Attitude::Hostile;
        relations[(int)Faction::Duty][(int)Faction::Bandit] = Attitude::Hostile;
    }

    Attitude GetRelation(Faction a, Faction b) {
        return relations[(int) a][(int) b];
    }

    ftxui::Color ResolveFactionColor(Faction fac) {
        switch(fac)
        {
            case Faction::Freedom:
                return ftxui::Color::Green;
            case Faction::Duty:
                return ftxui::Color::Red;
            case Faction::Bandit:
                return ftxui::Color::GrayDark;
            case Faction::Loner:
                return ftxui::Color::Yellow1;
            case Faction::Mutant:
                return ftxui::Color::RosyBrown;
            default:
                return ftxui::Color::White;
        }
    }

    std::string ResolveFactionName(Faction fac)
    {
        switch(fac)
        {
            case Faction::Freedom:
                return "[FREEDOM]";
            case Faction::Duty:
                return "[DUTY]";
            case Faction::Bandit:
                return "[BANDIT]";
            case Faction::Loner:
                return "[LONER]";
            case Faction::Mutant:
                return "[MUTANT]";
            default:
                return "";
        }
    }

}
