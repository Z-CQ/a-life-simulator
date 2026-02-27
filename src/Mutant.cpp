#include "headers/Mutant.h"

#include <iostream>

void Mutant::Update()
{
    AlifeAgent::Update();

    if(!IsAlive())
        return;

    double HungerRate = 0.1;

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

    // 60% chance to run away if in combat & morale/health is low
    if(currentIntent == COMBAT && GetMorale() <= 0.1 && GetHealth() < std::round((double)maxHP * 0.8) && zone->GenerateInRange(0.0, 1.0) <= 0.6)
    {
        currentIntent = RUNAWAY;
        SetTarget(nullptr);
        lastTarget = nullptr;
        SetAgentTeam(nullptr);
        
        // Set a random direction and run.
        // I would make it so they run the opposite direction consistently but i think it'd be funny if he charged at his attacker lol
        SetDirection(AgentDirection{
            zone->GenerateInRange(-1.0, 1.0),
            zone->GenerateInRange(-1.0, 1.0)
        });

        std::string ownFac = Factions::ResolveFactionName(GetAgentFaction());
        zone->AddEntry(LogEntry(ownFac + " abandoned their team and is running away.", Factions::ResolveFactionColor(GetAgentFaction()))); 
    } else if(currentIntent == RUNAWAY && (
        (GetHealth() >= (maxHP / 4) && zone->GenerateInRange(0.0, 1.0) <= 0.4) || // If this agent is running and one of two are true - enough health and 40% chance OR 7.5% chance - return to patrol
        (zone->GenerateInRange(0.0, 1.0) <= 0.075)
    ))
    {
        currentIntent = PATROL;
        
        std::string ownFac = Factions::ResolveFactionName(GetAgentFaction());
        zone->AddEntry(LogEntry(ownFac + " is returning to patrol alone.", Factions::ResolveFactionColor(GetAgentFaction()))); 
    }

    switch(currentIntent)
    {
        case PATROL:
        {
            SearchForNearbyEnemy();
            
            if(zone->GenerateInRange(0.0, 1.0) > GetMoveSpeed() * 0.7) // multiply move speed by 0.4 to artificially slow down patrol
                break;
                
            if(team)
                SetDirection(team->GetWalkDirection());

            Move();
            
            // If this agent locked onto something, switch to COMBAT.
            if(GetTarget()) {
                currentIntent = COMBAT;
            }
            
            break;
        }

        case COMBAT:
        {
            // If the target is dead, we can switch to looting; patrol if nullptr
            if(GetTarget())
            {
                if(!GetTarget()->IsAlive())
                {
                    currentIntent = LOOTING;
                    targetPosition = GetTarget()->GetLocation();
                    lastTarget = GetTarget();
                    SetTarget(nullptr);
                    break;
                }
            } else {
                currentIntent = PATROL;
                break;
            }

            HungerRate += 0.1;

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

        case LOOTING:
        {
            if((targetPosition.x < 0 && targetPosition.y < 0) || !lastTarget)
            {
                currentIntent = PATROL;
                break;
            }
            SearchForNearbyEnemy();

            int DistanceSquared = Vector2::DistanceSquared(GetLocation(), lastTarget->GetLocation());
            if(DistanceSquared > 2)
            {
                Move();
            } else {
                std::string ownFac = Factions::ResolveFactionName(GetAgentFaction());
                std::string lootedFac = Factions::ResolveFactionName(lastTarget->GetAgentFaction());
                zone->AddEntry(LogEntry(ownFac + " ate " + lootedFac + ".", Factions::ResolveFactionColor(GetAgentFaction()))); 
                
                AdjustHunger(-zone->GenerateInRange(0.1, 0.3));
                
                lastTarget = nullptr;
                currentIntent = PATROL;
                targetPosition = Vector2{-1, -1};
            }

            // If this agent locked onto something, switch to COMBAT.
            if(GetTarget()) {
                currentIntent = COMBAT;
            }

            break;
        }

        case RUNAWAY:
        {
            Move();
            break;
        }
    }

    if(zone->GenerateInRange(0.0, 1.0) <= HungerRate && zone->GenerateInRange(0.0, 1.0) <= 0.1)
        AdjustHunger(zone->GenerateInRange(0.05, 0.2));
}

void Mutant::OnAttacked(AlifeAgent* Attacker)
{
    if(!Attacker)
        return;

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
    if(!Attacker)
        return;

    if(!GetTarget())
    {
        bool ChanceToTarget = zone->GenerateInRange(0, 1) <= GetAwareness();
        if(ChanceToTarget) {
            SetTarget(Attacker);
            currentIntent = COMBAT;
        }
    }
}