#pragma once

#include "../util/Structures.h"

#include <ftxui/screen/color.hpp>


class EnvironmentEntity
{

private:
    Vector2 pos;
    ftxui::Color screenColor;
    char ch;
    int id;
    int width;
    int height;

public:
    EnvironmentEntity(Vector2 spawnPos, ftxui::Color col, char resembledBy, int eId, int w = 1, int h = 1)
        : pos(spawnPos), screenColor(col), ch(resembledBy), id(eId), width(w), height(h) {}

    Vector2 GetPosition() const { return pos; }
    ftxui::Color GetColor() const { return screenColor; }
    char GetResembledBy() const { return ch; }
    int GetID() const { return id; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    
};