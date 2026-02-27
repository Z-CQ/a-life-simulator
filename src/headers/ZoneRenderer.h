#pragma once

#include "AlifeAgent.h"
#include "EnvironmentEntity.h"
#include "../util/Structures.h"

#include <vector>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

class ZoneRenderer {

private:
    std::vector<LogEntry> log;
    std::vector<AlifeAgent*> agents;
    std::vector<EnvironmentEntity*> env;

    /**
     * Create a canvas that draws the zone: agents, walls, campfires, etc.
     * 
     * @param w The width of the zone
     * @param h The height of the zone
     * 
     * @return The canvas with the drawn characters
     */
    Element BuildWorldPane(int w, int h);

    /**
     * Create a vertical box that contains the most recent activity log.
     * 
     * @param maxLines The most amount of lines to show
     * 
     * @return The vbox containing the log
     */
    Element BuildLogPane(int maxLines);

public:
    void ClearDisplay();

    void SetActivityLog(std::vector<LogEntry> &l) { log = l; }
    void SetAgents(std::vector<AlifeAgent*> &ag) { agents = ag; }
    void SetEnvironmentEntities(std::vector<EnvironmentEntity*> &ev) { env = ev; }

    void Display();

};