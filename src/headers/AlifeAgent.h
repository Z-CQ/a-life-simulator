#pragma once

#include "../interfaces/IZone.h"
#include "Team.h"
#include "Factions.h"
#include "../util/Structures.h"

#include <string>

class AlifeAgent {

private:
    static unsigned int nextID;

protected:
    IZone* zone;

    Vector2 position;

    unsigned int agentID;
    std::string agentName;
    Factions::Faction agentFaction;
    Team* team;

    // The amount of health this agent has left. <= 0 and killable will kill the agent.
    int health;

    // Is the agent killable?
    bool killable;

    // Can this agent move?
    bool moveable;

    // Acts as a deviation for chance to move.
    int moveSpeed;

    // Higher strength: higher odds of winning a close-quarters fight.
    int strength;

    // Higher skill: higher odds of winning a gunfight.
    int skill;

    // 0-1; how aware the agent is when an ally is attacked.
    float awareness;

    // 0-1; how likley the agent is to see an enemy.
    float sight;

    // Targeted agent
    AlifeAgent* target;
    
public:

    AlifeAgent();

    virtual void Update();

    Vector2& GetLocation() { return position; }

    int GetAgentID() const { return agentID; }

    std::string GetAgentName() const { return agentName; }
    void SetAgentName(std::string n) { agentName = n; }

    Factions::Faction GetAgentFaction() const { return agentFaction; }
    void SetAgentFaction(Factions::Faction f) { agentFaction = f; }

    Team* GetAgentTeam() const { return team; }
    void SetAgentTeam(Team* t) { team = t; }

    int GetHealth() const { return health; }
    void SetHealth(int h) { health = h; }

    bool IsKillable() const { return killable; }
    void SetKillable(bool k) { killable = k; }

    bool IsMoveable() const { return moveable; }
    void SetMoveable(bool m) { moveable = m; }

    // Acts as a deviation for chance to move.
    int GetMoveSpeed() const { return moveSpeed; }
    void SetMoveSpeed(int s) { moveSpeed = s; }

    // Higher strength: higher odds of winning a close-quarters fight.
    int GetStrength() const { return strength; }
    void SetStrength(int s) { strength = s; }

    // Higher skill: higher odds of winning a gunfight.
    int GetSkill() const { return skill; }
    void SetSkill(int s) { skill = s; }

    // 0-1; how aware the agent is when an ally is attacked.
    float GetAwareness() const { return awareness; }
    void SetAwareness(float a) { awareness = a; }

    // 0-1; how likley the agent is to see an enemy.
    float GetSight() const { return sight; }
    void SetSight(float s) { sight = s; }

    AlifeAgent* GetTarget() const { return target; }
    void SetTarget(AlifeAgent* t) { target = t; }

    void SetZone(IZone* newZone) { zone = newZone; }

    // Remove this agent from its team.
    void DetachFromTeam();

    // Make this agent the leader of its team.
    void PromoteToLeader();

    

};