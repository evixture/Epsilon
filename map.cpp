#include "main.hpp"

//============TILES=======================CH===FOREGROUND=COLOR======BACKGROUND=COLOR====HEIGHT==WALK==TRANS==DESTR==
#define TILE_grass std::make_shared<Tile>('v', TCODColor::darkerGreen, TCODColor::darkestGreen, 4, true, true)
#define TILE_wall  std::make_shared<Tile>('#', TCODColor::lightSepia, TCODColor::lighterSepia, 4, false, false)
#define TILE_floor std::make_shared<Tile>(' ', TCODColor::darkSepia, TCODColor::darkerSepia, 4, true, true)
//============TILES==================================================================================================

//MapFile Struct
int MapFile::getMapTextLenght(const char* filepath)
{
	std::ifstream textFile(filepath);
	//textFile.open(filePath, std::ios::binary);

	int fileLength = 0;

	if (textFile.is_open())
	{
		while (!textFile.eof())
		{
			textFile.get();
			fileLength++;
		}
	}
	return fileLength;
}

//MapFile::MapFile()
//{
//	return;
//}

MapFile::MapFile(const char* filePath)
	:filePath(filePath)
{
	textLength = getMapTextLenght(filePath);
	std::cout << textLength << std::endl;
}



//Map Class
Map::Map(int w, int h)
	:mapW(w), mapH(h), lookHeight(4)
{
	

	fovMap = std::make_shared<TCODMap>(w, h);
	player = std::make_shared<Player>(Position(2, 2), '@', "Player", TCODColor::azure);
	entityList.push_back(player);
}




void Map::createMap(MapFile mapFile)
{
	std::ifstream textFile(mapFile.filePath);
	//textFile.open(filePath, std::ios::binary);

	//int fileLength = getMapFileLenght(filePath);

	//std::cout << fileLength << std::endl;

	textFile.seekg(0, std::ios::beg);

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
