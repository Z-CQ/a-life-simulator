#include "headers/ZoneRenderer.h"

#include <iostream>

void ZoneRenderer::ClearDisplay()
{
    std::cout << "\033[2J\033[1;1H";
}

Element ZoneRenderer::BuildWorldPane(int w, int h)
{
    Canvas c(w * 2, h * 4);

    for(EnvironmentEntity* e : env)
    {
        for(int w = 0; w < e->GetWidth(); w++)
            for(int h = 0; h < e->GetHeight(); h++)
                c.DrawText((e->GetPosition().x  + w )* 2, (e->GetPosition().y + h) * 4, std::string(1, e->GetResembledBy()), e->GetColor());
    }

    for(AlifeAgent* ag : agents)
    {
        Color col = Factions::ResolveFactionColor(ag->GetAgentFaction());
        if(ag->IsAlive())
        {
            c.DrawText(ag->GetLocation().x * 2, ag->GetLocation().y * 4, "O", col);
        }
        else {
            c.DrawText(ag->GetLocation().x * 2, ag->GetLocation().y * 4, "X", col); // Corpse
        }
    }


    return canvas(std::move(c));
}

Element ZoneRenderer::BuildLogPane(int maxLines)
{
    int start = std::max(0, (int)log.size() - maxLines);

    Elements lines;
    lines.reserve(std::min((int)log.size(), maxLines));

    for (int i = start; i < (int)log.size(); ++i) {
        lines.push_back(text(log[i]));
    }

    return vbox({
            filler(),
            vbox(std::move(lines)),
            }) | flex;
}

void ZoneRenderer::Display()
{
    ClearDisplay();

    auto screen = Screen::Create(Dimension::Full());

    int numLines = std::max(1, screen.dimy() - 2);

    auto world = window(text("Zone"), BuildWorldPane(screen.dimx() - 74, screen.dimy() - 2)) | flex;
    auto log = window(text("Activity Log"), BuildLogPane(numLines)) | size(WIDTH, EQUAL, 72);

    auto root = hbox({
          world,
          log,
        });

    Render(screen, root);
    screen.Print();
}