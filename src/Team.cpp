#include "headers/Team.h"

#include <algorithm>

std::vector<AlifeAgent*> Team::GetAllAgents() const
{
    return AllAgents;
}

bool Team::ContainsAgent(AlifeAgent* Agent) const
{
    auto it = std::find(AllAgents.begin(), AllAgents.end(), Agent);
    return it != AllAgents.end();
}

bool Team::AddAgent(AlifeAgent* Agent)
{
    if(!ContainsAgent(Agent))
        AllAgents.push_back(Agent);
}

bool Team::RemoveAgent(const AlifeAgent* Agent)
{
    auto it = std::find(AllAgents.begin(), AllAgents.end(), Agent);

    if(it == AllAgents.end())
        return false;

    *it = AllAgents.back();
    AllAgents.pop_back();

    return true;
}

void Team::SetTeamLeader(AlifeAgent* Agent)
{
    Leader = Agent;
}