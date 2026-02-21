#pragma once

#include <cmath>
#include <stdexcept>

struct Vector2 {
    int x = 0;
    int y = 0;

    void Zero()
    {
        x = 0;
        y = 0;
    }
};

struct AgentDirection {
private:
    double x = 0;
    double y = 0;

public:
    AgentDirection() = default;

    AgentDirection(double xVal, double yVal)
    {
        Set(xVal, yVal);
    }

    void Set(double xVal, double yVal)
    {
        double magSq = xVal * xVal + yVal * yVal;

        if (magSq < 1e-12)
        {
            x = 0;
            y = 0;
            return;
        }

        double invMag = 1.0 / std::sqrt(magSq);

        x = xVal * invMag;
        y = yVal * invMag;
    }

    double X() const { return x; }
    double Y() const { return y; }
};