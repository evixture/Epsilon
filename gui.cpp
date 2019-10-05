#include "main.hpp"

//Gui Class
Gui::Gui(int windowX, int windowY)
{
	windowList.push_back(playerPane = std::make_shared<PlayerPane>(10, 10, 64, 2));
	windowList.push_back(statusPane = std::make_shared<StatusPane>(44, 10, 75, 2));
	windowList.push_back(inventoryPane = std::make_shared<Window>(25, 25, "Inventory", 64, 13));
	windowList.push_back(proximityPane = std::make_shared<Window>(14, 25, "Proximity", 90, 13));
	windowList.push_back(actionsPane = std::make_shared<Window>(14, 25, "Actions", 105, 13));
	windowList.push_back(eventLogPane = std::make_shared<Window>(55, 24, "Event Log", 64, 39));
	windowList.push_back(mapPane = std::make_shared<MapPane>(61, 61, 1, 2));
}

void Gui::update()
{
	for (auto& window : windowList)
	{
		window->update();
	}
}

//Gui Render
void Gui::render() const
{
	for (auto& window : windowList)
	{
		window->render();
	}
}
