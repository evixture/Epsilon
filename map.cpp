#include "main.hpp"

Map::Map(const char* filePath)
	:filePath(filePath)
{
	TCODRandom* RNG = TCODRandom::getInstance();
	RNG->setDistribution(TCOD_DISTRIBUTION_LINEAR);

	int currentFloor = 0;

	std::ifstream fileIn(filePath, std::ios::in);

	if (fileIn.is_open())
	{
		std::string s_mapName;
		std::string s_tempTile;
		
		fileIn >> s_mapName >> totalFloors >> mapWidth >> mapHeight;
		mapName = s_mapName.c_str();

		//reserve vec size beforehand to inc perf
		levelList = std::vector < std::vector < std::shared_ptr < Tile >>> (totalFloors);
		
		while (!fileIn.eof())
		{
			if (fileIn.good())
			{
				fileIn >> s_tempTile;

				if (levelList[currentFloor].size() == mapWidth * mapHeight)
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
					{
						int grassRand = RNG->getInt(0, 3);

						if (grassRand == 0)
						{
							levelList[currentFloor].push_back(TILE_BasicGrass0);
						}
						else if (grassRand == 1)
						{
							levelList[currentFloor].push_back(TILE_BasicGrass1);
						}
						else if (grassRand == 2)
						{
							levelList[currentFloor].push_back(TILE_BasicGrass2);
						}
						else if (grassRand == 3)
						{
							levelList[currentFloor].push_back(TILE_BasicGrass3);
						}
					}
						break;
					case '_':
						levelList[currentFloor].push_back(TILE_BasicFloor);
						break;
					case '!':
						levelList[currentFloor].push_back(TILE_BasicConcrete);
						break;
					case '*':
						levelList[currentFloor].push_back(TILE_BasicFlower);
						break;
					case '3':
						levelList[currentFloor].push_back(TILE_BasicShingle);
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
	:xOffset(0), yOffset(0)
{
	debugmap = std::make_shared<Map>("data/maps/debugmap.txt");

	entityList.push_back(player = std::make_shared<Player>(Position(2, 2, 0)));

	fovMap = std::make_shared<TCODMap>(debugmap->mapWidth, debugmap->mapHeight);

	mapItemList.push_back(TESTITEM);
}

std::shared_ptr<Tile> World::getTile(int x, int y, int level) const
{
	if (inMapBounds(x, y, level))
	{
		return debugmap->levelList[level][x + y * debugmap->mapWidth];
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

	return debugmap->levelList[level][x + y * debugmap->mapWidth]->explored;
}

TCODColor World::getBgColor(int x, int y, int level) const
{
	return debugmap->levelList[level][x + y * debugmap->mapWidth]->backgroundColor;
}

bool World::inMapBounds(int x, int y, int level) const
{
	if (level < 0 || level > debugmap->totalFloors - 1) return false;
	if (x < 0 || x > debugmap->mapWidth - 1) return false;
	if (y < 0 || y > debugmap->mapHeight - 1) return false;
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
	else
	{
		return 0;
	}
}

bool World::getWalkability(int x, int y, int level) const
{
	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= debugmap->mapWidth) return false;
	if (y >= debugmap->mapHeight) return false;

	return debugmap->levelList[level][x + y * debugmap->mapWidth]->walkable;
}

bool World::getTransparency(int x, int y, int level, int height) const
{
	if (debugmap->levelList[level][x + y * debugmap->mapWidth])
	{
		if (height <= debugmap->levelList[level][x + y * debugmap->mapWidth]->height)
		{
			if (debugmap->levelList[level][x + y * debugmap->mapWidth]->tag == "destructible" && debugmap->levelList[level][x + y * debugmap->mapWidth]->getDestroyed())
			{
				return true;
			}
			return false;
		}
		else if (height > debugmap->levelList[level][x + y * debugmap->mapWidth]->height)
		{
			return true;
		}
	}
	return false;
}

void World::updateProperties()
{
	for (int y = 0; y < debugmap->mapHeight; y++)
	{
		for (int x = 0; x < debugmap->mapWidth; x++)
		{
			fovMap->setProperties(x, y, getTransparency(x, y, player->mapPosition.level, player->height), getWalkability(x, y, player->mapPosition.level));
		}
	}
}

void World::computeFov()
{
	fovMap->computeFov(player->mapPosition.x, player->mapPosition.y, engine->settings->fovRad, engine->settings->lightWalls, engine->settings->fovtype);
}

bool World::isInFov(int x, int y, int level) const
{
	x += xOffset;
	y += yOffset;

	if (x < 0 || x >= debugmap->mapWidth || y < 0 || y >= debugmap->mapHeight)
	{
		return false;
	}
	if (fovMap->isInFov(x, y))
	{
		debugmap->levelList[level][x + y * debugmap->mapWidth]->explored = true;
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------

void World::updateEntities()
{
	for (auto& entity : entityList)
	{
		entity->update();
	}
}

void World::update()
{
	xOffset = getOffset(player->mapPosition.x, debugmap->mapWidth, MAPPANE->drawWindow->consoleW);
	yOffset = getOffset(player->mapPosition.y, debugmap->mapHeight, MAPPANE->drawWindow->consoleH);

	updateProperties();
	computeFov();
	updateEntities();

	for (auto& item : mapItemList)
	{
		//item->updateTile();
	}
}

//--------------------------------------------------------------------------------------------

void World::renderTiles(const std::shared_ptr<Pane>& pane) const
{
	for (int y = yOffset; y < pane->consoleH + yOffset; y++)
	{
		for (int x = xOffset; x < pane->consoleW + xOffset; x++)
		{
			getTile(x, y, player->mapPosition.level)->render(x - xOffset, y - yOffset, pane);
		}
	}
}

void World::renderMouse(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setCharBackground(engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3, getTile(engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3, player->mapPosition.level)->backgroundColor - TCODColor::darkestGrey);
	pane->console->setChar(engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3, '+');
}

void World::renderEntities(const std::shared_ptr<Pane>& pane) const
{
	for (auto& entity : entityList)
	{
		if (entity->name == "Player")
		{
			player->render(pane);
		}
		else
		{
			entity->render(pane);
		}
	}
}

void World::render(const std::shared_ptr<Pane>& pane) const
{
	renderTiles(pane);

	for (auto& item : mapItemList)
	{
		item->renderTile(pane);
	}

	renderMouse(pane);
	renderEntities(pane);
}

