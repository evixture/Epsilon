#include "main.hpp"

Map::Map(int x, int y)
	:mapW(x), mapH(y)
{
	player = std::make_shared<Entity>(Position(1, 1), '@', "Player", TCODColor::darkBlue);
	entityList.push_back(player);
}

void Map::render(std::shared_ptr<GuiWindow> window)
{
	for (auto& entity : entityList)
	{
		entity->render(window->drawWindow);
	}
}