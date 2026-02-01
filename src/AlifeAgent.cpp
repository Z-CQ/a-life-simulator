#include "headers/AlifeAgent.h"

unsigned int AlifeAgent::nextID = 0;

AlifeAgent::AlifeAgent()
{
    agentID = nextID;
    nextID++;
}