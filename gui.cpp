#include "main.hpp"

//Gui Class
Gui::Gui(int windowX, int windowY)
{
	windowList.push_back(playerPane = std::make_shared<PlayerPane>(10, 10, 64, 2));
	windowList.push_back(statusPane = std::make_shared<StatusPane>(44, 10, 75, 2));
	windowList.push_back(inventoryPane = std::make_shared<InventoryPane>(25, 25, 64, 13));
	windowList.push_back(proximityPane = std::make_shared<Window>(14, 25, "Proximity", 90, 13));
	windowList.push_back(actionsPane = std::make_shared<Window>(14, 25, "Actions", 105, 13));
	windowList.push_back(eventLogPane = std::make_shared<Window>(55, 24, "Event Log", 64, 39));
	windowList.push_back(mapPane = std::make_shared<MapPane>(61, 61, 1, 2));

	startupSplash = std::make_shared<Window>(118, 62, "EPSILON", 1, 1);
}

void Gui::update()
{
	if (engine->gamestate == Engine::MAIN)
	{
		for (auto& window : windowList)
		{
			window->update();
		}
	}
	else if (engine->gamestate == Engine::STARTUPSPLASH)
	{
		startupSplash->update();
	}
}

void Gui::render() const
{
	if (engine->gamestate == Engine::MAIN)
	{
		for (auto& window : windowList)
		{
			window->render();
		}
	}
	else if (engine->gamestate == Engine::STARTUPSPLASH)
	{
		startupSplash->render();
	}
}
