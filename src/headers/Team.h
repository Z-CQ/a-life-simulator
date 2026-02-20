#pragma once

#include <vector>
#include "../util/Structures.h"

class AlifeAgent;

class Team {

protected:
    std::vector<AlifeAgent*> Members;
    AlifeAgent* Leader;

    Vector2 origin;
    double angle;
    double spread;

public:
    Team() : Leader(nullptr), origin{0, 0}, angle(0.0), spread(1.0) {}


    // @return All agents part of the team.
    std::vector<AlifeAgent*> GetAllAgents() const;

    // @return `true` if the team contains the agent, `false` if not.
    bool ContainsAgent(AlifeAgent* Agent) const;

    // @return `true` if the agent was added, `false` if not (if the team already contains the agent).
    bool AddAgent(AlifeAgent* Agent);
    
    // @return `true` if the agent was removed, `false` if not.
    bool RemoveAgent(const AlifeAgent* Agent);

    void SetTeamLeader(AlifeAgent* Agent);

    bool IsEmpty() const { return Members.empty(); }

    AlifeAgent* GetTeamLeader() const { return Leader; }

    Vector2 GetOrigin() const { return origin; }
    void SetOrigin(Vector2 o) { origin = o; }

    double GetAngle() const { return angle; }
    void SetAngle(double a) { angle = a; }

    double GetSpread() const { return spread; }
    void SetSpread(double s) { spread = s; }

};