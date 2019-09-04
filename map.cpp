#include "main.hpp"

//============TILES=======================CH===FOREGROUND=COLOR======BACKGROUND=COLOR====HEIGHT==WALK==TRANS==DESTR==
#define TILE_grass std::make_shared<Tile>('.', TCODColor::darkerGreen, TCODColor::darkestGreen, 4, true, true, false)
#define TILE_wall  std::make_shared<Tile>(' ', TCODColor::lightestSepia, TCODColor::lighterSepia, 4, false, false, false)
#define TILE_floor std::make_shared<Tile>('#', TCODColor::darkSepia, TCODColor::darkestSepia, 4, true, true, false)
//============TILES==================================================================================================

//Tile struct
Tile::Tile(int ch, TCODColor fgcol, TCODColor bgcol, int maxHeight, bool walkable, bool transparent, bool destructible)
	:ch(ch), fgcol(fgcol), bgcol(bgcol), maxHeight(maxHeight), walkable(walkable), transparent(transparent), destructible(destructible), explored(false)
{
}

//Map Class
Map::Map(int w, int h)
	:mapW(w), mapH(h), lookHeight(4)
{
	fovMap = std::make_shared<TCODMap>(w, h);
	player = std::make_shared<Player>(Position(6, 7), '@', "Player", TCODColor::darkBlue);
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
				tileList.push_back(TILE_grass);
				break;
			case '#':
				tileList.push_back(TILE_wall);
				break;
			case '_':
				tileList.push_back(TILE_floor);
				break;
			default:
				break;
			}
		}
		textFile.close();
	}
}

//TcodMap compute Fov
void Map::computeFov()
{
	fovMap->computeFov(player->position.x, player->position.y, 0, engine.settings->fovtype);
}

//Returns to tiles
bool Map::isExplored(int x, int y)
{
	return tileList[x + y * mapW]->explored;
}

TCODColor Map::getBgColor(int x, int y)
{
	return tileList[x + y * mapW]->bgcol;
}

TCODColor Map::getFgColor(int x, int y)
{
	return tileList[x + y * mapW]->fgcol;
}

int Map::getCh(int x, int y)
{
	return tileList[x + y * mapW]->ch;
}

bool Map::getTransparency(int x, int y)
{
	return tileList[x + y * mapW]->transparent;
}

bool Map::getWalkability(int tx, int ty)
{
	return tileList[tx + ty * mapW]->walkable;
}

void Map::updateProperties(std::shared_ptr<Window> window)
{
	for (int y = 0; y < window->consoleH; y++)
	{
		for (int x = 0; x < window->consoleW; x++)
		{
			fovMap->setProperties(x, y, getTransparency(x, y), getWalkability(x, y));
		}
	}
}

//check tcodmap fov
bool Map::isInFov(int x, int y)
{
	if (x < 0 || x >= mapW || y < 0 || y >= mapH)
	{
		return false;
	}
	if (fovMap->isInFov(x, y))
	{
		tileList[x + y * mapW]->explored = true;
		return true;
	}
	return false;
}

//Map update
void Map::update(std::shared_ptr<Window> window)
{
	updateProperties(window);
	computeFov();
}
//Map Render
void Map::render(std::shared_ptr<Window> window)
{
	for (int y = 0; y < window->consoleH; y++)
	{
		for (int x = 0; x < window->consoleW; x++)
		{
			if (isInFov(x, y))
			{
				window->console->setCharBackground(x, y, getBgColor(x, y));
				window->console->setCharForeground(x, y, getFgColor(x, y));
				window->console->setChar(x, y, getCh(x, y));
			}
			else if (isExplored(x, y))
			{
				window->console->setCharBackground(x, y, TCODColor::darkestGrey);
				window->console->setCharForeground(x, y, TCODColor::darkerGrey);
				window->console->setChar(x, y, getCh(x, y));
			}
			else
			{
				window->console->setCharBackground(x, y, TCODColor::black);
				window->console->setCharForeground(x, y, TCODColor::darkerGrey);
			}
		}
	}


	for (auto& entity : entityList)
	{
		entity->render(window);
	}
}
