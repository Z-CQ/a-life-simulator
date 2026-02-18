#pragma once

#include "AlifeAgent.h"

// What does this agent want to do?
enum Intent {
    PATROL, // Moving in the direction of the leader
    RELAX, // Non-moving, safe, lower awareness, campfire
    COMBAT, // Targeting and fighting 'target'
    LOOTING, // Approaching last-killed 'target' and looking the corpse
    RUNAWAY // Running due to low health, low morale, or high bleeding, effectively abandoning the team to rejoin later if alive
};

// How is this agent moving?
enum MovementState {
    MOVING, // Moving to intent?
    STILL, // Not moving?
    COVER, // Moving to cover?
    FLEEING, // Running?
};

class Stalker : public AlifeAgent {

private:
    Intent currentIntent;
    MovementState currentMovementState;

public:

    Stalker(IZone* owningZone, Stats initialStats) : AlifeAgent(owningZone, initialStats) {}

    virtual void Update() override;
    virtual void OnAttacked(AlifeAgent* Attacker) override;
    virtual void OnAllyAttacked(AlifeAgent* Attacker) override;

    /**
     * Use a bandage to heal.
     * 
     * @return `true` if a bandage was used, `false` if not (they were fully-healed or out of bandages).
     */
    bool UseBandage();

};