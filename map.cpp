#include "main.hpp"

Map::Map(int x, int y)
	:mapW(x), mapH(y)
{
	player = std::make_shared<Entity>(Position(1, 1), '@', "Player", TCODColor::darkBlue);
	window = std::make_shared<Window>(x, y);
}