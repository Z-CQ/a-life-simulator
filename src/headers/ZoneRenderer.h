#pragma once

#include <vector>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

class ZoneRenderer {

private:
    std::vector<std::string> log;
    void ClearDisplay();

public:
    void SetActivityLog(std::vector<std::string> l) { log = l; }

    void Display();

};