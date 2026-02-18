#pragma once

#include "../interfaces/IZone.h"
#include "Team.h"
#include "Factions.h"
#include "../util/Structures.h"

#include <string>
#include <algorithm>

struct Inventory {
    int Ammo;
    int Food;
    int Water;
    int Bandages;

    void Zero() {
        *this = {};
    }
};

struct Stats {
    // Is this agent alive?
    bool Alive = true;

    // The amount of health this agent has left. <= 0 and killable will kill the agent.
    int Health = 100;

    // 0-1; chance to move when not following leader.
    double MovementSpeed = 0.5;

    // 0-1; higher strength: higher odds of winning a close-quarters fight.
    double Strength = 0.5;

    // 0-1; higher skill: higher odds of winning a gunfight.
    double Skill = 0.5;

    // 0-1; how aware the agent is when an ally is attacked.
    double Awareness = 0.5;

    // 0-1; how likley the agent is to see an enemy.
    double Sight = 0.5;

    // 0-1; how happy this agent is. Lower is higher chance to run and less performance.
    double Morale = 0.5;

    // How much health does this agent regenerate per tick?
    int Regen = 0;

    // How much health should this agent lose per tick?
    int BleedAmount = 0;
};

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

    // Is the agent killable?
    bool killable;

    // Can this agent move?
    bool moveable;

    // The agent's stats.
    Stats stats;

    // The loot the Agent has.
    Inventory inventory;

    // The max HP this agent can have.
    int maxHP;

    // Targeted agent
    AlifeAgent* target;
    
public:

    AlifeAgent(IZone* owningZone, Stats initialStats);

    virtual void Update();
    virtual void OnAttacked([[maybe_unused]] AlifeAgent* Attacker) {
        if(GetAgentTeam()) {
            for(AlifeAgent* ag : GetAgentTeam()->GetAllAgents())
                if(ag != this)
                    ag->OnAllyAttacked(Attacker);
        }
    }
    virtual void OnAllyAttacked([[maybe_unused]] AlifeAgent* Attacker) {}

    Vector2& GetLocation() { return position; }

    int GetAgentID() const { return agentID; }

    std::string GetAgentName() const { return agentName; }
    void SetAgentName(std::string n) { agentName = n; }

    Factions::Faction GetAgentFaction() const { return agentFaction; }
    void SetAgentFaction(Factions::Faction f) { agentFaction = f; }

    Team* GetAgentTeam() const { return team; }
    void SetAgentTeam(Team* t) { team = t; }

    bool IsAlive() const { return stats.Alive; }
    void SetAlive(bool a) { stats.Alive = a; }

    int GetHealth() const { return stats.Health; }
    void SetHealth(int h) { stats.Health = h; }

    int GetRegeneration() const { return stats.Regen; }
    void SetRegeneration(int r) { stats.Regen = r; }

    bool IsKillable() const { return killable; }
    void SetKillable(bool k) { killable = k; }

    bool IsMoveable() const { return moveable; }
    void SetMoveable(bool m) { moveable = m; }

    // Acts as a deviation for chance to move.
    int GetMoveSpeed() const { return stats.MovementSpeed; }

    // Higher strength: higher odds of winning a close-quarters fight.
    int GetStrength() const { return stats.Strength; }

    // Higher skill: higher odds of winning a gunfight.
    int GetSkill() const { return stats.Skill; }

    // 0-1; how aware the agent is when an ally is attacked.
    float GetAwareness() const { return stats.Awareness; }

    // 0-1; how likley the agent is to see an enemy.
    float GetSight() const { return stats.Sight; }

    // 0-1; how happy this agent is. Lower is higher chance to run and less performance.
    bool GetMorale() const { return stats.Morale; }
    void AdjustMorale(double m) { stats.Morale = std::clamp(stats.Morale + m, 0.0, 1.0); }

    // How much health should this agent lose per tick?
    int GetBleeding() const { return stats.BleedAmount; }
    void SetBleeding(int b) { stats.BleedAmount = b; };
    void StopBleeding() { stats.BleedAmount = 0; }

    // The loot the agent has.
    Inventory& GetInventory() { return inventory; };

    // Set all values in the inventory to zero.
    void ZeroInventory() { inventory.Zero(); };

    // Who is this agent attacking?
    AlifeAgent* GetTarget() const { return target; }
    void SetTarget(AlifeAgent* t) { target = t; }

    /**
     * Remove this agent from its team.
     * 
     * @return `true` if the agent detached or not.
     */
    bool DetachFromTeam();

    // Make this agent the leader of its team.
    void PromoteToLeader();
    
    void Hurt(int Damage, AlifeAgent* Attacker);
    void Heal(int Amount);

    void Kill();


    virtual ~AlifeAgent() = default;
};