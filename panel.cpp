#include "main.hpp"

//MapWindow Struct
MapWindow::MapWindow(int consoleWidth, int windowH, int rx, int ry)
	:Window(consoleWidth, windowH, "World", rx, ry)
{
	mapSidePanel = std::make_shared<Pane>(1, 61, UICOLOR_Panel_Ribbon_BG, UICOLOR_Panel_Ribbon_FG);

	world = std::make_shared<World>();
}

void MapWindow::update()
{
	world->update();
}

void MapWindow::render() const
{
	clearWindow();
	world->render(drawWindow);

	mapSidePanel->render();
	mapSidePanel->console->blit(mapSidePanel->console, 0, 0, 1, 61, TCODConsole::root, 62, 2, 1, 1);

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

//StatusWindow Struct
StatusWindow::StatusWindow(int consoleWidth, int windowH, int rx, int ry)
	:Window(consoleWidth, windowH, "Status", rx, ry), displayHealth(0), displayArmor(0)
{
}

void StatusWindow::update()
{
	displayHealth = WORLD->player->health;
	displayArmor = WORLD->player->armor;
}

void StatusWindow::render() const
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

//PlayerWindow Struct
PlayerWindow::PlayerWindow(int consoleWidth, int windowH, int rx, int ry)
	:Window(consoleWidth, windowH, "Player", rx, ry), playerSpeed("Still"), playerStance("Standing")
{
}

void PlayerWindow::update()
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

	if (INPUT->baseMoveTime == .5f)
	{
		playerSpeed = "Walking";
	}
	else if (INPUT->baseMoveTime == .25f)
	{
		playerSpeed = "Running";
	}
	else if (INPUT->baseMoveTime == 1.0f)
	{
		playerSpeed = "Creeping";
	}
}

void PlayerWindow::render() const
{
	clearWindow();

	drawWindow->console->printf(0, 0, playerStance.c_str());
	drawWindow->console->printf(0, 1, playerSpeed.c_str());

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

InventoryWindow::InventoryWindow(int consoleWidth, int windowH, int rx, int ry)
	:Window(consoleWidth, windowH, "Inventory", rx, ry)
{
}

void InventoryWindow::update()
{
	inventoryItemList = WORLD->player->inventory;
}

void InventoryWindow::render() const
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

SplashWindow::SplashWindow(int consoleWidth, int windowH, int rx, int ry)
	:Window(consoleWidth, windowH, "EPSILON", rx, ry), menuIndex(0)
{
	menuItemList.push_back("Start");
	menuItemList.push_back("Exit");

	menuSelection = menuItemList[menuIndex];
}

//--------------------------------------------------------------------------------------------

void SplashWindow::update()
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

void SplashWindow::renderLargeLogo() const
{
	drawWindow->console->printf(48, 20, "|           |");
	drawWindow->console->printf(48, 21, "| |       | |");
	drawWindow->console->printf(48, 22, "| | |   | | |");
	drawWindow->console->printf(48, 24, "E P S I L O N");
	drawWindow->console->printf(48, 26, "| | |   | | |");
	drawWindow->console->printf(48, 27, "| |       | |");
	drawWindow->console->printf(48, 28, "|           |");
}

void SplashWindow::renderMenuOptions() const
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

void SplashWindow::render() const
{
	clearWindow();

	renderLargeLogo();
	renderMenuOptions();

	pushWindow();
}

InventoryFullWindow::InventoryFullWindow(int consoleWidth, int windowH, int rx, int ry)
	:Window(consoleWidth, windowH, "Inventory", rx, ry)
{
}

void InventoryFullWindow::update()
{
	inventoryItemList = WORLD->player->inventory;
}

void InventoryFullWindow::render() const
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

LogWindow::LogWindow(int consoleWidth, int windowH, int rx, int ry)
	: Window(consoleWidth, windowH, "Log", rx, ry)
{
}

void LogWindow::pushMessage(std::string message)
{
	messageList.push_back(message);
}

void LogWindow::update()
{
}

void LogWindow::render() const
{
	clearWindow();

	for (int i = 0; i < messageList.size(); i++)
	{
		drawWindow->console->printf(0, i, messageList[i].c_str());
	}

	pushWindow();
}

ProximityWindow::ProximityWindow(int consoleWidth, int windowH, int rx, int ry)
	:Window(consoleWidth, windowH, "Proximity", rx, ry)
{
}

void ProximityWindow::update()
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

void ProximityWindow::render() const
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

ActionWindow::ActionWindow(int consoleWidth, int windowH, int rx, int ry)
	:Window(consoleWidth, windowH, "Actions", rx, ry)
{
}

void ActionWindow::update()
{
	if (WORLD->player->selectedItem->actionManager)
	{
		actionManager = WORLD->player->selectedItem->actionManager;
	}
}

void ActionWindow::render() const
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
