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

    std::string ownFac = Factions::ResolveFactionName(GetAgentFaction());
    zone->AddEntry(LogEntry(ownFac + " used a bandage.", Factions::ResolveFactionColor(GetAgentFaction()))); 

    return true;
}

void Stalker::Update()
{
    AlifeAgent::Update();

    if(!IsAlive())
        return;

    double HungerRate = 0.05, ThirstRate = 0.1;

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

    // 40% chance to run away if in combat & morale/health is low
    if(currentIntent == COMBAT && GetMorale() <= 0.1 && GetHealth() < std::round((double)maxHP * 0.8) && zone->GenerateInRange(0.0, 1.0) <= 0.4)
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
            
            if(zone->GenerateInRange(0.0, 1.0) > GetMoveSpeed() * 0.5) // multiply move speed by 0.3 to artificially slow down patrol
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

            // Higher hunger/thirst since in combat
            HungerRate += 0.1;
            ThirstRate += 0.1;

            // If this agent is tiles away from the target, it should use Strength rather than Skill
            int DistanceToTarget = Vector2::DistanceSquared(GetLocation(), GetTarget()->GetLocation());
            bool IsClose = DistanceToTarget <= 9;

            if(!IsClose && GetInventory().Ammo <= 0)
                break;

            double RandomChance = GetTarget()->GetCurrentIntent() == RUNAWAY ? zone->GenerateInRange(0.0, 4.0) : zone->GenerateInRange(0.0, 1.0);
            bool CanStrike = (IsClose ? RandomChance <= GetStrength() : RandomChance <= GetSkill()) &&
                HasLineOfSight(GetTarget());

            if(CanStrike)
            {
                int Damage = IsClose ? std::round(zone->GenerateInRange(12.0, 32.0) * GetStrength()) : std::round(zone->GenerateInRange(21.0, 39.0) * GetSkill());
                GetTarget()->Hurt(Damage, this);
                if(!IsClose)
                    GetInventory().Ammo--;
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
                zone->AddEntry(LogEntry(ownFac + " looted " + lootedFac + ".", Factions::ResolveFactionColor(GetAgentFaction()))); 
                
                Inventory tInv = lastTarget->GetInventory();
                GetInventory().Ammo += tInv.Ammo;
                GetInventory().Food += tInv.Food;
                GetInventory().Water += tInv.Water;
                GetInventory().Bandages += tInv.Bandages;
                tInv.Zero();
                
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

    if((GetBleeding() > 0 || GetHealth() < std::round(maxHP / 5)) && zone->GenerateInRange(0.0, 1.0) <= 0.6)
        UseBandage();

    if(zone->GenerateInRange(0.0, 1.0) <= HungerRate && zone->GenerateInRange(0.0, 1.0) <= 0.1)
        AdjustHunger(zone->GenerateInRange(0.05, 0.2));

    if(zone->GenerateInRange(0.0, 1.0) <= ThirstRate && zone->GenerateInRange(0.0, 1.0) <= 0.2)
        AdjustThirst(zone->GenerateInRange(0.1, 0.25));

    if(GetHunger() > 0.8 && GetInventory().Food > 0 && zone->GenerateInRange(0.0, 1.0) <= 0.2)
        Eat();

    if(GetThirst() > 0.7 && GetInventory().Water > 0 && zone->GenerateInRange(0.0, 1.0) <= 0.3)
        Drink();
}

void Stalker::OnAttacked(AlifeAgent* Attacker)
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

void Stalker::OnAllyAttacked(AlifeAgent* Attacker)
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