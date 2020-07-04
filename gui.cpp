#include "main.hpp"

Gui::Gui(int windowX, int windowY)
	: activeWindow(Gui::ActiveWindow::STARTUPSPLASH), activeLogWindow(Gui::ActiveLogWindow::LOG)
{
	windowList.push_back(worldWindow		= std::make_shared<MapWindow>		(60, 61, 1, 2));
	windowList.push_back(playerWindow		= std::make_shared<PlayerWindow>	(10, 10, 63, 2));
	windowList.push_back(statusWindow		= std::make_shared<StatusWindow>	(45, 10, 74, 2));
	windowList.push_back(inventoryWindow	= std::make_shared<InventoryWindow>	(25, 25, 63, 13));
	windowList.push_back(logWindow			= std::make_shared<LogWindow>		(56, 24, 63, 39));
	windowList.push_back(proximityWindow	= std::make_shared<ProximityWindow>	(30, 12, 89, 26));
	windowList.push_back(actionWindow		= std::make_shared<ActionWindow>	(30, 12, 89, 13));
	windowList.push_back(infoWindow			= std::make_shared<InfoWindow>		(56, 24, 63, 39));

	pauseWindow			=	std::make_shared<PauseWindow>			(118, 61, 1, 2);
	startupSplashWindow =	std::make_shared<SplashWindow>			(118, 61, 1, 2);
	inventoryFullWindow =	std::make_shared<InventoryFullWindow>	(55, 61, 64, 2);
}

void Gui::update()
{
	if (INPUT->inventory->bind->isSwitched) //move to input?
	{
		if (activeWindow == Gui::ActiveWindow::INVENTORYFULL)															activeWindow = Gui::ActiveWindow::NONE;
		else if (activeWindow != Gui::ActiveWindow::INVENTORYFULL && activeWindow != Gui::ActiveWindow::STARTUPSPLASH)	activeWindow = Gui::ActiveWindow::INVENTORYFULL;
	}

	switch (activeWindow)
	{
	case Gui::ActiveWindow::NONE:
		worldWindow->update();
		playerWindow->update();
		statusWindow->update();
		inventoryWindow->update();
		proximityWindow->update();
		actionWindow->update();
		
		if		(activeLogWindow == Gui::ActiveLogWindow::LOG)	logWindow->update();
		else if (activeLogWindow == Gui::ActiveLogWindow::INFO) infoWindow->update();
		break;

	case Gui::ActiveWindow::STARTUPSPLASH:
		startupSplashWindow->update();
		break;

	case Gui::ActiveWindow::INVENTORYFULL:
		worldWindow->update();
		inventoryFullWindow->update();
		break;

	case Gui::ActiveWindow::PAUSE:
		pauseWindow->update();
		break;
	}
}

void Gui::renderMouse() const
{
	if (activeWindow != Gui::ActiveWindow::STARTUPSPLASH)
	{
		if (INPUT->mouse->screenPosition.x >= 1 && INPUT->mouse->screenPosition.x <= 60 && INPUT->mouse->screenPosition.y >= 3 && INPUT->mouse->screenPosition.y <= 60)
		{		
			Position3 position = Position3(INPUT->mouse->mapPosition.x, INPUT->mouse->mapPosition.y, WORLD->debugmap->player->mapPosition.z);
			TCODColor color = worldWindow->world->debugmap->getBlock(position)->tileList[0].backgroundColor;
			TCODConsole::root->setCharBackground(INPUT->mouse->screenPosition.x, INPUT->mouse->screenPosition.y, color - TCODColor::darkestGrey);
		}
	}
	TCODConsole::root->setChar(INPUT->mouse->screenPosition.x, INPUT->mouse->screenPosition.y, '+');
}

void Gui::render() const
{
	switch (activeWindow)
	{
	case Gui::ActiveWindow::NONE:
		worldWindow->render();
		playerWindow->render();
		statusWindow->render();
		inventoryWindow->render();
		proximityWindow->render();
		actionWindow->render();

		if (activeLogWindow == Gui::ActiveLogWindow::LOG)		logWindow->render();
		else if (activeLogWindow == Gui::ActiveLogWindow::INFO) infoWindow->render();
		break;

	case Gui::ActiveWindow::STARTUPSPLASH:
		startupSplashWindow->render();
		break;

	case Gui::ActiveWindow::INVENTORYFULL:
		worldWindow->render();
		inventoryFullWindow->render();
		break;

	case Gui::ActiveWindow::PAUSE:
		pauseWindow->render();
		break;
	}

	renderMouse();
}
