#include "headers/Mutant.h"

#include <iostream>

void Mutant::Update()
{
    AlifeAgent::Update();

    if(!IsAlive())
        return;

    if(team)
    {
        if(team->GetTeamLeader() == this && zone->GenerateInRange(0.0, 1.0) <= zone->GenerateInRange(0.0, 0.2))
        {
            team->SetWalkDirection(AgentDirection{
                zone->GenerateInRange(-1.0, 1.0),
                zone->GenerateInRange(-1.0, 1.0)
            });
        }
    } else if(zone->GenerateInRange(0.0, 1.0) <= 0.1) {
        SetDirection(AgentDirection{
            zone->GenerateInRange(-1.0, 1.0),
            zone->GenerateInRange(-1.0, 1.0)
        });
    }

    switch(currentIntent)
    {
        case PATROL:
            SearchForNearbyEnemy();
            
            if(zone->GenerateInRange(0.0, 1.0) > GetMoveSpeed() * 0.3) // multiply move speed by 0.3 to artificially slow down patrol
                return;
                
            if(team)
                SetDirection(team->GetWalkDirection());

            Move();
            
            // If this agent locked onto something, switch to COMBAT.
            if(GetTarget()) {
                currentIntent = COMBAT;
            }
            
            break;
        case COMBAT:
            // If the target is nullptr or dead, we can switch to looting
            if(!GetTarget() || !GetTarget()->IsAlive())
            {
                currentIntent = PATROL;
                SetTarget(nullptr);
                targetPosition = Vector2{-1, -1};
                break;
            }
            targetPosition = GetTarget()->GetLocation();
            Move();

            double RandomChance = zone->GenerateInRange(0.0, 1.0);
            int Distance = Vector2::DistanceSquared(GetLocation(), targetPosition);
            bool CanStrike = (RandomChance <= GetStrength()) && Distance < 9 &&
                HasLineOfSight(GetTarget());

            if(CanStrike)
            {
                int Damage = std::round(zone->GenerateInRange(31.0, 48.0) * GetStrength());
                GetTarget()->Hurt(Damage, this);
            }
            break;
    }
}

void Mutant::OnAttacked(AlifeAgent* Attacker)
{
    if(!GetTarget())
    {
        SetTarget(Attacker);
        currentIntent = COMBAT;

        if(!GetAgentTeam())
            return;
        for(AlifeAgent* ag : GetAgentTeam()->GetAllAgents())
        {
            if(!ag || ag == this || !ag->IsAlive()) continue;
            ag->OnAllyAttacked(Attacker);
        }
    }
}

void Mutant::OnAllyAttacked(AlifeAgent* Attacker)
{
    if(!GetTarget())
    {
        bool ChanceToTarget = zone->GenerateInRange(0, 1) <= GetAwareness();
        if(ChanceToTarget) {
            SetTarget(Attacker);
            currentIntent = COMBAT;
        }
    }
}