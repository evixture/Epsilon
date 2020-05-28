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

void Map::addCreature(std::shared_ptr<Creature> creature)
{
	creatureList.push_back(creature);
}

void Map::addItem(std::shared_ptr<Item> item)
{
	mapItemList.push_back(item);
}

void Map::addContainer(std::shared_ptr<Container> container)
{
	mapContainerList.push_back(container);
}

void Map::refreshFOV(int floor)
{
	Position4 position;

	for (int h = 0; h < 3; h++)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				position = Position4(x, y, h + 1, floor); //updates with floor??

				fovMapList[h]->setProperties(x, y, getTransparency(position), getWalkability(position, true)); //very very big bottleneck
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
			levelList = std::vector<std::vector<std::shared_ptr<Block>>>(totalFloors);
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

				levelList[floor].push_back((getTileFromCode(code)));
				
				//levelList[floor].push_back(std::make_shared<Block>(Destructible::wall));
			}
		}
		if (levelList.size() == totalFloors && levelList[totalFloors - 1].size() == width * height)
		{
			for (int i = 0; i < 3; i++)
			{
				fovMapList.push_back(std::make_shared<TCODMap>(width, height));
			}

			Position4 position;

			for (int h = 0; h < 3; h++)
			{
				for (int y = 0; y < height; ++y)
				{
					for (int x = 0; x < width; ++x)
					{
						position = Position4(x, y, h + 1, 0);

						//how to unitize for optimization for bigger maps??
						fovMapList[h]->setProperties(x, y, getTransparency(position), getWalkability(position, true)); //very very big bottleneck
					}
				}
			}
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
				Creature creature = Creature(Position4(x, y, level, floor), ch, name, color, health, Armor("TEMP", armorColor, armorDefense, armorDurability)); //missing armor name in file
				creatureList.push_back(std::make_shared<AICreature>(creature, fovMapList[creature.mapPosition.height - 1].get()));
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

			if		(name == "SIR556")			mapItemList.push_back(std::make_shared<Item>(ep::item::sir556(x, y, floor)));			
			else if (name == "556Magazine30")	mapItemList.push_back(std::make_shared<MagazineItem>(ep::magazine::cal556Magazine30(x, y, floor)));			
			else if (name == "45Magazine7")		mapItemList.push_back(std::make_shared<MagazineItem>(ep::magazine::cal45Magazine7(x, y, floor)));
			else if (name == "L1R3Armor")		mapItemList.push_back(std::make_shared<Item>(ep::item::L1R3Armor(x, y, floor)));
			else if (name == "Knife")			mapItemList.push_back(std::make_shared<Item>(ep::item::knife(x, y, floor)));
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
				mapContainerList.push_back(std::make_shared<Container>(ep::container::smallBackpack(x, y, floor)));
			}
		}
		return true;
	}
	return false;
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
					return std::make_shared<Block>(ep::block::grass0);
				}
				else if (grassRand == 1)
				{
					return std::make_shared<Block>(ep::block::grass1);
				}
				else if (grassRand == 2)
				{
					return std::make_shared<Block>(ep::block::grass2);
				}
				else if (grassRand == 3)
				{
					return std::make_shared<Block>(ep::block::grass3);

				}
				return std::make_shared<Block>(ep::block::error);
				break;
			}
			case '_':
				return std::make_shared<Block>(ep::block::floor);
				break;
			case '!':
				return std::make_shared<Block>(ep::block::concrete);
				break;
			case '*':
				return std::make_shared<Block>(ep::block::flower);
				break;
			case '3':
				return std::make_shared<Block>(ep::block::shingle);
				break;
			default:
				return std::make_shared<Block>(ep::block::error);
				break;
		}
		break;

	case 'w': //walls
		switch (code[1])
		{
			case '=':
				return std::make_shared<Block>(ep::block::wall);
				break;
			case 'O':
				return std::make_shared<Block>(ep::block::window);
				break;
			default:
				return std::make_shared<Block>(ep::block::error);
				break;
		}
		break;

	case 'd': //doors
		switch (code[1])
		{
			case '#':
				return std::make_shared<Block>(ep::block::door);
				break;
			default:
				return std::make_shared<Block>(ep::block::error);
				break;
		}
		break;

	case 's': //skies
		switch (code[1])
		{
			case '`':
				return std::make_shared<Block>(ep::block::sky);
				break;
			default:
				return std::make_shared<Block>(ep::block::error);
				break;
		}
		break;

	case 'p': //partial height
		switch (code[1])
		{
			case 'n':
				return std::make_shared<Block>(ep::block::tableTop);
				break;
			case 'l':
				return std::make_shared<Block>(ep::block::tableLeg);
				break;
			default:
				return std::make_shared<Block>(ep::block::error);
				break;
		}
		break;
	case 't': //stairs
		switch (code[1])
		{
			case '/':
				return std::make_shared<Stair>(ep::block::upStair);
				break;
			case '\\':
				return std::make_shared<Stair>(ep::block::downStair);
				break;
			default:
				return std::make_shared<Block>(ep::block::error);
				break;
		}
		break;

	default:
		return std::make_shared<Block>(ep::block::error);
		break;
	}
}

bool Map::inMapBounds(Position3& position) const
{
	if ((position.floor >= 0 && position.floor < totalFloors) && (position.x >= 0 && position.x < width) && (position.y >= 0 && position.y < height)) return true;
	return false;
}

bool Map::getWalkability(Position4 position, bool checkCreatures) const
{
	if (!inMapBounds(position)) //should never be called??
	{
		return false;
	}

	if (checkCreatures)
	{
		for (auto& creature : creatureList)
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
	unsigned char walkableFlag = getBlock(position)->walkableFlag;

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

bool Map::getSolidity(Position4& position) const
{
	if (getBlock(position)->walkableFlag & heightToBitFlag(position.height))
	{
		return true;
	}
	return false;
}

bool Map::getTransparency(Position4& position) const
{
	if (!(getBlock(position)->transparentFlag & heightToBitFlag(position.height)))
	{
		return true;
	}
	return false;
}

std::shared_ptr<Block> Map::getBlock(Position3 position) const
{
	if ((position.floor >= 0 && position.floor < totalFloors) && (position.x >= 0 && position.x < width) && (position.y >= 0 && position.y < height))
	{
		return levelList[position.floor][position.x + position.y * width]; //returns bad blocks
	}
	else
	{
		return std::make_shared<Block>(ep::block::error);
	}
}

//----------------------------------------------------------------------------------------------------

World::World()
	:xOffset(0), yOffset(0)//, soundManager(std::make_shared<Audio>())
{
	debugmap = std::make_shared<Map>("data/maps/debugmap.emp");

	//testSound = Sound(Position4(0, 0, 0, 0), 100, 100);
	/*for (int i = 0; i < 3; i++)
	{
		fovMapList.push_back(std::make_shared<TCODMap>(debugmap->width, debugmap->height));
	}*/
	//createFovMap();
}

bool World::isExplored(Position3& position) const
{
 	return debugmap->levelList[position.floor][position.x + position.y * debugmap->width]->explored;
}

void World::addSound(Sound sound) //need params
{
	soundBuffer.push_back(std::make_shared<Sound>(sound));
	//return soundManager->playSound(sound); //returns the sound handle
}

void World::updateBlock(Position3 blockPosition, bool checkCreatures)
{
	Position4 position;

	for (int h = 0; h < 3; h++)
	{
		position = Position4(blockPosition.x, blockPosition.y, h + 1, blockPosition.floor);

		debugmap->fovMapList[h]->setProperties(position.x, position.y, debugmap->getTransparency(position), debugmap->getWalkability(position, checkCreatures));
	}
}

TCODColor World::getBgColor(Position3& position) const
{
	return debugmap->levelList[position.floor][position.x + position.y * debugmap->width]->tileList[0].backgroundColor;
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

void World::computeFov(Position4 mapPosition) //calculate the fov from the point of view of the map position
{
	int height;

	if (mapPosition.height < 1) height = 0;
	else height = mapPosition.height - 1;

	debugmap->fovMapList[height]->computeFov(mapPosition.x, mapPosition.y, engine->settings->fovRad, engine->settings->lightWalls, engine->settings->fovtype);
}

bool World::isInPlayerFov(Position4 position) const
{
	if (!debugmap->inMapBounds(position) || position.height < 1)
	{
		return false;
	}

	if (position.floor == debugmap->player->mapPosition.floor && debugmap->fovMapList[position.height - 1]->isInFov(position.x, position.y))
	{
		debugmap->getBlock(position)->explored = true;
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
	xOffset = getOffset(debugmap->player->mapPosition.x, debugmap->width, MAPPANE->drawWindow->consoleWidth);
	yOffset = getOffset(debugmap->player->mapPosition.y, debugmap->height, MAPPANE->drawWindow->consoleHeight);

	if (INPUT->debug1Key->isSwitched)
	{
		//addSound(Sound("Testing", true, Position4(20, 10, 3, 0), 100.0f, 100.0f));
		AUDIO->playSound(PositionalStaticSound("Testing", Position4(20, 10, 0, 0), 0.0f, 100.0f)); //2d sound
	}

	//soundManager->update();

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

	//AUDIO
	soundList = soundBuffer;

	if (soundBuffer.size() > 0)
	{
		soundBuffer.clear();
	}

	//if (soundList.size() > 0)
	//{
	//	soundList.clear();
	//}
	//soundList = soundBuffer; //move all sounds from the buffer to the list so that creatures updated before the sound creation can react
	//
	//for (auto sound : soundList) //sound cut off when list clears
	//{
	//	ENGINE->audio->play(sound->speech);
	//}

	/*
	BETTER AUDIO LIST

	creatures push sounds to sound buffer, and react to the sound buffer

	RUN 1:
		creature 1 push sound buffer
		creature 2 push sound buffer

		sound list set equal to sound buffer
		buffer cleared

		sound list plays sound for each in list, then clears
		
		sound has react bool that is set false after first frame

	RUN 2:
		creature 1 react to both sounds
		creature 2 react to both sounds

		list set to buffer
		buffer cleared
	
	NEW IDEA:

	play sound function in map
		adds "sound" to world sound list, only uses for ai reaction
		also adds sound to sound manager
	*/
}

//--------------------------------------------------------------------------------------------

void World::renderTiles(const std::shared_ptr<Pane>& pane) const
{
	for (int y = yOffset; y < pane->consoleHeight + yOffset; ++y)
	{
		for (int x = xOffset; x < pane->consoleWidth + xOffset; ++x)
		{
			//same perf
			//std::shared_ptr<Block> block = debugmap->getBlock(Position3(x, y, debugmap->player->mapPosition.floor));
			//block->render(Position4(x - xOffset, y - yOffset, debugmap->player->mapPosition.height, debugmap->player->mapPosition.floor), pane);

			//render walkability
			if (debugmap->getWalkability(Position4(x, y, debugmap->player->mapPosition.height, 0), true) == true)
			{
				pane->console->setCharBackground(x - xOffset, y - yOffset, TCODColor::green);
			}
			else
			{
				pane->console->setCharBackground(x - xOffset, y - yOffset, TCODColor::red);
			}

			//debugmap->getBlock(Position3(x, y, debugmap->player->mapPosition.floor))->render(Position4(x - xOffset, y - yOffset, debugmap->player->mapPosition.height, debugmap->player->mapPosition.floor), pane);
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
