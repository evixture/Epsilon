#include "main.hpp"

//Level Strct
Level::Level()
	:tileList()
{
	tileList.reserve(1);
}

//Map Struct
Map::Map(const char* cFilePath)
	:filePath(cFilePath), TCODMap(mapW, mapH), floorNum(0)
{
	//levelList.reserve(1);
	Level tempFloor;
	int cFloorNum = 0;

	std::ifstream fileIn(filePath, std::ios::in);

	std::string sMapName;
	std::string sTempTile;

	if (fileIn.is_open())
	{
		fileIn >> sMapName >> totalFloors >> mapW >> mapH;
		mapName = sMapName.c_str();

		std::cout << mapName << totalFloors << mapW << mapH;

		while (!fileIn.eof())
		{
			if (fileIn.good())
			{
				fileIn >> sTempTile;
			
				if (tempFloor.tileList.size() % (mapW * mapH) == 0 && tempFloor.tileList.size() != 0)
				{
					levelList.push_back(tempFloor);
					tempFloor.tileList.clear();
					cFloorNum++;
				}

				switch (sTempTile[0])
				{
				case '.':
					tempFloor.tileList.push_back(TILE_grass);
					break;
				case '#':
					tempFloor.tileList.push_back(TILE_wall);
					break;
				case '_':
					tempFloor.tileList.push_back(TILE_floor);
					break;
				default:
					tempFloor.tileList.push_back(TILE_error);
					break;
				}
			}
		}
	}
	//TCODMap(mapW, mapH);
}

//World Class
World::World()
	:lookHeight(4)//, debugmap("data/maps/debugmap.txt")
{
	debugmap = std::make_shared<Map>("data/maps/debugmap.txt");

	//fovMap = std::make_shared<TCODMap>(debugmap.mapW, debugmap.mapH);
	player = std::make_shared<Player>(Position(2, 50), '@', "Player", TCODColor::azure);
	entityList.push_back(player);
}

//void World::createMap(Map mapFile)
//{
//	std::ifstream textFile(mapFile.filePath, std::ios::binary);
//	//textFile.open(filePath, std::ios::binary);
//
//	//int fileLength = getMapFileLenght(filePath);
//
//	//std::cout << fileLength << std::endl;
//
//	//textFile.seekg(0, std::ios::beg);
//
//	if (textFile.is_open())
//	{
//		while (!textFile.eof())
//		{
//			switch (textFile.get())
//			{
//			case '.':
//				tileList.push_back(TILE_grass);
//				break;
//			case '#':
//				tileList.push_back(TILE_wall);
//				break;
//			case '_':
//				tileList.push_back(TILE_floor);
//				break;
//			default:
//				break;
//			}
//		}
//		textFile.close();
//	}
//}

//TcodMap compute Fov
void World::computeFov()
{
	debugmap->computeFov(player->position.x, player->position.y, engine.settings->fovRad, engine.settings->lightWalls, engine.settings->fovtype);
}

//Returns to tiles
bool World::isExplored(int x, int y, int level) 
{
	return debugmap->levelList[level].tileList[x + y * debugmap->mapW]->explored;
}

TCODColor World::getBgColor(int x, int y, int level) 
{
	return debugmap->levelList[level].tileList[x + y * debugmap->mapW]->bgcol;
}

TCODColor World::getFgColor(int x, int y, int level) 
{
	return debugmap->levelList[level].tileList[x + y * debugmap->mapW]->fgcol;
}

int World::getCh(int x, int y, int level)
{
	return debugmap->levelList[level].tileList[x + y * debugmap->mapW]->ch;
}

bool World::getTransparency(int x, int y, int level)
{
	return debugmap->levelList[level].tileList[x + y * debugmap->mapW]->transparent;
}

bool World::getWalkability(int tx, int ty, int level)
{
	if (tx < 0) return false;
	if (ty < 0) return false;
	if (tx >= debugmap->mapW) return false;
	if (ty >= debugmap->mapH) return false;

	return debugmap->levelList[level].tileList[tx + ty * debugmap->mapW]->walkable;
}

//check limits
void World::updateProperties(std::shared_ptr<Pane> window)
{
	for (int y = 0; y < window->consoleH; y++)
	{
		for (int x = 0; x < window->consoleW; x++)
		{
			debugmap->setProperties(x, y, getTransparency(x, y, player->level), getWalkability(x, y, player->level));
		}
	}
}

//check tcodmap fov
bool World::isInFov(int x, int y, int level)
{
	if (x < 0 || x >= debugmap->mapW || y < 0 || y >= debugmap->mapH)
	{
		return false;
	}
	if (debugmap->isInFov(x, y))
	{
		debugmap->levelList[level].tileList[x + y * debugmap->mapW]->explored = true;
		return true;
	}
	return false;
}

//World update
void World::update(std::shared_ptr<Pane> window)
{
	updateProperties(window);
	computeFov();
}
//World Render
void World::render(std::shared_ptr<Pane> window)
{
	for (int y = 0; y < window->consoleH; y++)
	{
		for (int x = 0; x < window->consoleW; x++)
		{
			if (isInFov(x, y, player->level))
			{
				window->console->setCharBackground(x, y, getBgColor(x, y, player->level));
				window->console->setCharForeground(x, y, getFgColor(x, y, player->level));
				window->console->setChar(x, y, getCh(x, y, player->level));
			}
			else  // if (isExplored(x, y, player->level))
			{
				window->console->setCharBackground(x, y, TCODColor::darkestGrey);
				window->console->setCharForeground(x, y, TCODColor::darkerGrey);
				window->console->setChar(x, y, getCh(x, y, player->level));
			}
			/*else
			{
				window->console->setCharBackground(x, y, TCODColor::black);
				window->console->setCharForeground(x, y, TCODColor::darkerGrey);
			}*/
		}
	}

	for (auto& entity : entityList)
	{
		entity->render(window);
	}
}
