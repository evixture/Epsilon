#include "main.hpp"

//MapFile Struct
int MapFile::getMapTextLength()
{
	//ERROR HERE WHEN OPENING FILES
	std::ifstream textFile(filePath, std::ios::binary);

	int fileLength = 0;

	if (textFile.is_open())
	{
		while (!textFile.eof())
		{
			textFile.get();
			fileLength++;
		}
	}
	textFile.close();
	return fileLength;
}

MapFile::MapFile(const char* filepath, int mapWidth, int mapHeight)
	:filePath(filepath), mapW(mapWidth), mapH(mapHeight)
{
	textLength = getMapTextLength();
	std::cout << textLength << std::endl;
}

//Map Class
Map::Map()
	:lookHeight(4), debugmap(MapFile("data/maps/debugmap.txt", 61, 60))
{
	fovMap = std::make_shared<TCODMap>(debugmap.mapW, debugmap.mapH);
	player = std::make_shared<Player>(Position(2, 50), '@', "Player", TCODColor::azure);
	entityList.push_back(player);
}

void Map::createMap(MapFile mapFile)
{
	std::ifstream textFile(mapFile.filePath, std::ios::binary);
	//textFile.open(filePath, std::ios::binary);

	//int fileLength = getMapFileLenght(filePath);

	//std::cout << fileLength << std::endl;

	//textFile.seekg(0, std::ios::beg);

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
	fovMap->computeFov(player->position.x, player->position.y, engine.settings->fovRad, engine.settings->lightWalls, engine.settings->fovtype);
}

//Returns to tiles
bool Map::isExplored(int x, int y)
{
	return tileList[x + y * debugmap.mapW]->explored;
}

TCODColor Map::getBgColor(int x, int y)
{
	return tileList[x + y * debugmap.mapW]->bgcol;
}

TCODColor Map::getFgColor(int x, int y)
{
	return tileList[x + y * debugmap.mapW]->fgcol;
}

int Map::getCh(int x, int y)
{
	return tileList[x + y * debugmap.mapW]->ch;
}

bool Map::getTransparency(int x, int y)
{
	return tileList[x + y * debugmap.mapW]->transparent;
}

bool Map::getWalkability(int tx, int ty)
{
	if (tx < 0) return false;
	if (ty < 0) return false;
	if (tx >= debugmap.mapW) return false;
	if (ty >= debugmap.mapH) return false;

	return tileList[tx + ty * debugmap.mapW]->walkable;
}

//check limits
void Map::updateProperties(std::shared_ptr<Pane> window)
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
	if (x < 0 || x >= debugmap.mapW || y < 0 || y >= debugmap.mapH)
	{
		return false;
	}
	if (fovMap->isInFov(x, y))
	{
		tileList[x + y * debugmap.mapW]->explored = true;
		return true;
	}
	return false;
}

//Map update
void Map::update(std::shared_ptr<Pane> window)
{
	updateProperties(window);
	computeFov();
}
//Map Render
void Map::render(std::shared_ptr<Pane> window)
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
