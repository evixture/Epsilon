#include "main.hpp"

//Level Strct
Level::Level(int fovmapW, int fovmapH)
	:tileList(), TCODMap(fovmapW, fovmapH)
{
	tileList.reserve(1);
}

//Map Struct
Map::Map(const char* cFilePath)
	:filePath(cFilePath), floorNum(0), mapW(60), mapH(61), mapName("error"), totalFloors(2)
{
	//levelList.reserve(1);
	//Level tempFloor;
	int cFloorNum = 0;

	std::ifstream fileIn(filePath, std::ios::in);

	if (fileIn.is_open())
	{
		std::string sMapName;
		std::string sTempTile;

		fileIn >> sMapName >> totalFloors >> mapW >> mapH;
		mapName = sMapName.c_str();

		Level tempFloor(mapW, mapH);
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
				case '=':
					//map layer divider
					break;
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
		fileIn.close();
	}
}

//World Class
World::World()
	:lookHeight(4)
{
	debugmap = std::make_shared<Map>("data/maps/debugmap.txt");
	mapList.push_back(debugmap);

	//fovMap = std::make_shared<TCODMap>(debugmap.mapW, debugmap.mapH);
	player = std::make_shared<Player>(Position(2, 50), '@', "Player", TCODColor::azure);
	entityList.push_back(player);

	currentMap = mapList[player->level];
}

void World::computeFov()
{
	currentMap->levelList[player->level].computeFov(player->position.x, player->position.y, engine->settings->fovRad, engine->settings->lightWalls, engine->settings->fovtype);
}

//Returns to tiles
bool World::isExplored(int x, int y, int level)
{
	return currentMap->levelList[level].tileList[x + y * currentMap->mapW]->explored;
}

TCODColor World::getBgColor(int x, int y, int level)
{
	return currentMap->levelList[level].tileList[x + y * currentMap->mapW]->bgcol;
}

TCODColor World::getFgColor(int x, int y, int level)
{
	return currentMap->levelList[level].tileList[x + y * currentMap->mapW]->fgcol;
}

int World::getCh(int x, int y, int level)
{
	return currentMap->levelList[level].tileList[x + y * currentMap->mapW]->ch;
}

bool World::getTransparency(int x, int y, int level)
{
	return currentMap->levelList[level].tileList[x + y * currentMap->mapW]->transparent;
}

bool World::getWalkability(int tx, int ty, int level)
{
	if (tx < 0) return false;
	if (ty < 0) return false;
	if (tx >= debugmap->mapW) return false;
	if (ty >= debugmap->mapH) return false;

	return currentMap->levelList[level].tileList[tx + ty * currentMap->mapW]->walkable;
}

//check limits
void World::updateProperties()
{
	for (int y = 0; y < debugmap->mapH; y++)
	{
		for (int x = 0; x < debugmap->mapW; x++)
		{
			//currentMap->levelList[player->level].setProperties(x, y, getTransparency(x, y, player->level), getWalkability(x, y, player->level));
		}
	}
}

//check tcodmap fov
bool World::isInFov(int x, int y, int level)
{
	if (x < 0 || x >= currentMap->mapW || y < 0 || y >= currentMap->mapH)
	{
		return false;
	}
	if (currentMap->levelList[player->level].isInFov(x, y))
	{
		currentMap->levelList[level].tileList[x + y * debugmap->mapW]->explored = true;
		return true;
	}
	return false;
}

//World update
void World::update(std::shared_ptr<Pane> window)
{
	//currentMap = mapList[player->level];
	////updateProperties();
	//computeFov();
}
//World Render
void World::render(std::shared_ptr<Pane> window)
{
	//for (int y = 0; y < window->consoleH; y++)
	//{
	//	for (int x = 0; x < window->consoleW; x++)
	//	{
	//		if (isInFov(x, y, player->level))
	//		{
	//			window->console->setCharBackground(x, y, getBgColor(x, y, player->level));
	//			window->console->setCharForeground(x, y, getFgColor(x, y, player->level));
	//			window->console->setChar(x, y, getCh(x, y, player->level));
	//		}
	//		else  // if (isExplored(x, y, player->level))
	//		{
	//			window->console->setCharBackground(x, y, TCODColor::darkestGrey);
	//			window->console->setCharForeground(x, y, TCODColor::darkerGrey);
	//			window->console->setChar(x, y, getCh(x, y, player->level));
	//		}
	//		/*else
	//		{
	//			window->console->setCharBackground(x, y, TCODColor::black);
	//			window->console->setCharForeground(x, y, TCODColor::darkerGrey);
	//		}*/
	//	}
	//}

	for (auto& entity : entityList)
	{
		entity->render(window);
	}
}
