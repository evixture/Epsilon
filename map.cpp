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
			bool hasName = false;
			bool hasLevels = false;
			bool hasWidth = false;
			bool hasHeight = false;

			bool hasMap = false;

			bool hasCreatures = false;
			bool hasItems = false;
			bool hasContainers = false;

			for (auto& mapDataNode : document.child("map").children())
			{
				if (!mapDataNode.empty())
				{
					if (!hasName) //if the map name hasnt been found, get the name
					{
						hasName = getMapName(mapDataNode); //gives bool if created right
					}
					if (!hasLevels)
					{
						hasLevels = getMapLevels(mapDataNode);
					}
					if (!hasWidth)
					{
						hasWidth = getMapWidth(mapDataNode);
					}
					if (!hasHeight)
					{
						hasHeight = getMapHeight(mapDataNode);
					}
					
					if (hasName && hasLevels && hasWidth && hasHeight) //if the map has gotten all of the map data properly
					{
						if (!hasMap)
						{
							hasMap = createBlockMap(mapDataNode);
						}

						if (hasMap)
						{
							if (!hasCreatures)
							{
								hasCreatures = getCreatures(mapDataNode);
							}

							if (!hasItems)
							{
								hasItems = getItems(mapDataNode);
							}

							if (!hasContainers)
							{
								hasContainers = getContainers(mapDataNode);
							}
						}
					}
				}
			}
		}
	}
}

bool Map::getMapName(pugi::xml_node& dataNode)
{
	std::string testName = dataNode.name();

	if (testName == "name")
	{
		if (!dataNode.text().empty())
		{
			name = dataNode.text().as_string();
			return true;
		}
		//else throw "Could not get map name!";
		//return false;
	}
	return false;
}

bool Map::getMapLevels(pugi::xml_node& dataNode)
{
	std::string testName = dataNode.name();

	if (testName == "levels")
	{
		if (!dataNode.text().empty())
		{
			totalFloors = dataNode.text().as_int();
			levelList = std::vector < std::vector < std::shared_ptr < Block >>>(totalFloors);
			return true;
		}
		//else throw "Could not get the number of map levels!";
	}
	return false;
}

bool Map::getMapHeight(pugi::xml_node& dataNode)
{
	std::string testName = dataNode.name();

	if (testName == "height")
	{
		if (!dataNode.text().empty())
		{
			height = dataNode.text().as_int();
			return true;
		}
		//else throw "Could not get the map height!";
	}
	return false;
}

bool Map::getMapWidth(pugi::xml_node& dataNode)
{
	std::string testName = dataNode.name();

	if (testName == "width")
	{
		if (!dataNode.text().empty())
		{
			width = dataNode.text().as_int();
			return true;
		}
		//else throw "Coild not get the map width!";
	}
	return false;
}

bool Map::createBlockMap(pugi::xml_node& dataNode)
{
	std::string testName = dataNode.name();

	if (testName == "floor")
	{
		for (auto& level : levelList)
		{
			level.reserve(width * height);
		}

		std::string s_tileCodeList = dataNode.text().as_string();
		std::string code;
		int floor;

		if (!dataNode.attribute("level").empty())
		{
			floor = dataNode.attribute("level").as_int();

			for (int tileLocation = 1; tileLocation < width * height * 3; tileLocation += 3)
			{
				code = s_tileCodeList[tileLocation];
				code += s_tileCodeList[tileLocation + 1];

				levelList[floor].push_back(std::make_shared<Block>(getTileFromCode(code)));
				
				//levelList[floor].push_back(std::make_shared<Block>(Destructible::wall));
			}
		}
		if (levelList.size() == totalFloors && levelList[totalFloors - 1].size() == width * height)
		{
			return true;
		}
	}
	return false;
}

bool Map::getCreatures(pugi::xml_node& dataNode)
{
	std::string testName = dataNode.name();

	if (testName == "creatures")
	{
		for (auto& creature : dataNode.children()) //for nodes in the creatures 
		{
			int x;
			int y;
			int level;
			int floor;

			std::string name;
			int ch;
			TCODColor color;
			int health;
			int armorDefense;
			int armorDurability;
			TCODColor armorColor;


			if (!(creature.attribute("name").empty()))	name	= creature.attribute("name").as_string();
			else return false;

			if (!(creature.child("x").empty()))			x		= creature.child("x").text().as_int();
			else return false;

			if (!(creature.child("y").empty()))			y		= creature.child("y").text().as_int();
			else return false;

			if (!(creature.child("level").empty()))		level	= creature.child("level").text().as_int();
			else return false;

			if (!(creature.child("floor").empty()))		floor	= creature.child("floor").text().as_int();
			else return false;


			if (name == std::string("Player")) //should check if not empty first
			{
				creatureList.push_back(player = std::make_shared<Player>(Position4(x, y, level, floor)));
			}
			else
			{
				if (!(creature.child("ch").empty())) ch = creature.child("ch").text().as_string()[0];
				else return false;

				std::string colorName;
				if (!(creature.child("color").empty())) colorName = creature.child("color").text().as_string();
				else return false;

				if (colorName == "blue")		color = TCODColor::blue;
				else if (colorName == "white")	color = TCODColor::white;
				else if (colorName == "purple") color = TCODColor::purple;
				else							color = TCODColor::yellow;

				if (!(creature.child("health").empty())) health = creature.child("health").text().as_int();
				else return false;

				pugi::xml_node armorData;
				if (!(creature.child("armor").empty()))
				{
					armorData = creature.child("armor");

					bool empty = creature.child("armor").child("durability").empty();

					if (!(creature.child("armor").child("defense").empty())) armorDefense = armorData.child("defense").text().as_int();
					else return false;
				
					if (!(creature.child("armor").child("durability").empty())) armorDurability = armorData.child("durability").text().as_int();
					else return false;


					std::string armorColorName;

					if (!(creature.child("armor").child("color").empty())) armorColorName = armorData.child("color").text().as_string();
					else return false;

					if (armorColorName == "pink")
					{
						armorColor = TCODColor::pink;
					}
				}
				creatureList.push_back(std::make_shared<Creature>(Position4(x, y, level, floor), ch, name, color, health, Armor("TEMP", armorColor, armorDefense, armorDurability))); //missing armor name in file
			}	
		}
		return true;
	}
	return false;
}

bool Map::getItems(pugi::xml_node& dataNode)
{
	std::string testName = dataNode.name();

	if (testName == "items")
	{
		for (auto& item : dataNode.children())
		{
			int x;
			int y;
			int floor;

			std::string name;

			if (!(item.attribute("type").empty()))	name = item.attribute("type").as_string();
			else return false;

			if (!(item.child("x").empty()))		x = item.child("x").text().as_int();
			else return false;

			if (!(item.child("y").empty()))		y = item.child("y").text().as_int();
			else return false;

			if (!(item.child("floor").empty())) floor = item.child("floor").text().as_int();
			else return false;

			if		(name == "SIR556")			mapItemList.push_back(ITEM_SIR556(x, y, floor, player.get()));			
			else if (name == "556Magazine30")	mapItemList.push_back(MAGAZINE_556Magazine30(x, y, floor, player.get()));			
			else if (name == "45Magazine7")		mapItemList.push_back(MAGAZINE_45Magazine7(x, y, floor, player.get()));
			else if (name == "L1R3Armor")		mapItemList.push_back(ITEM_L1R3Armor(x, y, floor, player.get()));
			else if (name == "Knife")			mapItemList.push_back(ITEM_Knife(x, y, floor, player.get()));
		}
		return true;
	}
	return false;
}

bool Map::getContainers(pugi::xml_node& dataNode)
{
	std::string testName = dataNode.name();

	if (testName == "containers")
	{
		for (auto& container : dataNode.children())
		{
			int x;
			int y;
			int floor;

			std::string name;

			if (!(container.child("x").empty()))		x = container.child("x").text().as_int();
			else return false;

			if (!(container.child("y").empty()))		y = container.child("y").text().as_int();
			else return false;

			if (!(container.child("floor").empty()))	floor = container.child("floor").text().as_int();
			else return false;

			if (!(container.child("type").empty()))		name = container.attribute("type").as_string();
			else return false;

			if (name == "SmallBackpack")
			{
				mapContainerList.push_back(CONTAINER_SmallBackpack(x, y, floor, player.get()));
			}
		}
		return true;
	}
	return false;
}

Block Map::getTileFromCode(std::string code)
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
					return Block::grass0;
					//return TILE_Grass0;
				}
				else if (grassRand == 1)
				{
					return Block::grass1;
					//return TILE_Grass1;
				}
				else if (grassRand == 2)
				{
					return Block::grass2;
					//return TILE_Grass2;
				}
				else if (grassRand == 3)
				{
					return Block::grass3;
					//return TILE_Grass3;
				}
				else return Block::error;//return TILE_error; 
				break;
			}
			case '_':
				return Block::floor;
				//return TILE_BasicFloor;
				break;
			case '!':
				return Block::concrete;
				//return TILE_BasicConcrete;
				break;
			case '*':
				return Block::flower;
				//return TILE_BasicFlower;
				break;
			case '3':
				return Block::shingle;
				//return TILE_BasicShingle;
				break;
			default:
				return Block::error;
				//return TILE_error;
				break;
		}
		break;

	case 'w': //walls
		switch (code[1])
		{
			case '=':
				return Destructible::wall;
				//return DESTRUCTIBLE_BasicWall;
				break;
			case 'O':
				return Destructible::window;
				//return DESTRUCTIBLE_BasicWindow;
				break;
			default:
				return Block::error;
				//return TILE_error;
				break;
		}
		break;

	case 'd': //doors
		switch (code[1])
		{
			case '#':
				return Block::door;
				//return TILE_BasicDoor;
				break;
			default:
				return Block::error;
				//return TILE_error;
				break;
		}
		break;

	case 's': //skies
		switch (code[1])
		{
			case '`':
				return Block::sky;
				//return TILE_BasicSky;
				break;
			default:
				return Block::error;
				//return TILE_error;
				break;
		}
		break;

	case 'p': //partial height
		switch (code[1])
		{
			case 'n':
				return Destructible::tableTop;
				//return TILE_BasicTableTop;
				break;
			case 'l':
				return Destructible::tableLeg;
				//return TILE_BasicTableLeg;
				break;
			default:
				return Block::error;
				//return TILE_error;
				break;
		}
		break;
	case 't': //stairs
		switch (code[1])
		{
			case '/':
				return Stair::upStair;
				//return STAIR_UpStair;
				break;
			case '\\':
				return Stair::downStair;
				//return STAIR_DownStair;
				break;
			default:
				return Block::error;
				//return TILE_error;
				break;
		}
		break;

	default: //error tile
		return Block::error;
		//return TILE_error;
		break;
	}
}

//----------------------------------------------------------------------------------------------------

World::World()
	:xOffset(0), yOffset(0)
{
	debugmap = std::make_shared<Map>("data/maps/debugmap.xml"); //constructed right, but falls out of scope?

	for (int i = 0; i < 3; i++)
	{
		fovMapList.push_back(std::make_shared<TCODMap>(debugmap->width, debugmap->height)); //71 * 70 = 4970 * 3 = 14910 total tiles to update each cycle
		//fovMapList = std::make_shared<TCODMap>(debugmap->width, debugmap->height);
	}

	createFovMap();
}

std::shared_ptr<Block> World::getTile(Position3 position) const
{
	if (inMapBounds(position))
	{
		return debugmap->levelList[position.floor][position.x + position.y * debugmap->width];
	}
	else
	{
		return TILE_error;
	}
}

bool World::isExplored(Position3& position) const
{
 	return debugmap->levelList[position.floor][position.x + position.y * debugmap->width]->explored;
}

void World::updateBlock(Position3 blockPosition, bool checkCreatures)
{
	Position4 position;

	for (int h = 0; h < 3; h++)
	{
		position = Position4(blockPosition.x, blockPosition.y, h + 1, blockPosition.floor);

		fovMapList[h]->setProperties(position.x, position.y, getTransparency(position), getWalkability(position, checkCreatures));
	}
}

TCODColor World::getBgColor(Position3& position) const
{
	return debugmap->levelList[position.floor][position.x + position.y * debugmap->width]->tileList[0]->backgroundColor;
}

void World::addCreature(std::shared_ptr<Creature> creature)
{
	debugmap->creatureList.push_back(creature);
}

void World::addItem(std::shared_ptr<Item> item)
{
	debugmap->mapItemList.push_back(item);
}

void World::addContainer(std::shared_ptr<Container> container)
{
	debugmap->mapContainerList.push_back(container);
}

bool World::inMapBounds(Position3& position) const
{
	if (position.floor < 0 || position.floor >= debugmap->totalFloors) return false;
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

void World::createFovMap()
{
	Position4 position;

	for (int h = 0; h < 3; h++)
	{
		for (int y = 0; y < debugmap->height; ++y)
		{
			for (int x = 0; x < debugmap->width; ++x)
			{
				position = Position4(x, y, h + 1, debugmap->player->mapPosition.floor);

				//what if each creature has its own 60 by 60 personal fov map (BAD IDEA)
				//how to unitize for optimization for bigger maps??
				fovMapList[h]->setProperties(x, y, getTransparency(position), getWalkability(position, true)); //very very big bottleneck
			}
		}
	}
}

bool World::getWalkability(Position4 position, bool checkCreatures) const
{
	if (!inMapBounds(position)) //should never be called??
	{
		return false;
	}

	if (checkCreatures)
	{
		for (auto& creature : debugmap->creatureList)
		{
			if (position.floor == creature->mapPosition.floor && position.x == creature->mapPosition.x && position.y == creature->mapPosition.y)
			{
				if (creature->health > 0)
				{
					return false;
				}
			}
		}
	}

	bool walkableBool = true;
	unsigned char walkableFlag = getTile(position)->walkableFlag;

	for (int i = 0; i < position.height; ++i)
	{
		if (walkableFlag & heightToBitFlag(position.height - i))
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

bool World::getSolidity(Position4& position) const
{
	if (getTile(position)->walkableFlag & heightToBitFlag(position.height))
	{
		return true;
	}
	return false;
}

bool World::getTransparency(Position4& position) const
{
	if (!(getTile(position)->transparentFlag & heightToBitFlag(position.height)))
	{
		return true;
	}
	return false;
}

void World::computeFov(Position4 mapPosition) //calculate the fov from the point of view of the map position
{
	int height;

	if (mapPosition.height < 1) height = 0;
	else height = mapPosition.height - 1;

	fovMapList[height]->computeFov(mapPosition.x, mapPosition.y, engine->settings->fovRad, engine->settings->lightWalls, engine->settings->fovtype);
}

bool World::isInPlayerFov(Position4 position) const
{
	if (!inMapBounds(position))
	{
		return false;
	}

	if (position.height < 1) return false;

	if (fovMapList[position.height - 1]->isInFov(position.x, position.y) && position.floor == debugmap->player->mapPosition.floor)
	{
		getTile(position)->explored = true;
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------

void World::updateEntities()
{
	for (auto& entity : debugmap->creatureList)
	{
		entity->update();
	}
}

void World::update()
{
	if (soundBuffer.size() > 0)
	{
		soundBuffer.clear();
	}

	xOffset = getOffset(debugmap->player->mapPosition.x, debugmap->width, MAPPANE->drawWindow->consoleWidth);
	yOffset = getOffset(debugmap->player->mapPosition.y, debugmap->height, MAPPANE->drawWindow->consoleHeight);

	if (INPUT->debug1Key->isSwitched) // repeatable create live creature
	{
		//addCreature(std::make_shared<Creature>(Position4(30, 8, 3, 0), '0', "Creature", TCODColor::white, 100, Armor("", TCODColor::pink, 0, 0)));
		//addCreature(std::make_shared<AICreature>(Creature(Position4(30, 8, 3, 0), 'I', "AI Creature", TCODColor::white, 100, Armor("", TCODColor::pink, 0, 0))));
		Creature creature = Creature(Position4(30, 8, 3, 0), 'I', "AI Creature", TCODColor::white, 100, Armor("", TCODColor::pink, 0, 0));

		debugmap->creatureList.push_back(std::make_shared<AICreature>(creature, fovMapList[2].get())); //use whole map list
	}

	updateEntities(); //needs to be first to prevent bad fov checks
	computeFov(debugmap->player->mapPosition);

	for (auto& item : debugmap->mapItemList)
	{
		item->updateTile();
	}

	for (auto& container : debugmap->mapContainerList)
	{
		container->containerItem->updateTile();
	}

	if (soundList.size() > 0)
	{
		soundList.clear();
	}
	soundList = soundBuffer; //move all sounds from the buffer to the list so that creatures updated before the sound creation can react
}

//--------------------------------------------------------------------------------------------

void World::renderTiles(const std::shared_ptr<Pane>& pane) const
{
	for (int y = yOffset; y < pane->consoleHeight + yOffset; ++y)
	{
		for (int x = xOffset; x < pane->consoleWidth + xOffset; ++x)
		{
			std::shared_ptr<Block> block = getTile(Position3(x, y, debugmap->player->mapPosition.floor));
			block->render(Position4(x - xOffset, y - yOffset, debugmap->player->mapPosition.height, debugmap->player->mapPosition.floor), pane);
		}
	}
}

void World::renderCreatures(const std::shared_ptr<Pane>& pane) const
{
	for (auto& creature : debugmap->creatureList)
	{
		creature->render(pane);
	}
}

void World::render(const std::shared_ptr<Pane>& pane) const
{
	renderTiles(pane);

	for (auto& item : debugmap->mapItemList)
	{
		if (item->mapPosition.floor == debugmap->player->mapPosition.floor)
		{
			item->renderTile(pane);
		}
	}

	for (auto& container : debugmap->mapContainerList)
	{
		if (container->containerItem->mapPosition.floor == debugmap->player->mapPosition.floor)
		{
			container->containerItem->renderTile(pane);
		}
	}

	renderCreatures(pane);
}
