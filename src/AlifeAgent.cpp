#include "headers/AlifeAgent.h"

unsigned int AlifeAgent::nextID = 0;

AlifeAgent::AlifeAgent(IZone* owningZone, Stats initialStats) : zone(owningZone), stats(initialStats)
{
    agentID = nextID;
    nextID++;
    maxHP = stats.Health;
}

void AlifeAgent::Update()
{
    Heal(GetRegeneration());
}

void AlifeAgent::Heal(int Amount)
{
    SetHealth(std::clamp(GetHealth() + Amount, 0, maxHP));
}

void AlifeAgent::Hurt(int Damage, AlifeAgent* Attacker)
{
    SetHealth(GetHealth() - Damage);
    
    if(GetHealth() <= 0)
        Kill();
    else {
        double moraleShift = zone->GenerateInRange(-0.3, -0.1);
        AdjustMorale(moraleShift);

        // Can only bleed if the damage did more than 15% of the agent's max health
        bool shouldBleed = (Damage > (maxHP / 15)) && (zone->GenerateInRange(0, 1) >= 0.35);
        if(shouldBleed)
        {
            int addedBleeding = zone->GenerateInRange(1, 4);
            SetBleeding(GetBleeding() + addedBleeding);
        }

        OnAttacked(Attacker);
    }
}

void AlifeAgent::Kill()
{
    if(!IsKillable() || !IsAlive())
        return;

    SetAlive(false);
    SetMoveable(false);
}

bool AlifeAgent::DetachFromTeam()
{
    if(!GetAgentTeam())
        return false;
    
    GetAgentTeam()->RemoveAgent(this);

    if(GetAgentTeam()->IsEmpty())
        delete GetAgentTeam();

    SetAgentTeam(nullptr);

    return true;
}

void AlifeAgent::PromoteToLeader()
{
    GetAgentTeam()->SetTeamLeader(this);
}