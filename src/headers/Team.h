#pragma once

#include <vector>

class AlifeAgent;

class Team {

protected:
    std::vector<AlifeAgent*> AllAgents;
    AlifeAgent* Leader;

public:
    // @return All agents part of the team.
    std::vector<AlifeAgent*> GetAllAgents() const;

    // @return `true` if the team contains the agent, `false` if not.
    bool ContainsAgent(AlifeAgent* Agent) const;

    // @return `true` if the agent was added, `false` if not (if the team already contains the agent).
    bool AddAgent(AlifeAgent* Agent);
    
    // @return `true` if the agent was removed, `false` if not.
    bool RemoveAgent(const AlifeAgent* Agent);

    void SetTeamLeader(AlifeAgent* Agent);

};