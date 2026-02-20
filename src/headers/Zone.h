#pragma once

#include "../interfaces/IZone.h"
#include "AlifeAgent.h"
#include "Stalker.h"
#include "Mutant.h"
#include "EnvironmentEntity.h"
#include "ZoneRenderer.h"

#include <memory>
#include <random>

class Zone : IZone {

protected:
    std::mt19937 eng;
    ZoneRenderer* renderer;

    std::vector<AlifeAgent*> AllAgents;
    std::vector<Team*> AllTeams;

    std::vector<EnvironmentEntity*> AllEnvironmentEntites;

    std::vector<std::string> ActivityLog;

    int simWidth;
    int simHeight;

    std::vector<int> Map;

    inline int GetMapTile(int x, int y) const { return y * GetSimWidth() + x; }
    inline int GetMapTile(Vector2& pos) const { return pos.y * GetSimWidth() + pos.x; }

public:
    // Seed the engine with a random device and create a renderer on instantiation.
    Zone();
    
    double GenerateInRange(double min, double max) override;
    int GenerateInRange(int min, int max) override;
    std::vector<AlifeAgent*> GetAllAgents() const override { return AllAgents; }

    AlifeAgent* GetAgentByID(int AgentID) const { return AllAgents[AgentID - 2]; /* Agent IDs start at 2. */ }

    void AddEntry(std::string log) override;

    int GetSimWidth() const override { return simWidth; }
    int GetSimHeight() const override { return simHeight; }

    std::vector<int> GetMap() const override { return Map; }

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


    /**
     * Checks if the requested position is occupied or not.
     * 
     * @return Something is in the way.
     */
    virtual bool IsPositionOccupied(Vector2 pos) const override { return Map[GetMapTile(pos.x, pos.y)] != -1; }
    virtual bool IsPositionOccupied(int x, int y) const override { return Map[GetMapTile(x, y)] != -1; }


    /**
     * Set the value at the given position on the map
     * 
     * @param pos The position to change
     * @param value The new value
     */
    void SetMapValue(Vector2 pos, int value) { Map[GetMapTile(pos)] = value; }
    void SetMapValue(int x, int y, int value) { Map[GetMapTile(x, y)] = value; }


    /**
     * Find a spot near pos following maxRadius, overwriting pos 
     * 
     * @param pos The position to find a spot near
     * @param maxRadius the farthest to check
     * 
     * @return `true` if a position was found and pos was updated, `false` if not
     */
    virtual bool FindFreeSpace(Vector2& pos, int maxRadius) override {
        if (!IsPositionOccupied(pos)) return true;

        for (int r = 1; r <= maxRadius; r++) {
            for (int dy = -r; dy <= r; dy++) {
                for (int dx = -r; dx <= r; dx++) {
                    if (std::abs(dx) != r && std::abs(dy) != r) continue; // border only
                    int nx = pos.x + dx, ny = pos.y + dy;
                    if (nx < 0 || ny < 0 || nx >= GetSimWidth() || ny >= GetSimHeight()) continue;
                    if (!IsPositionOccupied(nx, ny)) { pos.x = nx; pos.y = ny; return true; }
                }
            }
        }
        return false;
    }

    ~Zone() {
        // Destroy all agents
        for(AlifeAgent* ag : AllAgents)
            delete ag;

        // Destroy all teams
        for(Team* t : AllTeams)
            delete t;

        // Destroy all environment entities (campfires, walls, stuff like that)
        for(EnvironmentEntity* e : AllEnvironmentEntites)
            delete e;

        delete renderer;
    }
};