#pragma once

#include <ftxui/screen/color.hpp>

namespace Factions {
    enum class Faction {
        Mutant,
        Loner,
        Bandit,
        Duty,
        Freedom,
        Count
    };

    enum class Attitude {
        Hostile = -1,
        Neutral = 0,
        Friendly = 1
    };

    constexpr int FactionCount = (int)Faction::Count;
    extern Attitude relations[FactionCount][FactionCount];

    /**
     * Initialize all factions' relations
     */
    void InitFactions();

    /**
     * Retrieve the relation between two factions
     * 
     * @param a The first faction to look at
     * @param b The second faction to look at
     * 
     * @return The attitude between both factions
     */
    Attitude GetRelation(Faction a, Faction b);

    ftxui::Color ResolveFactionColor(Faction fac);

}