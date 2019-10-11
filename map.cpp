#include "main.hpp"

Map::Map(const char* filePath)
	:filePath(filePath)
{
	int currentFloor = 0;

	std::ifstream fileIn(filePath, std::ios::in);

	if (fileIn.is_open())
	{
		std::string s_mapName;
		std::string s_tempTile;
		
		fileIn >> s_mapName >> totalFloors >> mapW >> mapH;
		mapName = s_mapName.c_str();

		levelList = std::vector < std::vector < std::shared_ptr < Tile >>> (totalFloors);
		
		while (!fileIn.eof())
		{
			if (fileIn.good())
			{
				fileIn >> s_tempTile;

				if (levelList[currentFloor].size() == mapW * mapH)
				{
					currentFloor++;
				}

				switch (s_tempTile[0])
				{
				case '~':
					//map layer divider
					break;

				//FLOORS
				case 'f':
					switch (s_tempTile[1])
					{
					case '.':
						levelList[currentFloor].push_back(TILE_BasicGrass);
						break;
					case '_':
						levelList[currentFloor].push_back(TILE_BasicFloor);
						break;
					case '!':
						levelList[currentFloor].push_back(TILE_BasicConcrete);
						break;
					default:
						levelList[currentFloor].push_back(TILE_error);
						break;
					}
					break;

				//WALLS
				case 'w':
					switch (s_tempTile[1])
					{
					case '=':
						levelList[currentFloor].push_back(TILE_BasicWall);
						break;
					case 'O':
						levelList[currentFloor].push_back(TILE_BasicWindow);
						break;
					}
					break;

				//DOORS
				case 'd':
					switch (s_tempTile[1])
					{
					case '#':
						levelList[currentFloor].push_back(TILE_BasicDoor);
						break;
					}
					break;

				//SKIES
				case 's':
					switch (s_tempTile[1])
					{
					case '`':
						levelList[currentFloor].push_back(TILE_BasicSky);
						break;
					}
					break;

				//PART HEIGHT
				case 'p':
					switch (s_tempTile[1])
					{
					case 'n':
						levelList[currentFloor].push_back(TILE_BasicTable);
						break;
					}
					break;
				case 't':
					switch (s_tempTile[1])
					{
					case '/':
						levelList[currentFloor].push_back(TILE_UpStair);
						break;
					case '\\':
						levelList[currentFloor].push_back(TILE_DownStair);
						break;
					}
					break;

				//ERROR TILE
				default:
					levelList[currentFloor].push_back(TILE_error);
					break;
				}
			}
		}
	}
}

//World Class
World::World()
	:lookHeight(4)
{
	debugmap = std::make_shared<Map>("data/maps/debugmap.txt");
	//mapList.push_back(debugmap);

	player = std::make_shared<Player>(Position(2, 50), '@', "Player", TCODColor::azure);
	entityList.push_back(player);

	fovMap = std::make_shared<TCODMap>(debugmap->mapW, debugmap->mapH);
	//currentMap = mapList[player->level]; 
}

//Returns to tiles

std::shared_ptr<Tile> World::getTile(int x, int y, int level) const
{
	if (inMapBounds(x, y, level))
	{
		return debugmap->levelList[level][x + y * debugmap->mapW];
	}
	else
	{
		return TILE_error;
	}
}

bool World::isExplored(int x, int y, int level) const
{
	x += xOffset;
	y += yOffset;

	return debugmap->levelList[level][x + y * debugmap->mapW]->explored;
}

TCODColor World::getBgColor(int x, int y, int level) const
{
	return debugmap->levelList[level][x + y * debugmap->mapW]->bgcol;
}

TCODColor World::getFgColor(int x, int y, int level) const
{
	return debugmap->levelList[level][x + y * debugmap->mapW]->fgcol;
}

int World::getCh(int x, int y, int level) const
{
	return debugmap->levelList[level][x + y * debugmap->mapW]->ch;
}

int World::getHeight(int tx, int ty, int level) const
{
	return debugmap->levelList[level][tx + ty * debugmap->mapW]->height;
}

bool World::inMapBounds(int x, int y, int level) const
{
	if (level < 0 || level > debugmap->totalFloors - 1) return false;
	if (x < 0 || x > debugmap->mapW - 1) return false;
	if (y < 0 || y > debugmap->mapH - 1) return false;
	return true;
}

int World::getOffset(int playerx, int mapw, int renderw)
{
	if (playerx > renderw / 2 && playerx < mapw - renderw / 2)
	{
		return playerx - renderw / 2;
	}
	else if (playerx <= renderw / 2)
	{
		return 0;
	}
	else if (playerx >= mapw - renderw / 2)
	{
		return mapw - renderw;
	}
}

bool World::getWalkability(int x, int y, int level) const
{
	//x += xOffset;
	//y += yOffset;

	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= debugmap->mapW) return false;
	if (y >= debugmap->mapH) return false;

	return debugmap->levelList[level][x + y * debugmap->mapW]->walkable;
}

bool World::getTransparency(int x, int y, int level, int height) const
{
	//BUG HERE
	if (debugmap->levelList[level][x + y * debugmap->mapW])
	{
		if (height <= debugmap->levelList[level][x + y * debugmap->mapW]->height)
		{
			if (debugmap->levelList[level][x + y * debugmap->mapW]->tag == "destructible" && debugmap->levelList[level][x + y * debugmap->mapW]->getDestroyed())
			{
				return true;
			}

			return false;
		}
		else if (height > debugmap->levelList[level][x + y * debugmap->mapW]->height)
		{
			return true;
		}
	}
	else return false;
}

//check limits
void World::updateProperties()
{
	for (int y = 0; y < debugmap->mapH; y++)
	{
		for (int x = 0; x < debugmap->mapW; x++)
		{
			fovMap->setProperties(x, y, getTransparency(x, y, player->level, player->height), getWalkability(x, y, player->level));
		}
	}
}

void World::computeFov()
{
	fovMap->computeFov(player->mapPosition.x, player->mapPosition.y, engine->settings->fovRad, engine->settings->lightWalls, engine->settings->fovtype);
}

//check tcodmap fov
bool World::isInFov(int x, int y, int level) const
{
	x += xOffset;
	y += yOffset;

	if (x < 0 || x >= debugmap->mapW || y < 0 || y >= debugmap->mapH)
	{
		return false;
	}
	if (fovMap->isInFov(x, y))
	{
		debugmap->levelList[level][x + y * debugmap->mapW]->explored = true;
		return true;
	}
	return false;
}

//World update
void World::update()
{
	xOffset = getOffset(player->mapPosition.x, debugmap->mapW, engine->gui->mapPane->drawWindow->consoleW);
	yOffset = getOffset(player->mapPosition.y, debugmap->mapH, engine->gui->mapPane->drawWindow->consoleH);

	updateProperties();
	computeFov();

	for (auto& entity : entityList)
	{
		entity->update();
	}
}

//World Render
void World::render(const std::shared_ptr<Pane>& pane) const
{
	for (int y = yOffset; y < pane->consoleH + yOffset; y++)
	{
		for (int x = xOffset; x < pane->consoleW + xOffset; x++)
		{
			////this has better performance for debug
			//if (isInFov(x - xOffset, y - yOffset, player->level))
			//{
			//	pane->console->setCharBackground(x - xOffset, y - yOffset,getBgColor(x - xOffset, y - yOffset, player->level));
			//	pane->console->setCharForeground(x - xOffset, y - yOffset, getFgColor(x - xOffset, y - yOffset, player->level));
			//	pane->console->setChar(x - xOffset, y - yOffset, getCh(x - xOffset, y - yOffset, player->level));
			//}
			//else if (isExplored(x - xOffset, y, player->level))
			//{
			//	pane->console->setCharBackground(x - xOffset, y - yOffset, TCODColor::darkestGrey);
			//	pane->console->setCharForeground(x - xOffset, y - yOffset, TCODColor::darkerGrey);
			//	pane->console->setChar(x - xOffset, y - yOffset, getCh(x - xOffset, y - yOffset, player->level));
			//}
			//else
			//{
			//	pane->console->setCharBackground(x - xOffset, y - yOffset, TCODColor::black);
			//	pane->console->setCharForeground(x - xOffset, y - yOffset, TCODColor::darkerGrey);
			//}

			debugmap->levelList[player->level][x + y * debugmap->mapW]->render(x - xOffset, y - yOffset, pane);

			if (x + 1 == engine->settings->input->mouse.cx && y + 3 == engine->settings->input->mouse.cy)
			{
				pane->console->setCharBackground(x - 0, y - 0, TCODColor::white);
			}
		}
	}

	for (auto& entity : entityList)
	{
		if (entity->name == "Player")
		{
			player->render(player, pane);
		}
		else
		{
			entity->render(pane);
		}
	}
}
