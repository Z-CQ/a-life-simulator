#include "headers/Stalker.h"

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
}

void Stalker::OnAttacked(AlifeAgent* Attacker)
{
    if(!GetTarget())
    {
        SetTarget(Attacker);
        OnAllyAttacked(Attacker);
    }
}

void Stalker::OnAllyAttacked(AlifeAgent* Attacker)
{
    if(!GetTarget())
    {
        bool ChanceToTarget = zone->GenerateInRange(0, 1) <= GetAwareness();
        if(ChanceToTarget)
            SetTarget(Attacker);
    }
}