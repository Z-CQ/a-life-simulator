#include "src/headers/Zone.h"

#include <stdio.h>
#include <iostream>

#define TICK_SPEED_MS 100

int main()
{
    Zone zone{};

    // Stalker, Mutant, Campfire, Building, Deviation
    zone.Populate(48, 12, 12, 80, 8);

    // Simualation loop
    while (true) {
        zone.Update();

        std::this_thread::sleep_for(std::chrono::milliseconds(TICK_SPEED_MS));
    }
}