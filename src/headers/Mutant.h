#pragma once

#include "AlifeAgent.h"

class Mutant : public AlifeAgent 
{

public:
    Mutant(IZone* owningZone, Stats initialStats) : AlifeAgent(owningZone, initialStats) {}

    virtual void Update() override;
    virtual void OnAttacked(AlifeAgent* Attacker) override;
    virtual void OnAllyAttacked(AlifeAgent* Attacker) override;

};