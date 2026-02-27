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

AlifeAgent* AlifeAgent::SearchForNearbyEnemy()
{
    if(GetTarget() && GetTarget()->IsAlive())
        return nullptr;
    
    int MaxRadius = 22;
    std::vector<AlifeAgent*> NearbyAgents = GetNearbyAgents(std::floor(MaxRadius * GetSight()));

    // Track all candidates to give each an equal chance.
    std::vector<AlifeAgent*> Candidates;

    for(AlifeAgent* ag : NearbyAgents)
    {
        if(!ag->IsAlive())
            continue;
        Factions::Faction targetFaction = ag->GetAgentFaction();
        Factions::Attitude targetAttitude = Factions::GetRelation(GetAgentFaction(), targetFaction);

        // Skip if not hostile
        if(targetAttitude != Factions::Attitude::Hostile)
            continue;

        bool ShouldDetect = zone->GenerateInRange(0.0, 1.0) <= GetSight() && HasLineOfSight(ag);
        if(ShouldDetect)
            Candidates.push_back(ag);
    }

    if(Candidates.empty())
        return nullptr;

    int MaxIndex = Candidates.size() - 1;
    int TargetIndex = zone->GenerateInRange(0, MaxIndex);
    AlifeAgent* NewTarget = Candidates[TargetIndex];
    
    SetTarget(NewTarget);
    return NewTarget;
}

void AlifeAgent::Update()
{
    Heal(GetRegeneration());
}

std::vector<AlifeAgent*> AlifeAgent::GetNearbyAgents(int radius)
{
    std::vector<AlifeAgent*> NearbyAgents;

    for (int r = 1; r <= radius; r++) {
        for (int dy = -r; dy <= r; dy++) {
            for (int dx = -r; dx <= r; dx++) {
                if (std::abs(dx) != r && std::abs(dy) != r) continue; // border only

                int nx = GetLocation().x + dx, ny = GetLocation().y + dy;
                if (nx < 0 || ny < 0 || nx >= zone->GetSimWidth() || ny >= zone->GetSimHeight()) continue;

                int TileID = zone->GetMap()[zone->GetMapTile(nx, ny)];
                if(TileID < 2) continue; // Not an agent

                NearbyAgents.push_back(zone->GetAgentByID(TileID));
            }
        }
    }

    return NearbyAgents;
}

bool AlifeAgent::HasLineOfSight(AlifeAgent* target)
{
    if (!target) return false;

    int x0 = GetLocation().x;
    int y0 = GetLocation().y;
    int x1 = target->GetLocation().x;
    int y1 = target->GetLocation().y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0;
    int y = y0;

    while (!(x == x1 && y == y1)) {
        int e2 = err * 2;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx)  { err += dx; y += sy; }

        if (x < 0 || y < 0 || x >= zone->GetSimWidth() || y >= zone->GetSimHeight())
            return false;

        int TileID = zone->GetMap()[zone->GetMapTile(x, y)];
        if (TileID == 0)
            return false;
    }

    return true;
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
    double dx = 0;
    double dy = 0;
    
    if(targetPosition.x >= 0 && targetPosition.y >= 0)
    {
        std::pair<double, double> dir = Vector2::Direction(GetLocation(), targetPosition);
        dx = dir.first;
        dy = dir.second;
    } else {
        dx = GetDirection().X();
        dy = GetDirection().Y();
    }


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