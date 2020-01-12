#include "main.hpp"

Map::Map(std::string filePath)
	:filePath(filePath), height(NULL), width(NULL), totalFloors(NULL)
{
	TCODRandom* RNG = TCODRandom::getInstance();
	RNG->setDistribution(TCOD_DISTRIBUTION_LINEAR);

	int currentFloor = 0;

	std::ifstream fileIn(filePath, std::ios::in);

	if (fileIn.is_open())
	{
		std::string s_mapName;
		std::string s_tempTile;
		
		fileIn >> s_mapName >> totalFloors >> width >> height;
		name = s_mapName.c_str();

		//reserve vec size beforehand to inc perf?
		levelList = std::vector < std::vector < std::shared_ptr < Tile >>> (totalFloors);

		for (auto& level : levelList)
		{
			level.reserve(width * height);
		}
		
		while (!fileIn.eof())
		{
			if (fileIn.good())
			{
				fileIn >> s_tempTile;

				if (levelList[currentFloor].size() == width * height)
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
							levelList[currentFloor].push_back(TILE_Grass0);
						}
						else if (grassRand == 1)
						{
							levelList[currentFloor].push_back(TILE_Grass1);
						}
						else if (grassRand == 2)
						{
							levelList[currentFloor].push_back(TILE_Grass2);
						}
						else if (grassRand == 3)
						{
							levelList[currentFloor].push_back(TILE_Grass3);
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
						levelList[currentFloor].push_back(DESTRUCTIBLE_BasicWall);
						break;
					case 'O':
						levelList[currentFloor].push_back(DESTRUCTIBLE_BasicWindow);
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
						levelList[currentFloor].push_back(STAIR_UpStair);
						break;
					case '\\':
						levelList[currentFloor].push_back(STAIR_DownStair);
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

//----------------------------------------------------------------------------------------------------

World::World()
	:xOffset(0), yOffset(0)
{
	debugmap = std::make_shared<Map>("data/maps/debugmap.txt");

	creatureList.push_back(player = std::make_shared<Player>(Position(2, 2, 0)));

	fovMap = std::make_shared<TCODMap>(debugmap->width, debugmap->height);

	addItem(ITEM_SIR556(4, 7, 0, player.get()));
	addItem(MAGAZINE_556Magazine30(5, 10, 0, player.get()));
	addItem(MAGAZINE_45Magazine7(10, 10, 0, player.get()));

	addCreature(std::make_shared<Creature>(Position(13, 38, 0), 'E', "Creature", TCODColor::white, 100, 0)); //replace colors
	addCreature(std::make_shared<Creature>(Position(16, 22, 0), 'H', "Creature", TCODColor::blue, 100, 0));
	addCreature(std::make_shared<Creature>(Position(64, 21, 0), 'F', "Creature", TCODColor::purple, 100, 0)); //grey when in fov???
}

std::shared_ptr<Tile> World::getTile(Position position) const
{
	if (inMapBounds(position))
	{
		return debugmap->levelList[position.level][position.x + position.y * debugmap->width];
	}
	else
	{
		return TILE_error;
	}
}

bool World::isExplored(Position position) const //err with this
{
	//position.x += xOffset;
	//position.y += yOffset;

	return debugmap->levelList[position.level][position.x + position.y * debugmap->width]->explored;
}

TCODColor World::getBgColor(Position position) const
{
	return debugmap->levelList[position.level][position.x + position.y * debugmap->width]->backgroundColor;
}

void World::addCreature(std::shared_ptr<Creature> creature)
{
	creatureList.push_back(creature);
}

void World::addItem(std::shared_ptr<Item> item)
{
	mapItemList.push_back(item);
}

void World::addContainer(std::shared_ptr<Container> container)
{
	mapContainerList.push_back(container);
}

bool World::inMapBounds(Position position) const
{
	if (position.level < 0 || position.level > debugmap->totalFloors - 1) return false;
	if (position.x < 0 || position.x > debugmap->width - 1) return false;
	if (position.y < 0 || position.y > debugmap->height - 1) return false;
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

bool World::getWalkability(Position position) const
{
	if (position.x < 0) return false;
	if (position.y < 0) return false;
	if (position.x >= debugmap->width) return false;
	if (position.y >= debugmap->height) return false;

	return debugmap->levelList[position.level][position.x + position.y * debugmap->width]->walkable;
}

bool World::getTransparency(Position position, int height) const
{
	if (debugmap->levelList[position.level][position.x + position.y * debugmap->width])
	{
		if (height <= debugmap->levelList[position.level][position.x + position.y * debugmap->width]->height)
		{
			if (debugmap->levelList[position.level][position.x + position.y * debugmap->width]->tag == Tile::Tag::DESTRUCTIBLE && debugmap->levelList[position.level][position.x + position.y * debugmap->width]->getDestroyed())
			{
				return true;
			}
			return false;
		}
		else if (height > debugmap->levelList[position.level][position.x + position.y * debugmap->width]->height)
		{
			return true;
		}
	}
	return false;
}

void World::updateProperties()
{
	for (int y = 0; y < debugmap->height; y++)
	{
		for (int x = 0; x < debugmap->width; x++)
		{
			fovMap->setProperties(x, y, getTransparency(Position(x, y, player->mapPosition.level), player->height), getWalkability(Position(x, y, player->mapPosition.level)));
		}
	}
}

void World::computeFov()
{
	fovMap->computeFov(player->mapPosition.x, player->mapPosition.y, engine->settings->fovRad, engine->settings->lightWalls, engine->settings->fovtype);
}

bool World::isInFov(Position position) const
{
	if (position.x < 0 || position.x >= debugmap->width || position.y < 0 || position.y >= debugmap->height)
	{
		return false;
	}
	if (fovMap->isInFov(position.x, position.y))
	{
		debugmap->levelList[position.level][position.x + position.y * debugmap->width]->explored = true;
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------

void World::updateEntities()
{
	for (auto& entity : creatureList)
	{
		entity->update();
	}
}

void World::update()
{
	xOffset = getOffset(player->mapPosition.x, debugmap->width, MAPPANE->drawWindow->consoleWidth);
	yOffset = getOffset(player->mapPosition.y, debugmap->height, MAPPANE->drawWindow->consoleHeight);

	if (INPUT->num9->isSwitched) // repeatable create live creature
	{
		addCreature(std::make_shared<Creature>(Position(30, 8, 0), 'E', "Creature", TCODColor::white, 100, 0));
	}

	updateProperties();
	computeFov();
	updateEntities();

	for (auto& item : mapItemList)
	{
		item->updateTile();
	}

	for (auto& container : mapContainerList)
	{
		container->containerItem->updateTile();
	}
}

//--------------------------------------------------------------------------------------------

void World::renderTiles(const std::shared_ptr<Pane>& pane) const
{
	for (int y = yOffset; y < pane->consoleHeight + yOffset; y++)
	{
		for (int x = xOffset; x < pane->consoleWidth + xOffset; x++)
		{
			getTile(Position(x, y, player->mapPosition.level))->render(x - xOffset, y - yOffset, pane);
		}
	}
}

void World::renderCreatures(const std::shared_ptr<Pane>& pane) const
{
	for (auto& creature : creatureList)
	{
		creature->render(pane);
	}
}

void World::render(const std::shared_ptr<Pane>& pane) const
{
	renderTiles(pane);

	for (auto& item : mapItemList)
	{
		if (item->mapPosition.level == player->mapPosition.level)
		{
			item->renderTile(pane);
		}
	}

	for (auto& container : mapContainerList)
	{
		if (container->containerItem->mapPosition.level == player->mapPosition.level)
		{
			container->containerItem->renderTile(pane);
		}
	}

	renderCreatures(pane);
}
