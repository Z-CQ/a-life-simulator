#include "headers/Stalker.h"

#include <iostream>

bool Stalker::UseBandage()
{
    if(GetInventory().Bandages <= 0 || GetHealth() >= maxHP)
        return false;

    GetInventory().Bandages--;
    StopBleeding();
    
    int RandomHeal = zone->GenerateInRange(5, 15);
    Heal(RandomHeal);

    return true;
}

void Stalker::Update()
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
            
            if(zone->GenerateInRange(0.0, 1.0) > GetMoveSpeed() * 0.2) // multiply move speed by 0.2 to artificially slow down patrol
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
                break;
            }

            // If this agent is tiles away from the target, it should use Strength rather than Skill
            int DistanceToTarget = Vector2::DistanceSquared(GetLocation(), GetTarget()->GetLocation());
            bool IsClose = DistanceToTarget <= 9;

            double RandomChance = zone->GenerateInRange(0.0, 1.0);
            bool CanStrike = (IsClose ? RandomChance <= GetStrength() : RandomChance <= GetSkill()) &&
                HasLineOfSight(GetTarget());

            if(CanStrike)
            {
                int Damage = IsClose ? std::round(zone->GenerateInRange(12.0, 32.0) * GetStrength()) : std::round(zone->GenerateInRange(21.0, 39.0) * GetSkill());
                GetTarget()->Hurt(Damage, this);
            }
            break;
    }
}

void Stalker::OnAttacked(AlifeAgent* Attacker)
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

void Stalker::OnAllyAttacked(AlifeAgent* Attacker)
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