#include "headers/Zone.h"

#include <iostream>

Zone::Zone() : eng(std::random_device{}())
{
    renderer = new ZoneRenderer();
    simWidth = Dimension::Full().dimx - 74;
    simHeight = Dimension::Full().dimy - 2;

    Map.resize(simWidth * simHeight, -1);
}

double Zone::GenerateInRange(double min, double max)
{
    if (min > max)
        std::swap(min, max);

    std::uniform_real_distribution<double> dist(min, max);
    return dist(eng);
}

int Zone::GenerateInRange(int min, int max)
{
    if (min > max)
        std::swap(min, max);

    std::uniform_int_distribution<int> dist(min, max);
    return dist(eng);
}

void Zone::Populate(int Stalkers, int Mutants, int Campfires, int Buildings, int Deviation)
{
    Factions::InitFactions();

    // Determine how many stalkers & mutants we want.
    int finalStalkerCount = Stalkers + GenerateInRange(-Deviation, Deviation);
    int stalkersToMake = finalStalkerCount;

    int finalMutantCount = Mutants + GenerateInRange(-Deviation, Deviation);
    int mutantsToMake = finalMutantCount;

    // How many stalkers at most do we want per team?
    int stalkersPerTeam = 4;

    // How many mutants at most do we want per team?
    int mutantsPerTeam = 3;

    // Calculate how many STALKER teams we want.
    int numStalkerTeams = (finalStalkerCount + stalkersPerTeam - 1) / stalkersPerTeam;

    // Calculate how mnay MUTANT teams we want.
    int numMutantTeams = (finalMutantCount + mutantsPerTeam - 1) / mutantsPerTeam;

    // How many campfires should be on the map?
    int numCampfires = Campfires + GenerateInRange(-Deviation, Deviation);

    // How many "buildings" should be on the map?
    int numBuildings = Buildings + GenerateInRange(-Deviation, Deviation);

    for(int i = 0; i < numBuildings; i++)
    {
        std::cout << "Generating a new building." << std::endl;

        int width = GenerateInRange(5, 8);
        int height = GenerateInRange(2, 6);

        Vector2 spawnPos{
            GenerateInRange(0, GetSimWidth() - width),
            GenerateInRange(0, GetSimHeight() - height)
        };

        if(IsPositionOccupied(spawnPos)) {
            if(!FindFreeSpace(spawnPos, 5))
                continue;
        }

        EnvironmentEntity* building = new EnvironmentEntity(spawnPos, ftxui::Color::GrayDark, '#', 0, width, height);
        for(int w = 0; w < width; w++)
            for(int h = 0; h < height; h++)
                SetMapValue(spawnPos.x + w, spawnPos.y + h, 0);

        AllEnvironmentEntites.push_back(building);
    }

    // Generate the amount of stalker teams needed
    for(int i = 0; i < numStalkerTeams; i++)
    {
        std::cout << "Generating a new STALKER team." << std::endl;

        // Pick a random faction
        int facIndex = GenerateInRange(1, Factions::FactionCount - 1);
        Factions::Faction fac = static_cast<Factions::Faction>(facIndex);

        // Instantiate the team
        Team* team = new Team();

        // Keep some margin from the edges
        int margin = 4;
        Vector2 teamOrigin = Vector2{GenerateInRange(margin, GetSimWidth() - margin),
            GenerateInRange(margin, GetSimHeight() - margin)};

        if(IsPositionOccupied(teamOrigin))
            FindFreeSpace(teamOrigin, 8);

        team->SetOrigin(teamOrigin);

        // Spread to control how far apart they are; smaller is tighter
        team->SetSpread(std::max(1.0, std::min(60.0, (double)stalkersPerTeam * 2.0)));

        // Pick a random base angle so teams don't all have same orientation
        team->SetAngle(GenerateInRange(0.0, TwoPi));

        for(int j = 0; j < stalkersPerTeam && stalkersToMake > 0; j++)
        {
            std::cout << "Creating a new STALKER." << std::endl;

            // Create initial stats for the stalker
            Stats stats;
            stats.MovementSpeed += GenerateInRange(0.3, 0.5);
            stats.Strength += GenerateInRange(-0.25, 0.25);
            stats.Skill += GenerateInRange(-0.25, 0.25);
            stats.Awareness += GenerateInRange(-0.1, 0.1);
            stats.Sight += GenerateInRange(-0.15, 0.15);
            stats.Morale += GenerateInRange(-0.15, 0.15);
            stats.Hunger += GenerateInRange(0.0, 0.1);
            stats.Thirst += GenerateInRange(0.0, 0.15);

            // Create the stalker and assign as leader if there's none assigned
            Stalker* stalker = new Stalker(this, stats);

            team->AddAgent(stalker);
            stalker->SetAgentTeam(team);
            if(!team->GetTeamLeader())
            {
                team->SetTeamLeader(stalker);
                stalker->AdjustMorale(GenerateInRange(0.1, 0.2)); // Leader should be a little happier
            }

            // Create an inventory for the stalker
            Inventory inv;
            inv.Ammo = GenerateInRange(80, 140);
            inv.Bandages = GenerateInRange(0, 2);
            inv.Water = GenerateInRange(0, 2);
            inv.Food = GenerateInRange(0, 3);

            stalker->GetInventory() = inv;

            // Assign the stalker to its faction
            stalker->SetAgentFaction(fac);

            // Place agent around the center with small random jitter so they stay close
            double angle = team->GetAngle() + TwoPi * (double)j / (double)stalkersPerTeam + GenerateInRange(-0.15, 0.15);
            double radius = GenerateInRange(team->GetSpread() * 0.05, team->GetSpread() * 0.3); // keep radius relatively small
            double offsetX = std::cos(angle) * radius;
            double offsetY = std::sin(angle) * radius;

            double spawnX = team->GetOrigin().x + offsetX;
            double spawnY = team->GetOrigin().y + offsetY;

            // Clamp inside screen bounds and make pos
            Vector2 pos{
                (int) std::max(0.0, std::min((double)GetSimWidth() - 1.0, spawnX)),
                (int) std::max(0.0, std::min((double)GetSimHeight() - 1.0, spawnY))
            };

            if(IsPositionOccupied(pos))
                FindFreeSpace(pos, 6);

            stalker->GetLocation() = pos;
            SetMapValue(pos, stalker->GetAgentID());

            // Add to global agent list
            AllAgents.push_back(stalker);

            stalkersToMake--;
        }

        AllTeams.push_back(team);
    }


    // Generate the amount of mutant teams needed
    for(int i = 0; i < numMutantTeams; i++)
    {
        std::cout << "Generating a new MUTANT team." << std::endl;

        // Instantiate the team
        Team* team = new Team();

        // Keep some margin from the edges
        int margin = 4;
        Vector2 teamOrigin = Vector2{GenerateInRange(margin, GetSimWidth() - margin),
            GenerateInRange(margin, GetSimHeight() - margin)};

        if(IsPositionOccupied(teamOrigin))
            FindFreeSpace(teamOrigin, 8);

        team->SetOrigin(teamOrigin);

        // Spread to control how far apart they are; smaller is tighter
        team->SetSpread(std::max(1.0, std::min(60.0, (double)mutantsPerTeam * 2.0)));

        // Pick a random base angle so teams don't all have same orientation
        team->SetAngle(GenerateInRange(0.0, TwoPi));

        for(int j = 0; j < mutantsPerTeam && mutantsToMake > 0; j++)
        {
            std::cout << "Creating a new MUTANT." << std::endl;

            // Create initial stats for the mutant
            Stats stats;
            stats.MovementSpeed += GenerateInRange(0.4, 0.5);
            stats.Strength += GenerateInRange(0.0, 0.3);
            stats.Skill = 0;
            stats.Awareness += GenerateInRange(-0.25, 0.1);
            stats.Sight += GenerateInRange(-0.1, 0.05);
            stats.Morale += GenerateInRange(-0.05, 0.05);
            stats.Hunger += GenerateInRange(0.0, 0.1);
            stats.Thirst += GenerateInRange(0.0, 0.15);

            // Create the mutant and assign as leader if there's none assigned
            Mutant* mutant = new Mutant(this, stats);

            team->AddAgent(mutant);
            mutant->SetAgentTeam(team);
            if(!team->GetTeamLeader())
            {
                team->SetTeamLeader(mutant);
                mutant->AdjustMorale(GenerateInRange(0.1, 0.2)); // Leader should be a little happier
            }

            // Create an inventory for the mutant (just food, acting as meat)
            Inventory inv;
            inv.Food = GenerateInRange(1, 3);
            mutant->GetInventory() = inv;

            // Assign the mutant to its faction
            mutant->SetAgentFaction(Factions::Faction::Mutant);

            // Place agent around the center with small random jitter so they stay close
            double angle = team->GetAngle() + TwoPi * (double)j / (double)mutantsPerTeam + GenerateInRange(-0.15, 0.15);
            double radius = GenerateInRange(team->GetSpread() * 0.05, team->GetSpread() * 0.3); // keep radius relatively small
            double offsetX = std::cos(angle) * radius;
            double offsetY = std::sin(angle) * radius;

            double spawnX = team->GetOrigin().x + offsetX;
            double spawnY = team->GetOrigin().y + offsetY;

            // Clamp inside screen bounds and make pos
            Vector2 pos{
                (int) std::max(0.0, std::min((double)GetSimWidth() - 1.0, spawnX)),
                (int) std::max(0.0, std::min((double)GetSimHeight() - 1.0, spawnY))
            };

            if(IsPositionOccupied(pos))
                FindFreeSpace(pos, 6);

            mutant->GetLocation() = pos;
            SetMapValue(pos, mutant->GetAgentID());

            // Add to global agent list
            AllAgents.push_back(mutant);

            mutantsToMake--;
        }

        AllTeams.push_back(team);
    }
    

    for(int i = 0; i < numCampfires; i++)
    {
        std::cout << "Creating a campfire." << std::endl;

        Vector2 spawnPos{
            GenerateInRange(0, GetSimWidth()),
            GenerateInRange(0, GetSimHeight())
        };

        if(IsPositionOccupied(spawnPos))
            FindFreeSpace(spawnPos, 5);

        EnvironmentEntity* campfire = new EnvironmentEntity(spawnPos, ftxui::Color::Orange1, 'A', 1);
        SetMapValue(spawnPos, 1);

        AllEnvironmentEntites.push_back(campfire);
    }


}

void Zone::AddEntry(std::string log)
{
    ActivityLog.push_back(log);
}

void Zone::Update()
{
    for(AlifeAgent* ag : AllAgents)
        if(ag)
            ag->Update();

    renderer->SetActivityLog(ActivityLog);
    renderer->SetAgents(AllAgents);
    renderer->SetEnvironmentEntities(AllEnvironmentEntites);

    renderer->Display();
}