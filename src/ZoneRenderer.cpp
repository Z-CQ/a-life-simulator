#include "headers/ZoneRenderer.h"

#include <iostream>

#define SIM_WIDTH 128
#define LOG_WIDTH 96

void ZoneRenderer::ClearDisplay()
{
    std::cout << "\033[2J\033[1;1H";
}

void ZoneRenderer::Display()
{
    ClearDisplay();

    
}