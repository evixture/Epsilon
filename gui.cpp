#include "main.hpp"

Gui::Gui(int windowX, int windowY)
	:activeWindow(Gui::ActiveWindow::STARTUPSPLASH)
{
	windowList.push_back(worldWindow		= std::make_shared<MapWindow>		(60, 61, 1, 2));
	windowList.push_back(playerWindow		= std::make_shared<PlayerWindow>	(10, 10, 63, 2));
	windowList.push_back(statusWindow		= std::make_shared<StatusWindow>	(45, 10, 74, 2));
	windowList.push_back(inventoryWindow	= std::make_shared<InventoryWindow>	(25, 25, 63, 13));
	windowList.push_back(logWindow			= std::make_shared<LogWindow>		(56, 24, 63, 39));
	windowList.push_back(proximityWindow	= std::make_shared<ProximityWindow>	(30, 12, 89, 26));
	windowList.push_back(actionWindow		= std::make_shared<ActionWindow>	(30, 12, 89, 13));

	pauseWindow =			std::make_shared<PauseWindow>			(118, 61, 1, 2);
	startupSplashWindow =	std::make_shared<SplashWindow>			(118, 61, 1, 2);
	inventoryFullWindow =	std::make_shared<InventoryFullWindow>	(55, 61, 64, 2);
}


void Gui::update()
{
	if (INPUT->inventoryKey->isSwitched)
	{
		if (activeWindow == Gui::ActiveWindow::INVENTORYFULL)
		{
			activeWindow = Gui::ActiveWindow::NONE;
		}
		else if (activeWindow != Gui::ActiveWindow::INVENTORYFULL && activeWindow != Gui::ActiveWindow::STARTUPSPLASH)
		{
			activeWindow = Gui::ActiveWindow::INVENTORYFULL;
		}
	}

	if (activeWindow == Gui::ActiveWindow::NONE)
	{
		for (auto& window : windowList)
		{
			window->update();
		}
	}
	else if (activeWindow == Gui::ActiveWindow::STARTUPSPLASH)
	{
		startupSplashWindow->update();
	}
	else if (activeWindow == Gui::ActiveWindow::INVENTORYFULL)
	{
		worldWindow->update();
		inventoryFullWindow->update();
	}
	else if (activeWindow == Gui::ActiveWindow::PAUSE)
	{
		pauseWindow->update();
	}
}

void Gui::renderMouse() const
{
	if (activeWindow != Gui::ActiveWindow::STARTUPSPLASH)
	{
		if (INPUT->mouse.cx >= 1 && INPUT->mouse.cx <= 60)
		{
			if (INPUT->mouse.cy >= 3 && INPUT->mouse.cy <= 60)
			TCODConsole::root->setCharBackground(INPUT->mouse.cx, INPUT->mouse.cy, 
				worldWindow->world->debugmap->getBlock(Position3(INPUT->mouse.cx - 1 + worldWindow->world->xOffset, INPUT->mouse.cy - 3 + WORLD->yOffset, WORLD->debugmap->player->mapPosition.floor))
				->tileList[0]->backgroundColor - TCODColor::darkestGrey);  //get bg color of the tile
		}
	}

	TCODConsole::root->setChar(INPUT->mouse.cx, INPUT->mouse.cy, '+');
}

void Gui::render() const
{
	if (activeWindow == Gui::ActiveWindow::NONE)
	{
		for (auto& window : windowList)
		{
			window->render();
		}
	}
	else if (activeWindow == Gui::ActiveWindow::STARTUPSPLASH)
	{
		startupSplashWindow->render();
	}
	else if (activeWindow == Gui::ActiveWindow::INVENTORYFULL)
	{
		worldWindow->render();
		inventoryFullWindow->render();
	}
	else if (activeWindow == Gui::ActiveWindow::PAUSE)
	{
		pauseWindow->render();
	}

	renderMouse();
}
