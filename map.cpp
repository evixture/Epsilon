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
bool World::isExplored(int x, int y, int level)
{
	return debugmap->levelList[level][x + y * debugmap->mapW]->explored;
}

TCODColor World::getBgColor(int x, int y, int level)
{
	return debugmap->levelList[level][x + y * debugmap->mapW]->bgcol;
}

TCODColor World::getFgColor(int x, int y, int level)
{
	return debugmap->levelList[level][x + y * debugmap->mapW]->fgcol;
}

int World::getCh(int x, int y, int level)
{
	return debugmap->levelList[level][x + y * debugmap->mapW]->ch;
}

int World::getHeight(int tx, int ty, int level)
{
	return debugmap->levelList[level][tx + ty * debugmap->mapW]->height;
}

bool World::getWalkability(int tx, int ty, int level)
{
	if (tx < 0) return false;
	if (ty < 0) return false;
	if (tx >= debugmap->mapW) return false;
	if (ty >= debugmap->mapH) return false;

	return debugmap->levelList[level][tx + ty * debugmap->mapW]->walkable;
}

bool World::getTransparency(int x, int y, int level, int height)
{
	if (height <= debugmap->levelList[level][x + y * debugmap->mapW]->height)
	{
		return false;
	}
	else
	{
		return debugmap->levelList[level][x + y * debugmap->mapW]->transparent;
	}
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
	//debugmap->levelList[player->level].computeFov(player->position.x, player->position.y, engine->settings->fovRad, engine->settings->lightWalls, engine->settings->fovtype);
	fovMap->computeFov(player->position.x, player->position.y, engine->settings->fovRad, engine->settings->lightWalls, engine->settings->fovtype);
}

//check tcodmap fov
bool World::isInFov(int x, int y, int level)
{
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
void World::update(std::shared_ptr<Pane> window)
{
	//currentMap = mapList[player->level];
	updateProperties();
	computeFov();

	for (auto& entity : entityList)
	{
		entity->update();
	}
}

//World Render
void World::render(std::shared_ptr<Pane> pane) 
{
	for (int y = 0; y < pane->consoleH; y++)
	{
		for (int x = 0; x < pane->consoleW; x++)
		{
			//this has better performance for debug
			if (isInFov(x, y, player->level))
			{
				pane->console->setCharBackground(x, y,getBgColor(x, y, player->level));
				pane->console->setCharForeground(x, y, getFgColor(x, y, player->level));
				pane->console->setChar(x, y, getCh(x, y, player->level));
			}
			else if (isExplored(x, y, player->level))
			{
				pane->console->setCharBackground(x, y, TCODColor::darkestGrey);
				pane->console->setCharForeground(x, y, TCODColor::darkerGrey);
				pane->console->setChar(x, y, getCh(x, y, player->level));
			}
			else
			{
				pane->console->setCharBackground(x, y, TCODColor::black);
				pane->console->setCharForeground(x, y, TCODColor::darkerGrey);
			}

			//serverely limits fps for some reason
			//debugmap->levelList[player->level][x + y * debugmap->mapW]->render(x, y, pane);

			if (x + 1 == engine->settings->input->mouse.cx && y + 3 == engine->settings->input->mouse.cy)
			{
				pane->console->setCharBackground(x, y, TCODColor::white);
			}
		}
	}

	for (auto& entity : entityList)
	{
		if (entity->name == "Player")
		{
			player->render(player, pane);
		}
		entity->render(pane);
	}

	//need to move to input
		//if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		//{
		//	//leftMouseClick = true;
		//	engine->gui->mapPane->world->player->testWeapon->weaponFire(engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3, engine->gui->mapPane->drawWindow);
		//}
		//else
		//{
		//	TCODLine::init(player->testWeapon->wx, player->testWeapon->wy, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3);
		//	//leftMouseClick = false;
		//}

}
