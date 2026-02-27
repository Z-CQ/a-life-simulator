#include "src/headers/Zone.h"

#include <stdio.h>
#include <iostream>

#define TICK_SPEED_MS 200

int main()
{
    Zone zone{};

    zone.Populate(35, 9, 12, 80, 4);

    // Simualation loop
    while (true) {
        zone.Update();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}