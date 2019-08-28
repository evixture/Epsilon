#include "main.hpp"

//Tile struct
Tile::Tile(int ch, TCODColor fgcol, TCODColor bgcol, int maxHeight, bool walkable, bool transparent, bool destructible)
	:ch(ch), fgcol(fgcol), bgcol(bgcol), maxHeight(maxHeight), walkable(walkable), transparent(transparent), destructible(destructible)
{
}

//Map Class
Map::Map(int w, int h)
	:mapW(w), mapH(h), lookHeight(4)
{
	player = std::make_shared<Entity>(Position(1, 1), '@', "Player", TCODColor::darkBlue);
	entityList.push_back(player);
}

void Map::createMap(const char* filePath)
{
	std::ifstream textFile;
	textFile.open(filePath);

	if (textFile.is_open())
	{
		while (!textFile.eof())
		{
			switch (textFile.get())
			{
			case '.':
				tileList.push_back(std::make_shared<Tile>(' ', TCODColor::pink, TCODColor::darkestGreen, 4, true, true, false));
				break;
			case 'W':
				tileList.push_back(std::make_shared<Tile>('#', TCODColor::lightSepia, TCODColor::sepia, 4, false, false, false));
				break;
			default:
				break;
			}
		}
	}
}

//Map Render
void Map::render(std::shared_ptr<Window> window)
{
	for (int y = 0; y < window->consoleH; y++)
	{
		for (int x = 0; x < window->consoleW; x++)
		{
			window->console->setCharBackground(x, y, tileList[x + y * window->consoleW]->bgcol);
			window->console->setCharForeground(x, y, tileList[x + y * window->consoleW]->fgcol);
			window->console->setChar(x, y, tileList[x + y * window->consoleW]->ch);
		}
	}


	for (auto& entity : entityList)
	{
		entity->render(window);

	}
}
