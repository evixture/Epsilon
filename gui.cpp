#include "main.hpp"

Gui::Gui(int windowX, int windowY)
	:activeWindow(Gui::ActiveWindow::STARTUPSPLASH)
{
	windowList.push_back(worldWindow		= std::make_shared<MapWindow>		(61, 61, 1, 2));
	windowList.push_back(playerWindow		= std::make_shared<PlayerWindow>	(10, 10, 64, 2));
	windowList.push_back(statusWindow		= std::make_shared<StatusWindow>	(44, 10, 75, 2));
	windowList.push_back(inventoryWindow	= std::make_shared<InventoryWindow>	(25, 25, 64, 13));
	windowList.push_back(logWindow			= std::make_shared<LogWindow>		(55, 24, 64, 39));
	windowList.push_back(proximityWindow	= std::make_shared<ProximityWindow>	(14, 25, 105, 13));
	windowList.push_back(actionWindow		= std::make_shared<ActionWindow>	(14, 25, 90, 13));

	startupSplashWindow = std::make_shared<SplashWindow>		(118, 62, 1, 1);
	inventoryFullWindow = std::make_shared<InventoryFullWindow>	(55, 61, 64, 2);
}

void Gui::update()
{
	if (INPUT->i->isSwitched)
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
}
