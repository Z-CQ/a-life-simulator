#pragma once

#include "../interfaces/IZone.h"
#include "AlifeAgent.h"
#include "ZoneRenderer.h"

#include <memory>
#include <random>

class Zone : IZone {

protected:
    std::mt19937 eng;
    ZoneRenderer* renderer;

    std::vector<AlifeAgent*> AllAgents;

    std::vector<std::string> ActivityLog;

public:
    // Seed the engine with a random device and create a renderer on instantiation.
    Zone();
    
    double GenerateInRange(double min, double max) override;
    std::vector<AlifeAgent*> GetAllAgents() const override { return AllAgents; }

    void AddEntry(std::string log) override;

    /**
     * Creates a random amount of Stalkers and Mutants, using deviation [-Deviation, Deviation) to add or subtract to both agent counts.
     * 
     * @param Stalkers The amount of stalkers to generate.
     * @param Mutants The amount of mutants to generate.
     * @param Deviation The range to subtract/add to both counts.
     */
    void Populate(int Stalkers, int Mutants, int Deviation);

    /**
     * Called every tick of the game. Updates each agent and display. Naturally, the call frequency determines the speed of the simulation.
     */
    void Update();

    ~Zone() {
        // Destroy all agents
        for(AlifeAgent* ag : AllAgents)
        {
            if(ag && ag->GetAgentTeam())
                delete ag->GetAgentTeam();
                
            delete ag;
        }

        delete renderer;
    }
};