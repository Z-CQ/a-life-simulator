#pragma once

#include <vector>
#include <string>

class AlifeAgent;

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

    // @return All living agents registered in the Zone.
    virtual std::vector<AlifeAgent*> GetAllAgents() const = 0;

    /**
     * Add an entry to the activity log and increase numLogs.
     * 
     * @param log The log itself.
     */
    virtual void AddEntry(std::string log) = 0;


};