#include "main.hpp"

MapWindow::MapWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	: Window(consoleWidth, consoleHeight, "World", rx, ry)
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

	world->render(drawPane);
	mapSidePanel->render();
	mapSidePanel->console->blit(mapSidePanel->console, 0, 0, 1, 61, TCODConsole::root, 61, 2, 1, 1);

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

StatusWindow::StatusWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	: Window(consoleWidth, consoleHeight, "Status", rx, ry), healthBar(Bar("Health", TCODColor::red, 20, 100, 0, Position3(0, 0, 0))), armorBar(Bar("Armor ", TCODColor::blue, 20, 100, 0, Position3(0, 1, 0)))
{
}

void StatusWindow::update()
{
	healthBar.setBarValue(WORLD->debugmap->player->health);
	armorBar.setBarValue(WORLD->debugmap->player->equippedArmor.defense);
}

void StatusWindow::render() const
{
	clearWindow();

	healthBar.render(drawPane);
	armorBar.render(drawPane);

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

PlayerWindow::PlayerWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	: Window(consoleWidth, consoleHeight, "Player", rx, ry), playerSpeed(0), playerStance(0)
{
}

void PlayerWindow::update()
{
	playerStance = WORLD->debugmap->player->mapPosition.h;

	if (!(INPUT->moveUpKey->isDown) && !(INPUT->moveDownKey->isDown) && !(INPUT->moveLeftKey->isDown) && !(INPUT->moveRightKey->isDown)) playerSpeed = 0;
	else if (WORLD->debugmap->player->baseMoveTime == .25f) playerSpeed = 3;
	else if (WORLD->debugmap->player->baseMoveTime == .5f)	playerSpeed = 2;
	else if (WORLD->debugmap->player->baseMoveTime == 1.0f) playerSpeed = 1;
}

void PlayerWindow::renderSpeedAndStance() const
{
	drawPane.console->setChar(0, 8, ep::character::table);
	drawPane.console->setChar(0, 7, ep::character::bottomSquareBrace);
	drawPane.console->setChar(0, 0, ep::character::topSquareBrace);

	drawPane.console->setChar(1, 8, '[');
	drawPane.console->setChar(8, 8, ']');

	for (int x = 0; x < playerSpeed * 2; x++) //draw speed graph
	{
		drawPane.console->setChar(x + 2, 8, '=');
		drawPane.console->setCharForeground(x + 2, 8, TCODColor::red);
	}

	for (int y = 0; y < playerStance * 2; y++) //draw height graph
	{
		drawPane.console->setChar(0, (6 - y), ep::character::verticalEquals);
		drawPane.console->setCharForeground(0, (6 - y), TCODColor::blue);
	}

	for (int y = 0; y < playerStance * 2; y++)
	{
		for (int x = 0; x < playerSpeed * 2; x++)
		{
			drawPane.console->setChar((x + 2), (6 - y), '\\');
			drawPane.console->setCharForeground((x + 2), (6 - y), TCODColor::purple);
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
	: Window(consoleWidth, consoleHeight, "Inventory", rx, ry)
{
}

void InventoryWindow::update()
{
	if (inventoryItemList != WORLD->debugmap->player->inventory) inventoryItemList = WORLD->debugmap->player->inventory;
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
					drawPane.console->printf(0, drawLineStart, "|>%s", container->item->tool->name.c_str());
					drawPane.console->setCharForeground(1, drawLineStart, ep::color::selector); //color the cursor
					drawPane.console->setCharForeground(0, drawLineStart, WORLD->debugmap->player->inventory[WORLD->debugmap->player->containerIndex]->item->barColor); //color the bar
				}
				else
				{
					drawPane.console->printf(0, drawLineStart, "| %s", container->item->tool->name.c_str());
					drawPane.console->setCharForeground(0, drawLineStart, WORLD->debugmap->player->inventory[WORLD->debugmap->player->containerIndex]->item->barColor); //color the bar
				}
				++drawLine;
			}
			else
			{
				drawPane.console->printf(0, drawLine + i - 1, "|   .");
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
						drawPane.console->printf(0, drawLine, "|>  %s", item->tool->name.c_str());
						drawPane.console->setCharForeground(1, drawLine, ep::color::selector);
						drawPane.console->setCharForeground(0, drawLine, item->barColor); //color the bar
					}
					else
					{
						drawPane.console->printf(0, drawLine, "|   %s", item->tool->name.c_str());
						drawPane.console->setCharForeground(0, drawLine, item->barColor); //color the bar
					}
					++drawLine;
				}
				else
				{
					drawPane.console->printf(0, drawLine, "|     |");
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
	: Window(consoleWidth, consoleHeight, "EPSILON", rx, ry), menu(Menu(std::vector<std::string>{"Start", "Exit"})), slashClock(Clock(.03f)), numSlashIndexes(57) //best at higher numbers
{
	for (int i = 0; i < (drawPane.consoleWidth * drawPane.consoleHeight) / 4; i++) slashList.push_back(0);
	
	for (int i = 0; i < numSlashIndexes; i++)
	{
		int val;

		if (i == 0) val = 0;
		else		val = (int)(slashList.size() / numSlashIndexes) * i;

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

			if (slashList[index] > 3) slashList[index] = 0;
		
			if (index + 1 < slashList.size())	index++;
			else								index = 0;
		}
	}

	menu.update();

	if (INPUT->worldInteractKey->isSwitched)
	{
		if		(menu.menuSelection == "Start") GUI->activeWindow = Gui::ActiveWindow::NONE;
		else if (menu.menuSelection == "Exit")	engine->gamestate = Engine::Gamestate::EXIT;
	}
}

void SplashWindow::renderLargeLogo() const
{
	drawPane.console->printf(48, 20, "|           |");
	drawPane.console->printf(48, 21, "| |       | |");
	drawPane.console->printf(48, 22, "| | |   | | |");

	drawPane.console->printf(48, 24, "E P S I L O N");

	drawPane.console->printf(48, 26, "| | |   | | |");
	drawPane.console->printf(48, 27, "| |       | |");
	drawPane.console->printf(48, 28, "|           |");
}

void SplashWindow::renderMenuOptions() const
{
	menu.render(drawPane, 50, 50);
}

void SplashWindow::render() const
{
	clearWindow();

	int slashWidth	= ((drawPane.consoleWidth / 2)	- (drawPane.consoleWidth % 2));
	int slashHeight = ((drawPane.consoleHeight / 2) - (drawPane.consoleHeight % 2));

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
						drawPane.console->setChar(1 + (x * 2), 1 + (y * 2), '|');
						drawPane.console->setCharForeground(1 + (x * 2), 1 + (y * 2), TCODColor::darkerGrey);
						break;
					case 1:
						drawPane.console->setChar(1 + (x * 2), 1 + (y * 2), '/');
						drawPane.console->setCharForeground(1 + (x * 2), 1 + (y * 2), TCODColor::darkerGrey);
						break;
					case 2:
						drawPane.console->setChar(1 + (x * 2), 1 + (y * 2), '-');
						drawPane.console->setCharForeground(1 + (x * 2), 1 + (y * 2), TCODColor::darkerGrey);
						break;
					case 3:
						drawPane.console->setChar(1 + (x * 2), 1 + (y * 2), '\\');
						drawPane.console->setCharForeground(1 + (x * 2), 1 + (y * 2), TCODColor::darkerGrey);
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
	: Window(consoleWidth, consoleHeight, "Inventory", rx, ry)
{
}

void InventoryFullWindow::update()
{ 
	if (inventoryItemList != WORLD->debugmap->player->inventory) inventoryItemList = WORLD->debugmap->player->inventory;
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
					drawPane.console->printf(0, drawLineStart, "|>%s", container->item->tool->name.c_str());
					drawPane.console->setCharForeground(1, drawLineStart, ep::color::selector);
				}
				else
				{
					drawPane.console->printf(0, drawLineStart, "| %s", container->item->tool->name.c_str());
				}
				++drawLine;
			}
			else
			{
				drawPane.console->printf(0, drawLine + i - 1, "|   .");
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
						drawPane.console->printf(0, drawLine, "|>  %s", item->tool->name.c_str());
						drawPane.console->setCharForeground(1, drawLine, ep::color::selector);
					}
					else
					{
						drawPane.console->printf(0, drawLine, "|   %s", item->tool->name.c_str());
					}
					++drawLine;
				}
				else
				{
					drawPane.console->printf(0, drawLine, "|     |");
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
	: message(message), messageLevel(messageLevel)
{
	if		(messageLevel == Message::MessageLevel::HIGH)	color = ep::color::messageHigh;
	else if (messageLevel == Message::MessageLevel::MEDIUM) color = ep::color::messageMed;
	else													color = ep::color::messageLow;
}

//----------------------------------------------------------------------------------------------------

LogWindow::LogWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	: Window(consoleWidth, consoleHeight, "Log", rx, ry)
{
}

void LogWindow::pushMessage(Message message)
{
	messageList.insert(messageList.begin(), message);
}

void LogWindow::update()
{
	while (messageList.size() > consoleHeight) messageList.erase(messageList.begin() + messageList.size() - 1);
}

void LogWindow::render() const
{
	clearWindow();

	int line = 0;
	for (int i = 0; i < messageList.size(); ++i)
	{
		if (i == 0) drawPane.console->printf(0, line, "|>%s", messageList[i].message.c_str());
		else		drawPane.console->printf(0, line, "| %s", messageList[i].message.c_str());

		for (int j = 0; j < drawPane.consoleWidth; ++j) //set string message color
		{
			if (i == 0 && j == 1)	drawPane.console->setCharForeground(j, line, ep::color::selector);
			else					drawPane.console->setCharForeground(j, line, messageList[i].color);
		}
		++line;
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

ProximityWindow::ProximityWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	: Window(consoleWidth, consoleHeight, "Proximity", rx, ry)
{
}

void ProximityWindow::update()
{
	if (proximityItemList != WORLD->debugmap->mapItemList) proximityItemList = WORLD->debugmap->mapItemList; //can be optimized later
	if (proximityContainerList != WORLD->debugmap->mapContainerList) proximityContainerList = WORLD->debugmap->mapContainerList;
}

void ProximityWindow::render() const
{
	clearWindow();

	int line = 0;

	for (auto& container : proximityContainerList)
	{
		if (container->item->mapPosition.z == WORLD->debugmap->player->mapPosition.z)
		{
			if (container->item->distToEnt < 5 && WORLD->isInPlayerFov(container->item->mapPosition))
			{
				drawPane.console->printf(0, line, "|%s", container->item->tool->name.c_str());
				++line;
			}
		}
	}

	for (auto& item : proximityItemList)
	{
		if (item->mapPosition.z == WORLD->debugmap->player->mapPosition.z)
		{
			if (item->distToEnt < 5 && WORLD->isInPlayerFov(item->mapPosition))
			{
				drawPane.console->printf(0, line, "|%s", item->tool->name.c_str());
				++line;
			}
		}
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

ActionWindow::ActionWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	: Window(consoleWidth, consoleHeight, "Actions", rx, ry)
{
}

void ActionWindow::update()
{
	if (WORLD->debugmap->player->selectedItem->actionManager)
	{
		if (actionManager != WORLD->debugmap->player->selectedItem->actionManager) actionManager = WORLD->debugmap->player->selectedItem->actionManager;
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
					drawPane.console->printf(0, line, "|>%s", action.name.c_str());
					drawPane.console->setCharForeground(1, line, ep::color::selector);
				}
				else
				{
					drawPane.console->printf(0, line, "| %s", action.name.c_str());
				}
				++line;
			}
		}
	}

	pushWindow();
}

//----------------------------------------------------------------------------------------------------

PauseWindow::PauseWindow(int consoleWidth, int consoleHeight, int rx, int ry)
	: Window(consoleWidth, consoleHeight, "Paused", rx, ry), 
	baseMenu(std::vector<std::string>{"Return", "Settings", "Exit"}), settingsMenu(std::vector<std::string>{"Input", "Video", "Back"}), 
	baseMenuActive(true), settingsMenuActive(false)
{
}

void PauseWindow::update()
{
	if		(baseMenuActive)		baseMenu.update();
	else if (settingsMenuActive)	settingsMenu.update();

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

	if		(baseMenuActive)		baseMenu.render(drawPane, 50, 50);
	else if (settingsMenuActive)	settingsMenu.render(drawPane, 50, 50);

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
		if (WORLD->debugmap->getBlock(Position3(INPUT->mouse.cx + WORLD->xOffset - 1, INPUT->mouse.cy + WORLD->yOffset - 3, WORLD->debugmap->player->mapPosition.z))->explored)
		{
			static Position3 position = Position3(INPUT->mouse.cx + WORLD->xOffset - 1, INPUT->mouse.cy + WORLD->yOffset - 3, WORLD->debugmap->player->mapPosition.z);
			tileDetail = WORLD->debugmap->getBlock(position)->getTileData(WORLD->debugmap->player->mapPosition.h).name;
		}
	}
}

void InfoWindow::setCreatureDetails()
{
	creatureDetail = "---";

	for (auto& creature : WORLD->debugmap->creatureList)
	{
		if (creature->mapPosition.z == WORLD->debugmap->player->mapPosition.z)
		{
			if (creature->mapPosition.x == INPUT->mouse.cx + WORLD->xOffset - 1 && creature->mapPosition.y == INPUT->mouse.cy + WORLD->yOffset - 3)
			{
				if (WORLD->isInPlayerFov(creature->mapPosition)) creatureDetail = creature->name;
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

	drawPane.console->printf(0, 0, "Tile : %s", tileDetail.c_str());
	drawPane.console->printf(0, 1, "Creature : %s", creatureDetail.c_str());
	drawPane.console->printf(0, 2, "Item : %s", itemDetail.c_str()); //will use later to show in depth description of held item

	pushWindow();
}
