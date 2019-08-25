#include "main.hpp"

//Gui Class
Gui::Gui(int windowX, int windowY)
{
	windowList.push_back(std::make_shared<GuiWindow>(10, 10, "Player", 64, 2));
	windowList.push_back(std::make_shared<GuiWindow>(44, 10, "Status", 75, 2));
	windowList.push_back(std::make_shared<GuiWindow>(25, 25, "Inventory", 64, 13));
	windowList.push_back(std::make_shared<GuiWindow>(14, 25, "Proximity", 90, 13));
	windowList.push_back(std::make_shared<GuiWindow>(14, 25, "Actions", 105, 13));
	windowList.push_back(std::make_shared<GuiWindow>(55, 24, "Event Log", 64, 39));

	guiMap = std::make_shared<GuiMap>(61, 61, 1, 2);
}

//Gui Render
void Gui::render()
{
	for (auto& window : windowList)
	{
		window->render();
	}

	guiMap->render();
}
