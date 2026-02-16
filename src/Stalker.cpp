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