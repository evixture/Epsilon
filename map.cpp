#include "main.hpp"

//Map Class
Map::Map(int x, int y)
	:mapW(x), mapH(y)
{
	player = std::make_shared<Entity>(Position(1, 1), '@', "Player", TCODColor::darkBlue);
	entityList.push_back(player);
}

//Map Render
void Map::render(std::shared_ptr<GuiWindow> window)
{
	//window->drawWindow->console->clear();
	for (auto& entity : entityList)
	{
		entity->render(window->drawWindow);
	}
}
