#include "main.hpp"

//MapPane Struct
MapPane::MapPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "World", rx, ry)
{
	mapSidePanel = std::make_shared<Pane>(1, 61, RIBONBGCOLOR, RIBONFGCOLOR);

	world = std::make_shared<World>();
	//map->createMap(map->debugmap);
}

void MapPane::update()
{
	world->update();
	//setRibonName(world->debugmap->mapName);
}

void MapPane::render() const
{
	clearWindow();
	world->render(drawWindow);

	mapSidePanel->render();
	mapSidePanel->console->blit(mapSidePanel->console, 0, 0, 1, 61, TCODConsole::root, 62, 2, 1, 1);

	pushWindow();
}

//StatusPane Struct
StatusPane::StatusPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "Status", rx, ry), displayHealth(0), displayArmor(0)
{

}

void StatusPane::update()
{
	displayHealth = engine->gui->mapPane->world->player->health;
	displayArmor = engine->gui->mapPane->world->player->armor;
}

void StatusPane::render() const
{
	clearWindow();

	drawWindow->console->printf(0, 0, "Health : [%i/100]", engine->gui->mapPane->world->player->health);
	drawWindow->console->printf(0, 1, "Armor  : [%i/100]", engine->gui->mapPane->world->player->armor);
	pushWindow();
}

//PlayerPane Struct
PlayerPane::PlayerPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "Player", rx, ry), playerSpeed("still"), playerStance("Standing")
{
}

void PlayerPane::update()
{
	if (engine->gui->mapPane->world->player->height == 1)
	{
		playerStance = "Prone";
	}
	else if (engine->gui->mapPane->world->player->height == 2)
	{
		playerStance = "Crouching";
	}
	else if (engine->gui->mapPane->world->player->height == 3)
	{
		playerStance = "Standing";
	}

	if (engine->settings->input->baseMoveWait == 30)
	{
		playerSpeed = "Walking";
	}
	else if (engine->settings->input->baseMoveWait == 15)
	{
		playerSpeed = "Running";
	}
	else if (engine->settings->input->baseMoveWait == 60)
	{
		playerSpeed = "Creeping";
	}
}

void PlayerPane::render() const
{
	clearWindow();

	drawWindow->console->printf(0, 0, playerStance);
	drawWindow->console->printf(0, 1, playerSpeed);

	pushWindow();
}
