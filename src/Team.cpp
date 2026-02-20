#include "headers/Team.h"

#include <algorithm>

std::vector<AlifeAgent*> Team::GetAllAgents() const
{
    return Members;
}

bool Team::ContainsAgent(AlifeAgent* Agent) const
{
    auto it = std::find(Members.begin(), Members.end(), Agent);
    return it != Members.end();
}

bool Team::AddAgent(AlifeAgent* Agent)
{
    if(!ContainsAgent(Agent)) {
        Members.push_back(Agent);
        return true;
    }

    return false;
}

bool Team::RemoveAgent(const AlifeAgent* Agent)
{
    auto it = std::find(Members.begin(), Members.end(), Agent);
    if (it == Members.end())
        return false;

    *it = Members.back();
    Members.pop_back();

    if (IsEmpty())
    {
        SetTeamLeader(nullptr);
        return true;
    }

    if(Leader == Agent)
    {
        Leader = Members.front();
    }

    return true;
}


void Team::SetTeamLeader(AlifeAgent* Agent)
{
    Leader = Agent;
}