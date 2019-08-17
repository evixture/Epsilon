#include "main.hpp"

Map::Map(int x, int y)
	:mapW(x), mapH(y)
{
	player = std::make_shared<Entity>(Position(1, 1), '@', "Player", TCODColor::darkBlue);
	entityList.push_back(player);
	window = std::make_shared<Window>(x, y);
}

void Map::render()
{
	for (auto& entity : entityList)
	{
		entity->render(window);
	}
}