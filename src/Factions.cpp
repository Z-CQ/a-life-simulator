#include "Factions.h"

namespace Factions {

    Attitude relations[FactionCount][FactionCount];

    void InitFactions() {
        for (int i = 0; i < FactionCount; ++i)
            for (int j = 0; j < FactionCount; ++j)
                relations[i][j] = Attitude::Neutral;

        // Tell the mutants to hate everyone.
        for(int i = 1; i < FactionCount; ++i)
            relations[0][i] = Attitude::Hostile;

        relations[(int)Faction::Loner][(int)Faction::Bandit] = Attitude::Hostile;
        relations[(int)Faction::Bandit][(int)Faction::Loner] = Attitude::Hostile;
    }

    Attitude GetRelation(Faction a, Faction b) {
        return relations[(int) a][(int) b];
    }

}
