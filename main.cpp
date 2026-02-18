#include "src/headers/Zone.h"

#include <stdio.h>
#include <iostream>

int main()
{
    Zone zone{};

    zone.Populate(11, 5, 3);

    zone.AddEntry("Hello world!");
    zone.AddEntry("Ready?");
    zone.Update();
}