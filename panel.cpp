#include "main.hpp"

//MapPane Struct
MapPane::MapPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "World", rx, ry)
{
	mapSidePanel = std::make_shared<Pane>(1, 61, UICOLOR_Panel_Ribbon_BG, UICOLOR_Panel_Ribbon_FG);

	world = std::make_shared<World>();
}

void MapPane::update()
{
	world->update();
}

void MapPane::render() const
{
	clearWindow();
	world->render(drawWindow);

	mapSidePanel->render();
	mapSidePanel->console->blit(mapSidePanel->console, 0, 0, 1, 61, TCODConsole::root, 62, 2, 1, 1);

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

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

	//render health bar
	drawWindow->console->printf(0, 0, "Health : [");
	for (int i = 0; i < 20; i++)
	{
		if (i * 5 < WORLD->player->health)
		{
			drawWindow->console->printf(i + 10, 0, "=");
			drawWindow->console->setCharForeground(i + 10, 0, TCODColor::red);
		}
	}
	drawWindow->console->printf(30, 0, "]");

	//render armor bar
	drawWindow->console->printf(0, 1, "Armor  : [");
	for (int i = 0; i < 20; i++)
	{
		if (i * 5 < WORLD->player->armor)
		{
			drawWindow->console->printf(i + 10, 1, "=");
			drawWindow->console->setCharForeground(i + 10, 0, TCODColor::blue);
		}
	}
	drawWindow->console->printf(30, 1, "]");

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

//PlayerPane Struct
PlayerPane::PlayerPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "Player", rx, ry), playerSpeed("Still"), playerStance("Standing")
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

	if (INPUT->baseMoveWait == .5f)
	{
		playerSpeed = "Walking";
	}
	else if (INPUT->baseMoveWait == .25f)
	{
		playerSpeed = "Running";
	}
	else if (INPUT->baseMoveWait == 1.0f)
	{
		playerSpeed = "Creeping";
	}
}

void PlayerPane::render() const
{
	clearWindow();

	drawWindow->console->printf(0, 0, playerStance.c_str());
	drawWindow->console->printf(0, 1, playerSpeed.c_str());

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

InventoryPane::InventoryPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "Inventory", rx, ry)
{
}

void InventoryPane::update()
{
	inventoryItemList = WORLD->player->inventory;
}

void InventoryPane::render() const
{
	clearWindow();

	int drawLine = 0;
	int drawLineStart = 0;

	for (auto& container : inventoryItemList)
	{
		for (int i = 0; i <= container->itemCapacity; i++)
		{
			if (i == 0)
			{
				//error here
				if (container == WORLD->player->inventory[WORLD->player->containerIndex] && WORLD->player->itemIndex == -1)
				{
					drawWindow->console->printf(0, drawLineStart, "|>%s", container->containerItem->tool->name.c_str());
					drawWindow->console->setCharForeground(1, drawLineStart, UICOLOR_Selector);
				}
				else
				{
					drawWindow->console->printf(0, drawLineStart, "| %s", container->containerItem->tool->name.c_str());

				}
				drawLine++;
			}

			else
			{
				drawWindow->console->printf(0, drawLine + i - 1, "|   .");
			}
		}

		for (auto& item : container->itemList)
		{
			for (int j = 0; j < item->size; j++)
			{
				if (j == 0)
				{
					if (item == WORLD->player->selectedItem)
					{
						drawWindow->console->printf(0, drawLine, "|>  %s", item->tool->name.c_str());
						drawWindow->console->setCharForeground(1, drawLine, UICOLOR_Selector);
					}
					else
					{
						drawWindow->console->printf(0, drawLine, "|   %s", item->tool->name.c_str());
					}
					drawLine++;
				}
				else
				{
					drawWindow->console->printf(0, drawLine, "|     |");
					drawLine++;
				}
			}
		}

		drawLineStart += container->itemCapacity + 1;
		drawLine = drawLineStart;
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

SplashPane::SplashPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "EPSILON", rx, ry), menuIndex(0)
{
	menuItemList.push_back("Start");
	menuItemList.push_back("Exit");

	menuSelection = menuItemList[menuIndex];
}

//--------------------------------------------------------------------------------------------

void SplashPane::update()
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

//--------------------------------------------------------------------------------------------

void SplashPane::renderLargeLogo() const
{
	drawWindow->console->printf(48, 20, "|           |");
	drawWindow->console->printf(48, 21, "| |       | |");
	drawWindow->console->printf(48, 22, "| | |   | | |");
	drawWindow->console->printf(48, 24, "E P S I L O N");
	drawWindow->console->printf(48, 26, "| | |   | | |");
	drawWindow->console->printf(48, 27, "| |       | |");
	drawWindow->console->printf(48, 28, "|           |");
}

void SplashPane::renderMenuOptions() const
{
	for (int i = 0; i < menuItemList.size(); i++)
	{
		if (i == menuIndex)
		{
			drawWindow->console->printf(50, 50 + i, "|> %s", menuItemList[i].c_str());
			drawWindow->console->setCharForeground(51, 50 + i, UICOLOR_Selector);
		}
		else
		{
			drawWindow->console->printf(50, 50 + i, "|  %s", menuItemList[i].c_str());
		}
	}
}

void SplashPane::render() const
{
	clearWindow();

	renderLargeLogo();
	renderMenuOptions();

	pushWindow();
}

InventoryFullPane::InventoryFullPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "Inventory", rx, ry)
{
}

void InventoryFullPane::update()
{
	inventoryItemList = WORLD->player->inventory;
}

void InventoryFullPane::render() const
{
	clearWindow();

	int drawLine = 0;
	int drawLineStart = 0;

	for (auto& container : inventoryItemList)
	{
		for (int i = 0; i <= container->itemCapacity; i++)
		{
			if (i == 0)
			{
				//error here
				if (container == WORLD->player->inventory[WORLD->player->containerIndex] && WORLD->player->itemIndex == -1)
				{
					drawWindow->console->printf(0, drawLineStart, "|>%s", container->containerItem->tool->name.c_str());
					drawWindow->console->setCharForeground(1, drawLineStart, UICOLOR_Selector);
				}
				else
				{
					drawWindow->console->printf(0, drawLineStart, "| %s", container->containerItem->tool->name.c_str());

				}
				drawLine++;
			}

			else
			{
				drawWindow->console->printf(0, drawLine + i - 1, "|   .");
			}
		}

		for (auto& item : container->itemList)
		{
			for (int j = 0; j < item->size; j++)
			{
				if (j == 0)
				{
					if (item == WORLD->player->selectedItem)
					{
						drawWindow->console->printf(0, drawLine, "|>  %s", item->tool->name.c_str());
						drawWindow->console->setCharForeground(1, drawLine, UICOLOR_Selector);
					}
					else
					{
						drawWindow->console->printf(0, drawLine, "|   %s", item->tool->name.c_str());
					}
					drawLine++;
				}
				else
				{
					drawWindow->console->printf(0, drawLine, "|     |");
					drawLine++;
				}
			}
		}

		drawLineStart += container->itemCapacity + 1;
		drawLine = drawLineStart;
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

LogPane::LogPane(int windowW, int windowH, int rx, int ry)
	: Window(windowW, windowH, "Log", rx, ry)
{
}

void LogPane::pushMessage(std::string message)
{
	messageList.push_back(message);
}

void LogPane::update()
{
}

void LogPane::render() const
{
	clearWindow();

	for (int i = 0; i < messageList.size(); i++)
	{
		drawWindow->console->printf(0, i, messageList[i].c_str());
	}

	pushWindow();
}

ProximityPane::ProximityPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "Proximity", rx, ry)
{
}

void ProximityPane::update()
{
	proximityContainerList.clear();
	proximityItemList.clear();

	for (auto& container : WORLD->mapContainerList)
	{
		if (container->containerItem->distToEnt < 5)
		{
			proximityContainerList.push_back(container);
		}
	}

	for (auto& item : WORLD->mapItemList)
	{
		if (item->distToEnt < 5)
		{
			proximityItemList.push_back(item);
		}
	}
}

void ProximityPane::render() const
{
	clearWindow();

	for (int line = 0; line < proximityContainerList.size() + proximityItemList.size();)
	{
		for (auto& container : proximityContainerList)
		{
			drawWindow->console->printf(0, line, "|%s", container->containerItem->tool->name.c_str());
			line++;
		}

		for (auto& item : proximityItemList)
		{
			drawWindow->console->printf(0, line, "|%s", item->tool->name.c_str());
			line++;
		}
	}

	pushWindow();
}

ActionPane::ActionPane(int windowW, int windowH, int rx, int ry)
	:Window(windowW, windowH, "Actions", rx, ry)
{
}

void ActionPane::update()
{
	if (WORLD->player->selectedItem->actionManager)
	{
		actionManager = WORLD->player->selectedItem->actionManager;
	}
}

void ActionPane::render() const
{
	clearWindow();

	if (actionManager) //needed because will throw mydata no data error
	{
		for (int line = 0; line < actionManager->actionList.size();)
		{
			for (auto& action : actionManager->actionList)
			{
				if (action == actionManager->selectedAction)
				{
					drawWindow->console->printf(0, line, "|>%s", action->name.c_str());
				}
				else
				{
					drawWindow->console->printf(0, line, "| %s", action->name.c_str());
				}

				line++;
			}
		}
	}

	pushWindow();
}
