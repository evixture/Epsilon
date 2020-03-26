#include "main.hpp"

MapWindow::MapWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "World", rx, ry)
{
	//actual map dims should be 60x60
	mapSidePanel = std::make_shared<Pane>(1, 61, ep::color::ribbonBG, ep::color::ribbonFG);
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
	mapSidePanel->console->blit(mapSidePanel->console, 0, 0, 1, 61, TCODConsole::root, 61, 2, 1, 1);

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

StatusWindow::StatusWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "Status", rx, ry), displayHealth(0), displayArmor(0)
{}

void StatusWindow::update()
{
	if (displayHealth != WORLD->debugmap->player->health)
	{
		displayHealth = WORLD->debugmap->player->health;
	}

	if (displayArmor != WORLD->debugmap->player->equippedArmor.defense)
	{
		displayArmor =  WORLD->debugmap->player->equippedArmor.defense;
	}
}

void StatusWindow::render() const
{
	clearWindow();

	drawWindow->console->printf(0, 0, "Health : [");
	for (int i = 0; i < 20; ++i)
	{
		if (displayHealth != -1)
		{
			if (i * 5 < displayHealth)
			{
				drawWindow->console->printf(i + 10, 0, "=");
				drawWindow->console->setCharForeground(i + 10, 0, TCODColor::red);
			}
		}
		else
		{
			drawWindow->console->printf(i + 10, 0, "!");
			drawWindow->console->setCharForeground(i + 10, 0, TCODColor::red);
		}
	}
	drawWindow->console->printf(30, 0, "]");

	drawWindow->console->printf(0, 1, "Armor  : [");
	for (int i = 0; i < 20; ++i)
	{
		if (i * 5 < displayArmor)
		{
			drawWindow->console->printf(i + 10, 1, "=");
			drawWindow->console->setCharForeground(i + 10, 1, TCODColor::blue);
		}
	}
	drawWindow->console->printf(30, 1, "]");

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

PlayerWindow::PlayerWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "Player", rx, ry), playerSpeed(0), playerStance(0)
{}

void PlayerWindow::update()
{
	playerStance = WORLD->debugmap->player->mapPosition.height;

	if (!(INPUT->moveUpKey->isDown) && !(INPUT->moveDownKey->isDown) && !(INPUT->moveLeftKey->isDown) && !(INPUT->moveRightKey->isDown)) playerSpeed = 0;
	else if (WORLD->debugmap->player->baseMoveTime == .25f) playerSpeed = 3;
	else if (WORLD->debugmap->player->baseMoveTime == .5f) playerSpeed = 2;
	else if (WORLD->debugmap->player->baseMoveTime == 1.0f) playerSpeed = 1;
}

void PlayerWindow::renderSpeedAndStance() const
{
	drawWindow->console->setChar(0, 8, ep::character::table);
	drawWindow->console->setChar(0, 7, ep::character::bottomSquareBrace);
	drawWindow->console->setChar(0, 0, ep::character::topSquareBrace);

	drawWindow->console->setChar(1, 8, '[');
	drawWindow->console->setChar(8, 8, ']');


	for (int x = 0; x < playerSpeed * 2; x++) //draw speed graph
	{
		drawWindow->console->setChar(x + 2, 8, '=');
		drawWindow->console->setCharForeground(x + 2, 8, TCODColor::red);
	}

	for (int y = 0; y < playerStance * 2; y++) //draw height graph
	{
		drawWindow->console->setChar(0, (6 - y), ep::character::verticalEquals);
		drawWindow->console->setCharForeground(0, (6 - y), TCODColor::blue);
	}

	for (int y = 0; y < playerStance * 2; y++)
	{
		for (int x = 0; x < playerSpeed * 2; x++)
		{
			drawWindow->console->setChar((x + 2), (6 - y), '\\');
			drawWindow->console->setCharForeground((x + 2), (6 - y), TCODColor::purple);
		}
	}
}

void PlayerWindow::render() const
{
	clearWindow();

	renderSpeedAndStance();

	pushWindow();
}


//----------------------------------------------------------------------------------------------------

InventoryWindow::InventoryWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "Inventory", rx, ry)
{}

void InventoryWindow::update()
{
	if (inventoryItemList != WORLD->debugmap->player->inventory)
	{
		inventoryItemList = WORLD->debugmap->player->inventory;
	}
}

void InventoryWindow::render() const
{
	clearWindow();

	int drawLine = 0;
	int drawLineStart = 0;

	for (auto& container : inventoryItemList)
	{
		for (int i = 0; i <= container->itemCapacity; ++i)
		{
			if (i == 0)
			{
				if (container == WORLD->debugmap->player->inventory[WORLD->debugmap->player->containerIndex] && WORLD->debugmap->player->itemIndex == -1)
				{
					drawWindow->console->printf(0, drawLineStart, "|>%s", container->containerItem->tool->name.c_str());
					drawWindow->console->setCharForeground(1, drawLineStart, ep::color::selector); //color the cursor
					drawWindow->console->setCharForeground(0, drawLineStart, WORLD->debugmap->player->inventory[WORLD->debugmap->player->containerIndex]->containerItem->barColor); //color the bar
				}
				else
				{
					drawWindow->console->printf(0, drawLineStart, "| %s", container->containerItem->tool->name.c_str());
					drawWindow->console->setCharForeground(0, drawLineStart, WORLD->debugmap->player->inventory[WORLD->debugmap->player->containerIndex]->containerItem->barColor); //color the bar
				}
				++drawLine;
			}
			else
			{
				drawWindow->console->printf(0, drawLine + i - 1, "|   .");
			}
		}

		for (auto& item : container->itemList)
		{
			for (int j = 0; j < item->size; ++j)
			{
				if (j == 0)
				{
					if (item == WORLD->debugmap->player->selectedItem)
					{
						drawWindow->console->printf(0, drawLine, "|>  %s", item->tool->name.c_str());
						drawWindow->console->setCharForeground(1, drawLine, ep::color::selector);
						drawWindow->console->setCharForeground(0, drawLine, item->barColor); //color the bar
					}
					else
					{
						drawWindow->console->printf(0, drawLine, "|   %s", item->tool->name.c_str());
						drawWindow->console->setCharForeground(0, drawLine, item->barColor); //color the bar
					}
					++drawLine;
				}
				else
				{
					drawWindow->console->printf(0, drawLine, "|     |");
					++drawLine;
				}
			}
		}

		drawLineStart += container->itemCapacity + 1;
		drawLine = drawLineStart;
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

SplashWindow::SplashWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "EPSILON", rx, ry), menu(Menu(std::vector<std::string>{"Start", "Exit"})), slashClock(Clock(.03f)), numSlashIndexes(57) //best at higher numbers
{
	for (int i = 0; i < (drawWindow->consoleWidth * drawWindow->consoleHeight) / 4; i++)
	{
		slashList.push_back(0);
	}

	
	for (int i = 0; i < numSlashIndexes; i++)
	{
		int val;

		if (i == 0) val = 0;
		else
		{
			val = (int)(slashList.size() / numSlashIndexes) * i;
		}

		slashIndexList.push_back(val);
	}
}

void SplashWindow::update()
{
	slashClock.tickUp();

	for (int i = 1; i <= slashClock.numCalls; slashClock.numCalls--)
	{
		for (auto& index : slashIndexList)
		{
			slashList[index]++;

			if (slashList[index] > 3)
			{
				slashList[index] = 0;
			}
		
			if (index + 1 < slashList.size())
			{
				index++;
			}
			else
			{
				index = 0;
			}
		}
	}

	menu.update();

	if (INPUT->worldInteractKey->isSwitched)
	{
		if (menu.menuSelection == "Start")
		{
			GUI->activeWindow = Gui::ActiveWindow::NONE;
		}
		else if (menu.menuSelection == "Exit")
		{
			engine->gamestate = Engine::Gamestate::EXIT;
		}
	}
}

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
	//menu render
	menu.render(drawWindow, 50, 50);
}

void SplashWindow::render() const
{
	clearWindow();

	int slashWidth = ((drawWindow->consoleWidth / 2) - (drawWindow->consoleWidth % 2));
	int slashHeight = ((drawWindow->consoleHeight / 2) - (drawWindow->consoleHeight % 2));

	for (int y = 0; y < slashHeight; y++)
	{
		for (int x = 0; x < slashWidth; x++)
		{
			if (((x < 46 / 2) || (x > 60 / 2)) || ((y < 18 / 2) || (y > 28 / 2))) /*not in logo*/
			{
				if (((x < 48 / 2) || (x > 58 / 2)) || ((y < 48 / 2) || (y > 52 / 2))) /*not in menu options*/
				{
					switch (slashList[x + y * slashWidth])
					{
					case 0:
						drawWindow->console->setChar(1 + (x * 2), 1 + (y * 2), '|');
						drawWindow->console->setCharForeground(1 + (x * 2), 1 + (y * 2), TCODColor::darkerGrey);
						break;
					case 1:
						drawWindow->console->setChar(1 + (x * 2), 1 + (y * 2), '/');
						drawWindow->console->setCharForeground(1 + (x * 2), 1 + (y * 2), TCODColor::darkerGrey);
						break;
					case 2:
						drawWindow->console->setChar(1 + (x * 2), 1 + (y * 2), '-');
						drawWindow->console->setCharForeground(1 + (x * 2), 1 + (y * 2), TCODColor::darkerGrey);
						break;
					case 3:
						drawWindow->console->setChar(1 + (x * 2), 1 + (y * 2), '\\');
						drawWindow->console->setCharForeground(1 + (x * 2), 1 + (y * 2), TCODColor::darkerGrey);
						break;
					}
				}
			}
		}
	}

	renderLargeLogo();
	renderMenuOptions();

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

InventoryFullWindow::InventoryFullWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "Inventory", rx, ry)
{}

void InventoryFullWindow::update()
{
	if (inventoryItemList != WORLD->debugmap->player->inventory)
	{
		inventoryItemList = WORLD->debugmap->player->inventory;
	}
}

void InventoryFullWindow::render() const
{
	clearWindow();

	int drawLine = 0;
	int drawLineStart = 0;

	for (auto& container : inventoryItemList)
	{
		for (int i = 0; i <= container->itemCapacity; ++i)
		{
			if (i == 0)
			{
				if (container == WORLD->debugmap->player->inventory[WORLD->debugmap->player->containerIndex] && WORLD->debugmap->player->itemIndex == -1)
				{
					drawWindow->console->printf(0, drawLineStart, "|>%s", container->containerItem->tool->name.c_str());
					drawWindow->console->setCharForeground(1, drawLineStart, ep::color::selector);
				}
				else
				{
					drawWindow->console->printf(0, drawLineStart, "| %s", container->containerItem->tool->name.c_str());

				}
				++drawLine;
			}
			else
			{
				drawWindow->console->printf(0, drawLine + i - 1, "|   .");
			}
		}

		for (auto& item : container->itemList)
		{
			for (int j = 0; j < item->size; ++j)
			{
				if (j == 0)
				{
					if (item == WORLD->debugmap->player->selectedItem)
					{
						drawWindow->console->printf(0, drawLine, "|>  %s", item->tool->name.c_str());
						drawWindow->console->setCharForeground(1, drawLine, ep::color::selector);
					}
					else
					{
						drawWindow->console->printf(0, drawLine, "|   %s", item->tool->name.c_str());
					}
					++drawLine;
				}
				else
				{
					drawWindow->console->printf(0, drawLine, "|     |");
					++drawLine;
				}
			}
		}

		drawLineStart += container->itemCapacity + 1;
		drawLine = drawLineStart;
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

LogWindow::Message::Message(std::string message, MessageLevel messageLevel)
	:message(message), messageLevel(messageLevel)
{
	if (messageLevel == Message::MessageLevel::HIGH) color = ep::color::messageHigh;
	else if (messageLevel == Message::MessageLevel::MEDIUM) color = ep::color::messageMed;
	else color = ep::color::messageLow;
}

//----------------------------------------------------------------------------------------------------

LogWindow::LogWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "Log", rx, ry)
{}

void LogWindow::pushMessage(Message message)
{
	messageList.insert(messageList.begin(), message);
}

void LogWindow::update()
{
	while (messageList.size() > consoleHeight)
	{
		messageList.erase(messageList.begin() + messageList.size() - 1);
	}
}

void LogWindow::render() const
{
	clearWindow();

	int line = 0;
	for (int i = 0; i < messageList.size(); ++i)
	{
		if (i == 0)
		{
			drawWindow->console->printf(0, line, "|>%s", messageList[i].message.c_str());
		}
		else
		{
			drawWindow->console->printf(0, line, "| %s",  messageList[i].message.c_str());
		}

		for (int j = 0; j < drawWindow->consoleWidth; ++j) //set string message color
		{
			if (i == 0 && j == 1)
			{
				drawWindow->console->setCharForeground(j, line, ep::color::selector);
			}
			else
			{
				drawWindow->console->setCharForeground(j, line, messageList[i].color);
			}
		}
		++line;
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

ProximityWindow::ProximityWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "Proximity", rx, ry)
{}

void ProximityWindow::update()
{
	if (proximityItemList != WORLD->debugmap->mapItemList)
	{
		proximityItemList = WORLD->debugmap->mapItemList; //can be optimized later
	}

	if (proximityContainerList != WORLD->debugmap->mapContainerList)
	{
		proximityContainerList = WORLD->debugmap->mapContainerList;
	}
}

void ProximityWindow::render() const
{
	clearWindow();

	int line = 0;

	for (auto& container : proximityContainerList)
	{
		if (container->containerItem->mapPosition.floor == WORLD->debugmap->player->mapPosition.floor)
		{
			if (container->containerItem->distToEnt < 5 && WORLD->isInPlayerFov(container->containerItem->mapPosition))
			{
				drawWindow->console->printf(0, line, "|%s", container->containerItem->tool->name.c_str());
				++line;
			}
		}
	}

	for (auto& item : proximityItemList)
	{
		if (item->mapPosition.floor == WORLD->debugmap->player->mapPosition.floor)
		{
			if (item->distToEnt < 5 && WORLD->isInPlayerFov(item->mapPosition))
			{
				drawWindow->console->printf(0, line, "|%s", item->tool->name.c_str());
				++line;
			}
		}
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

ActionWindow::ActionWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "Actions", rx, ry)
{}

void ActionWindow::update()
{
	if (WORLD->debugmap->player->selectedItem->actionManager)
	{
		if (actionManager != WORLD->debugmap->player->selectedItem->actionManager)
		{
			actionManager = WORLD->debugmap->player->selectedItem->actionManager;
		}
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
					drawWindow->console->setCharForeground(1, line, ep::color::selector);
				}
				else
				{
					drawWindow->console->printf(0, line, "| %s", action->name.c_str());
				}

				++line;
			}
		}
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

PauseWindow::PauseWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	:Window(consoleWidth, consoleHeight, "Paused", rx, ry), 
	baseMenu(std::vector<std::string>{"Return", "Settings", "Exit"}), settingsMenu(std::vector<std::string>{"Input", "Video", "Back"}), 
	baseMenuActive(true), settingsMenuActive(false)
{
}

void PauseWindow::update()
{
	if (baseMenuActive)
	{
		baseMenu.update();
	}
	else if (settingsMenuActive)
	{
		settingsMenu.update();
	}

	if (INPUT->worldInteractKey->isSwitched)
	{
		if (baseMenuActive)
		{
			if (baseMenu.menuSelection == "Return")
			{
				GUI->activeWindow = Gui::ActiveWindow::NONE;
			}
			else if (baseMenu.menuSelection == "Settings")
			{
				baseMenuActive = false;
				settingsMenuActive = true;
			}
			else if (baseMenu.menuSelection == "Exit")
			{
				ENGINE->gamestate = Engine::Gamestate::EXIT;
			}
		}
		else if (settingsMenuActive)
		{
			if (settingsMenu.menuSelection == "Input")
			{
				//enter input menu
			}
			else if (settingsMenu.menuSelection == "Video")
			{
				//enter video menu
			}
			else if (settingsMenu.menuSelection == "Back")
			{
				settingsMenuActive = false;
				baseMenuActive = true;
			}
		}
	}

	if (INPUT->menuKey->isSwitched && GUI->activeWindow == Gui::ActiveWindow::PAUSE)
	{
		if (baseMenuActive)
		{
			GUI->activeWindow = Gui::ActiveWindow::NONE;
		}
		else if (settingsMenuActive)
		{
			settingsMenuActive = false;
			baseMenuActive = true;
		}
	}
}

void PauseWindow::render() const
{
	clearWindow();

	if (baseMenuActive)
	{
		baseMenu.render(drawWindow, 50, 50);
	}
	else if (settingsMenuActive)
	{
		settingsMenu.render(drawWindow, 50, 50);
	}

	pushWindow();
}

InfoWindow::InfoWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	: Window(consoleWidth, consoleHeight, "Information", rx, ry), tileDetail(""), creatureDetail(""), itemDetail("")

{
}

void InfoWindow::setTileDetails()
{
	tileDetail = "---";

	if (INPUT->mouse.cx >= 1 && INPUT->mouse.cx <= 60 && INPUT->mouse.cy >= 3 && INPUT->mouse.cy <= 62) //if in map window
	{
		if (WORLD->debugmap->getBlock(Position3(INPUT->mouse.cx + WORLD->xOffset - 1, INPUT->mouse.cy + WORLD->yOffset - 3, WORLD->debugmap->player->mapPosition.floor))->explored)
		{
			tileDetail = WORLD->debugmap->getBlock(Position3(INPUT->mouse.cx + WORLD->xOffset - 1, INPUT->mouse.cy + WORLD->yOffset - 3, WORLD->debugmap->player->mapPosition.floor))->
				getTileData(WORLD->debugmap->player->mapPosition.height).name;
		}
	}
}

void InfoWindow::setCreatureDetails()
{
	creatureDetail = "---";

	for (auto& creature : WORLD->debugmap->creatureList)
	{
		if (creature->mapPosition.floor == WORLD->debugmap->player->mapPosition.floor)
		{
			if (creature->mapPosition.x == INPUT->mouse.cx + WORLD->xOffset - 1 && creature->mapPosition.y == INPUT->mouse.cy + WORLD->yOffset - 3)
			{
				if (WORLD->isInPlayerFov(creature->mapPosition))
				{
					creatureDetail = creature->name;
				}
			}
		}
	}
}

void InfoWindow::setItemDetails()
{
	itemDetail = WORLD->debugmap->player->selectedItem->tool->name;
}

void InfoWindow::update()
{
	setTileDetails();
	
	setCreatureDetails();
	
	setItemDetails();
}

void InfoWindow::render() const
{
	clearWindow();

	drawWindow->console->printf(0, 0, "Tile : %s", tileDetail.c_str());
	drawWindow->console->printf(0, 1, "Creature : %s", creatureDetail.c_str());
	drawWindow->console->printf(0, 2, "Item : %s", itemDetail.c_str()); //will use later to show in depth description of held item

	pushWindow();
}
