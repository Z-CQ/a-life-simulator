#pragma once

#include "AlifeAgent.h"



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