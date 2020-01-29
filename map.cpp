#include "main.hpp"

Map::Map(std::string filePath)
	:filePath(filePath), height(NULL), width(NULL), totalFloors(NULL)
{
	TCODRandom* RNG = TCODRandom::getInstance();
	RNG->setDistribution(TCOD_DISTRIBUTION_LINEAR);

	pugi::xml_document document;
	pugi::xml_parse_result result = document.load_file(filePath.c_str());

	if (result) //should add errors and exceptions to make sure map loads correctly
	{
		if (!document.child("map").empty())
		{
			for (auto& mapDataNode : document.child("map").children())
			{
				if (!mapDataNode.empty())
				{
					if (std::string(mapDataNode.name()) == "name")
					{
						name = mapDataNode.text().as_string();
					}

					if (std::string(mapDataNode.name()) == "levels")
					{
						totalFloors = mapDataNode.text().as_int();
						levelList = std::vector < std::vector < std::shared_ptr < Block >>>(totalFloors);
					}

					if (std::string(mapDataNode.name()) == "width")
					{
						width = mapDataNode.text().as_int();
					}

					if (std::string(mapDataNode.name()) == "height")
					{
						height = mapDataNode.text().as_int();
					}

					if (std::string(mapDataNode.name()) == "floor")
					{
						for (auto& level : levelList)
						{
							level.reserve(width * height);
						}

						std::string s_tileCodeList = mapDataNode.text().as_string();
						std::string code;
						int floor;

						if (!mapDataNode.attribute("level").empty())
						{
							floor = mapDataNode.attribute("level").as_int();

							for (int tileLocation = 1; tileLocation < width * height * 3; tileLocation += 3)
							{
								code = s_tileCodeList[tileLocation];
								code += s_tileCodeList[tileLocation + 1];

								levelList[floor].push_back(getTileFromCode(code));
							}
						}

					}
				}
			}
		}
	}
}

std::shared_ptr<Block> Map::getTileFromCode(std::string code)
{
	static TCODRandom* RNG = TCODRandom::getInstance();
	if (!RNG)
	{
		RNG->setDistribution(TCOD_DISTRIBUTION_LINEAR);
	}

	switch (code[0])
	{
	case 'f': //floors
		switch (code[1])
		{
			case '.': //grass
			{
				int grassRand = RNG->getInt(0, 3);

				if (grassRand == 0)
				{
					//return std::make_shared<Block>(DATA_Grass0, OOOOI, OOOOI);
					return TILE_Grass0;
				}
				else if (grassRand == 1)
				{
					return TILE_Grass1;
				}
				else if (grassRand == 2)
				{
					return TILE_Grass2;
				}
				else if (grassRand == 3)
				{
					return TILE_Grass3;
				}
				else return TILE_error;
				break;
			}
			case '_':
				return TILE_BasicFloor;
				break;
			case '!':
				return TILE_BasicConcrete;
				break;
			case '*':
				return TILE_BasicFlower;
				break;
			case '3':
				return TILE_BasicShingle;
				break;
			default:
				return TILE_error;
				break;
		}
		break;

	case 'w': //walls
		switch (code[1])
		{
			case '=':
				return DESTRUCTIBLE_BasicWall;
				break;
			case 'O':
				return DESTRUCTIBLE_BasicWindow;
				break;
			default:
				return TILE_error;
				break;
		}
		break;

	case 'd': //doors
		switch (code[1])
		{
			case '#':
				return TILE_BasicDoor;
				break;
			default:
				return TILE_error;
				break;
		}
		break;

	case 's': //skies
		switch (code[1])
		{
			case '`':
				return TILE_BasicSky;
				break;
			default:
				return TILE_error;
				break;
		}
		break;

	case 'p': //partial height
		switch (code[1])
		{
			case 'n':
				return TILE_BasicTableTop;
				break;
			case 'l':
				return TILE_BasicTableLeg;
				break;
			default:
				return TILE_error;
				break;
		}
		break;
	case 't': //stairs
		switch (code[1])
		{
			case '/':
				return STAIR_UpStair;
				break;
			case '\\':
				return STAIR_DownStair;
				break;
			default:
				return TILE_error;
				break;
		}
		break;

	default: //error tile
		return TILE_error;
		break;
	}
}

//----------------------------------------------------------------------------------------------------

World::World()
	:xOffset(0), yOffset(0)
{
	debugmap = std::make_shared<Map>("data/maps/debugmap.xml");

	creatureList.push_back(player = std::make_shared<Player>(Position4(2, 2, 3, 0)));

	fovMap = std::make_shared<TCODMap>(debugmap->width, debugmap->height);

	//first floor
	addItem(ITEM_SIR556(4, 7, 0, player.get()));
	addItem(ITEM_SIR556(33, 21, 0, player.get()));
	addItem(MAGAZINE_556Magazine30(5, 10, 0, player.get()));
	addItem(MAGAZINE_556Magazine30(22, 36, 0, player.get()));
	addItem(MAGAZINE_556Magazine30(31, 25, 0, player.get()));
	addItem(MAGAZINE_556Magazine30(56, 6, 0, player.get()));

	addItem(ITEM_testArmor(3, 3, 0, player.get()));
	
	//second floor
	addItem(ITEM_SIP45(21, 45, 0, player.get()));
	addItem(MAGAZINE_45Magazine7(10, 10, 0, player.get()));
	addItem(MAGAZINE_45Magazine7(36, 50, 0, player.get()));
	addItem(MAGAZINE_45Magazine7(49, 36, 0, player.get()));
	
	addContainer(CONTAINER_SmallBackpack(35, 35, 0, player.get()));

	addCreature(std::make_shared<Creature>(Position4(13, 38, 3, 0), '1', "Creature", TCODColor::white, 100, Armor("armor", TCODColor::pink, 100, 200))); //replace colors
	addCreature(std::make_shared<Creature>(Position4(16, 22, 3, 0), '0', "Creature", TCODColor::blue, 100, Armor("armor", TCODColor::pink, 0, 0)));
	addCreature(std::make_shared<Creature>(Position4(64, 21, 3, 0), '2', "Creature", TCODColor::purple, 100, Armor("armor", TCODColor::pink, 200, 400)));
}

std::shared_ptr<Block> World::getTile(Position3 position) const
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

bool World::isExplored(Position3 position) const
{
 	return debugmap->levelList[position.level][position.x + position.y * debugmap->width]->explored;
}

TCODColor World::getBgColor(Position3 position) const
{
	return debugmap->levelList[position.level][position.x + position.y * debugmap->width]->tileList[0]->backgroundColor;
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

bool World::inMapBounds(Position3 position) const
{
	if (position.level < 0 || position.level >= debugmap->totalFloors) return false;
	if (position.x < 0 || position.x >= debugmap->width) return false;
	if (position.y < 0 || position.y >= debugmap->height) return false;
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

bool World::getWalkability(Position4 position) const
{
	if (!inMapBounds(position)) //should never be called??
	{
		return false;
	}

	bool walkableBool = true;
	unsigned char walkableFlag = getTile(position)->walkableFlag;

	for (int i = 0; i < position.height; ++i) //check bound
	{
		if (walkableFlag & heightToBitFlag(position.height - i)) // converts player height to the bit of flag
		{
			walkableBool = false;
		}
	}

	if (walkableBool == true && walkableFlag & heightToBitFlag(0))
	{
		return true;
	}
	return false;
}

bool World::getSolidity(Position4 position) const
{
	if (getTile(position)->walkableFlag & heightToBitFlag(position.height))
	{
		return true;
	}
	return false;
}

bool World::getTransparency(Position4 position) const
{
	if (!(getTile(position)->transparentFlag & heightToBitFlag(position.height))) // converts player height to the bit of flag
	{
		return true;
	}
	return false;
}

void World::updateProperties()
{
	for (int y = 0; y < debugmap->height; ++y)
	{
		for (int x = 0; x < debugmap->width; ++x)
		{
			Position4 position = Position4(x, y, player->mapPosition.height, player->mapPosition.level);
			fovMap->setProperties(x, y, getTransparency(position), getWalkability(position));
		}
	}
}

void World::computeFov()
{
	fovMap->computeFov(player->mapPosition.x, player->mapPosition.y, engine->settings->fovRad, engine->settings->lightWalls, engine->settings->fovtype);
}

bool World::isInFov(Position3 position) const
{
	if (!inMapBounds(position))
	{
		return false;
	}
	if (fovMap->isInFov(position.x, position.y) && position.level == player->mapPosition.level)
	{
		getTile(position)->explored = true;
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
		addCreature(std::make_shared<Creature>(Position4(30, 8, 3, 0), '0', "Creature", TCODColor::white, 100, Armor("", TCODColor::pink, 0, 0)));
	}

	updateEntities(); //needs to be first to prevent bad fov checks
	updateProperties();
	computeFov();

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
	for (int y = yOffset; y < pane->consoleHeight + yOffset; ++y)
	{
		for (int x = xOffset; x < pane->consoleWidth + xOffset; ++x)
		{
			std::shared_ptr<Block> block = getTile(Position3(x, y, player->mapPosition.level));
			block->render(Position4(x - xOffset, y - yOffset, player->mapPosition.height, player->mapPosition.level), pane);
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
