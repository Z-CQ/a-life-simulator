#pragma once

#include "../interfaces/IZone.h"
#include "Team.h"
#include "Factions.h"
#include "../util/Structures.h"

#include <string>
#include <algorithm>
#include <cmath>

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

    // 0-1; chance to move
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

    // 0-1; how thirsty is this agent?
    double Thirst = 0.0;

    // 0-1; how hungry is this agent?
    double Hunger = 0.0;
};

// What does this agent want to do?
enum Intent {
    PATROL, // Moving in the direction of the leader
    RELAX, // Non-moving, safe, lower awareness, campfire
    COMBAT, // Targeting and fighting 'target'
    LOOTING, // Approaching last-killed 'target' and looking through the corpse's inventory
    RUNAWAY // Running due to low health, low morale, or high bleeding, effectively abandoning the team to rejoin later if alive
};

// How is this agent moving?
enum MovementState {
    MOVING, // Moving to intent?
    STILL, // Not moving?
    COVER, // Moving to cover?
    FLEEING, // Running?
};

class AlifeAgent {

private:
    static unsigned int nextID;

protected:
    IZone* zone;
    
    Intent currentIntent = PATROL;
    MovementState currentMovementState = MOVING;

    Vector2 position;
    Vector2 targetPosition{-1, -1};
    AgentDirection ownDirection;

    unsigned int agentID;
    std::string agentName;
    Factions::Faction agentFaction;
    Team* team = nullptr;

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
    AlifeAgent* target = nullptr;

    // Last targeted agent
    AlifeAgent* lastTarget = nullptr;

    double overflowX;
    double overflowY;
    
public:

    AlifeAgent(IZone* owningZone, Stats initialStats);

    /**
     * Tell the agent to search & lock on a nearby enemy. Search radius is 14 multiplied with the agent's sight.
     * The event will cancel if there's already a living target.
     * 
     * @return the Agent that was targeted
     */
    AlifeAgent* SearchForNearbyEnemy();

    virtual void Update() = 0;
    virtual void OnAttacked([[maybe_unused]] AlifeAgent* Attacker) {
        if(GetAgentTeam()) {
            for(AlifeAgent* ag : GetAgentTeam()->GetAllAgents())
                if(ag != this)
                    ag->OnAllyAttacked(Attacker);
        }
    }
    virtual void OnAllyAttacked([[maybe_unused]] AlifeAgent* Attacker) {}

    std::vector<AlifeAgent*> GetNearbyAgents(int radius);
    bool HasLineOfSight(AlifeAgent* target);

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
    double GetMoveSpeed() const { return stats.MovementSpeed; }

    // Higher strength: higher odds of winning a close-quarters fight.
    double GetStrength() const { return stats.Strength; }

    // Higher skill: higher odds of winning a gunfight.
    double GetSkill() const { return stats.Skill; }

    // 0-1; how aware the agent is when an ally is attacked.
    double GetAwareness() const { return stats.Awareness; }

    // 0-1; how likley the agent is to see an enemy.
    double GetSight() const { return stats.Sight; }

    // 0-1; how happy this agent is. Lower is higher chance to run and less performance.
    double GetMorale() const { return stats.Morale; }
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
    void SetTarget(AlifeAgent* t) {
        target = t;
        if(!target)
            return;
        std::string ownFac = Factions::ResolveFactionName(GetAgentFaction());
        std::string enemyFac = Factions::ResolveFactionName(GetTarget()->GetAgentFaction());
        zone->AddEntry(LogEntry(ownFac + " engaged " + enemyFac + ".", Factions::ResolveFactionColor(GetAgentFaction()))); 
    }

    AgentDirection GetDirection() const { return ownDirection; }
    void SetDirection(AgentDirection dir) { ownDirection = dir; }

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

    /**
     * Tell the agent to walk in its current direction.
     */
    void Move();

    /**
     * Move this agent by `x` and by `y` coordinates.
     * First, the agent shifts on the x axis. Then, on the y axis.
     * If the agent cannot move on an axis (an obstacle is in the way), it will skip that axis.
     * This does not test for obstacles from and to, only the target coordinates.
     * 
     * @param x How many x steps to move by.
     * @param y How many y steps to move by.
     */
    void MoveBy(int x, int y);


    virtual ~AlifeAgent() = default;
};