#include "headers/AlifeAgent.h"

unsigned int AlifeAgent::nextID = 2; // Starting at two so that 1 can be reserved for campfires, and 0 reserved for walls.

AlifeAgent::AlifeAgent(IZone* owningZone, Stats initialStats) : zone(owningZone), stats(initialStats)
{
    agentID = nextID;
    nextID++;
    maxHP = stats.Health;

    SetAlive(true);
    SetKillable(true);
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
    zone->SetMapValue(GetLocation(), -1);
}

void AlifeAgent::Move()
{
    double dx = GetDirection().X();
    double dy = GetDirection().Y();

    overflowX += dx;
    overflowY += dy;

    int stepX = (int)std::trunc(overflowX);
    int stepY = (int)std::trunc(overflowY);

    if (stepX != 0 || stepY != 0)
    {
        MoveBy(stepX, stepY);
        overflowX -= stepX;
        overflowY -= stepY;
    }
}

void AlifeAgent::MoveBy(int x, int y)
{
    if(!IsAlive()) // if the agent is dead, it should not be able to move
        return;

    // Free up this agent's spot
    zone->SetMapValue(GetLocation(), -1);

    int desiredX = std::clamp(GetLocation().x + x, 0, zone->GetSimWidth() - 1);
    int desiredY = std::clamp(GetLocation().y + y, 0, zone->GetSimHeight() - 1);

    // If the space is not occupied, the agent can move to it
    if(!zone->IsPositionOccupied(desiredX, GetLocation().y))
        GetLocation().x = desiredX;

    // Same for the y coordinate
    if(!zone->IsPositionOccupied(GetLocation().x, desiredY))
        GetLocation().y = desiredY;
        
    // Set this agent's spot on the map
    zone->SetMapValue(GetLocation(), GetAgentID());
}

bool AlifeAgent::DetachFromTeam()
{
    if(!GetAgentTeam())
        return false;
    
    GetAgentTeam()->RemoveAgent(this);
    SetAgentTeam(nullptr);

    return true;
}

void AlifeAgent::PromoteToLeader()
{
    GetAgentTeam()->SetTeamLeader(this);
}