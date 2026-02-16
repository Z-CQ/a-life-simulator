#pragma once

#include <vector>

class AlifeAgent;

class IZone {

public:
    virtual ~IZone() = default;

    virtual double GenerateInRange(double min, double max) = 0;
    virtual std::vector<AlifeAgent*> GetAllAgents() const = 0;


};