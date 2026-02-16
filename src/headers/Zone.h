#pragma once

#include "../interfaces/IZone.h"
#include "AlifeAgent.h"

#include <memory>
#include <random>

class Zone : IZone {

protected:
    std::mt19937 eng;

    // All agents in the Zone.
    std::vector<AlifeAgent*> AllAgents;

public:
    // Seed the engine with a random device on instantiation.
    Zone() : eng(std::random_device{}()) {}

    /**
     * Generates a number between [min, max).
     * 
     * @param min The lowest number to generate, inclusive.
     * @param max The largest number to generate, exclusive.
     * @return The number generated [min, max).
     */
    double GenerateInRange(double min, double max) override;

    // @return All living agents registered in the Zone.
    std::vector<AlifeAgent*> GetAllAgents() const override { return AllAgents; }



    /**
     * Creates a random amount of Stalkers and Mutants, using deviation [-Deviation, Deviation) to add or subtract to both agent counts.
     * 
     * @param Stalkers The amount of stalkers to generate.
     * @param Mutants The amount of mutants to generate.
     * @param Deviation The range to subtract/add to both counts.
     */
    void Populate(int Stalkers, int Mutants, int Deviation);

    /**
     * Called every tick of the game. Updates each agent. Naturally, the call frequency determines the speed of the simulation.
     */
    void Update();

    ~Zone() {
        // Destroy all agents
        for(AlifeAgent* ag : AllAgents)
        {
            if(ag->GetAgentTeam())
                delete ag->GetAgentTeam();
                
            delete ag;
        }
    }
};