#include "headers/Mutant.h"

#include <iostream>

void Mutant::Update()
{
    AlifeAgent::Update();

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
            if(zone->GenerateInRange(0.0, 1.0) > GetMoveSpeed())
                return;
                
            if(team)
                SetDirection(team->GetWalkDirection());

            Move();
            
            break;
    }
}

void Mutant::OnAttacked(AlifeAgent* Attacker)
{
    
}

void Mutant::OnAllyAttacked(AlifeAgent* Attacker)
{

}