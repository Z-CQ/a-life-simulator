#pragma once

#include <cmath>
#include <stdexcept>
#include <string>
#include <ftxui/screen/color.hpp>

struct Vector2 {
    int x = 0;
    int y = 0;

    void Zero()
    {
        x = 0;
        y = 0;
    }

    static int DistanceSquared(const Vector2& from, const Vector2& to)
    {
        int dx = to.x - from.x;
        int dy = to.y - from.y;
        return dx * dx + dy * dy;
    }

    static std::pair<double, double> Direction(const Vector2& from, const Vector2& to)
    {
        double dx = static_cast<double>(to.x - from.x);
        double dy = static_cast<double>(to.y - from.y);

        double length = std::sqrt(dx * dx + dy * dy);

        if (length == 0.0)
            return {0.0, 0.0};

        return { dx / length, dy / length };
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

struct LogEntry {
    std::string log;
    ftxui::Color color;

    LogEntry(std::string l, ftxui::Color col = ftxui::Color::White) : log(l), color(col) {}
};