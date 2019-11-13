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
	displayHealth = WORLD->player->health;
	displayArmor = WORLD->player->armor;
}

void StatusPane::render() const
{
	clearWindow();

	drawWindow->console->printf(0, 0, "Health : [%i/100]", WORLD->player->health);
	drawWindow->console->printf(0, 1, "Armor  : [%i/100]", WORLD->player->armor);
	pushWindow();
}

//PlayerPane Struct
PlayerPane::PlayerPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "Player", rx, ry), playerSpeed("still"), playerStance("Standing")
{
}

void PlayerPane::update()
{
	if (WORLD->player->height == 1)
	{
		playerStance = "Prone";
	}
	else if (WORLD->player->height == 2)
	{
		playerStance = "Crouching";
	}
	else if (WORLD->player->height == 3)
	{
		playerStance = "Standing";
	}

	if (INPUT->baseMoveWait == 30)
	{
		playerSpeed = "Walking";
	}
	else if (INPUT->baseMoveWait == 15)
	{
		playerSpeed = "Running";
	}
	else if (INPUT->baseMoveWait == 60)
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

InventoryPane::InventoryPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "Inventory", rx, ry)
{
}

void InventoryPane::update()
{
	//for (int i = 0; i < engine->gui->mapPane->world->player->inventory.size() - 1; i++)
	//{
	//	//itemNameList[i] = engine->gui->mapPane->world->player->inventory[i]->name;
	//	//if (i < engine->gui->mapPane->world->player->inventory.size())
	//	//{
	//		itemNameList.push_back(engine->gui->mapPane->world->player->inventory[i]->name);
	//	//}
	//}
}

void InventoryPane::render() const
{
	clearWindow();

	//for (int i = 0; i < drawWindow->consoleH; i++)
	//{
	//	//drawWindow->console->printf(0, i, itemNameList[i]);
	//	if (i < engine->gui->mapPane->world->player->inventory.size())
	//	{
	//		if (engine->gui->mapPane->world->player->inventory[i]->active)
	//		{
	//			drawWindow->console->printf(0, i, "|>%s", engine->gui->mapPane->world->player->inventory[i]->name);
	//		}
	//		else
	//		{
	//			drawWindow->console->printf(0, i, "| %s", engine->gui->mapPane->world->player->inventory[i]->name);
	//		}
	//	}
	//}

	pushWindow();
}

SplashPanel::SplashPanel(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "EPSILON", rx, ry), menuIndex(0)
{
	menuItemList.push_back("Start");
	menuItemList.push_back("Exit");

	menuSelection = menuItemList[menuIndex];
}

void SplashPanel::update()
{
	menuSelection = menuItemList[menuIndex];

	if (INPUT->w->isSwitched)
	{
		if (menuIndex > 0)
		{
			menuIndex--;
		}
	}
	if (INPUT->s->isSwitched)
	{
		if (menuIndex < menuItemList.size() - 1)
		{
			menuIndex++;
		}
	}

	if (INPUT->space->isSwitched)
	{
		if (menuItemList[menuIndex] == "Start")
		{
			GUI->activeWindow = Gui::NONE;
		}
		else if (menuItemList[menuIndex] == "Exit")
		{
			engine->gamestate = Engine::EXIT;
		}
	}
}

void SplashPanel::render() const
{
	clearWindow();

	/*
	|           |
	| |       | |
	| | |   | | |
	| | | | | | |         
	E P S I L O N
	| | | | | | |
	| | |   | | |
	| |       | |
	|           |
	*/

	drawWindow->console->printf(48, 20, "|           |");
	drawWindow->console->printf(48, 21, "| |       | |");
	drawWindow->console->printf(48, 22, "| | |   | | |");
	drawWindow->console->printf(48, 23, "| | | | | | |");
	drawWindow->console->printf(48, 24, "E P S I L O N");
	drawWindow->console->printf(48, 25, "| | | | | | |");
	drawWindow->console->printf(48, 26, "| | |   | | |");
	drawWindow->console->printf(48, 27, "| |       | |");
	drawWindow->console->printf(48, 28, "|           |");

	for (int i = 0; i < menuItemList.size(); i++)
	{
		if (i == menuIndex)
		{
			drawWindow->console->printf(50, 50 + i, "|> %s", menuItemList[i]);
		}
		else
		{
			drawWindow->console->printf(50, 50 + i, "|  %s", menuItemList[i]);
		}
	}

	pushWindow();
}
