#pragma once

#include "../util/Structures.h"

#include <vector>
#include <string>

class AlifeAgent;

constexpr double TwoPi = 6.2831853071795864769;

class IZone {

public:
    virtual ~IZone() = default;

    /**
     * Generates a number between [min, max).
     * 
     * @param min The lowest number to generate, inclusive.
     * @param max The largest number to generate, exclusive.
     * @return The number generated [min, max).
     */
    virtual double GenerateInRange(double min, double max) = 0;
    virtual int GenerateInRange(int min, int max) = 0;

    // @return All living agents registered in the Zone.
    virtual std::vector<AlifeAgent*> GetAllAgents() const = 0;

    virtual AlifeAgent* GetAgentByID(int AgentID) const = 0;

    /**
     * Add an entry to the activity log and increase numLogs.
     * 
     * @param log The log itself.
     */
    virtual void AddEntry(LogEntry log) = 0;

    /**
     * @return The width of the simulation
     */
    virtual int GetSimWidth() const = 0;

    /**
     * @return The height of the simulation
     */
    virtual int GetSimHeight() const = 0;

    /**
     * @return The world grid
     */
    virtual std::vector<int> GetMap() const = 0;
    virtual inline int GetMapTile(int x, int y) const = 0;
    virtual inline int GetMapTile(Vector2& pos) const = 0;

    virtual void SetMapValue(Vector2 pos, int value) = 0;
    virtual void SetMapValue(int x, int y, int value) = 0;

    virtual bool IsPositionOccupied(Vector2 pos) const = 0;
    virtual bool IsPositionOccupied(int x, int y) const = 0;

    virtual bool FindFreeSpace(Vector2& pos, int maxRadius) = 0;

};