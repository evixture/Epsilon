#include "main.hpp"

Map::Map(std::string filePath)
	: filePath(filePath), height(NULL), width(NULL), totalFloors(NULL)
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
						if (!hasMap) hasMap = createBlockMap(mapDataNode);

						if (hasMap)
						{
							if (!hasCreatures)	hasCreatures = getCreatures(mapDataNode);

							if (!hasItems)		hasItems = getItems(mapDataNode);

							if (!hasContainers) hasContainers = getContainers(mapDataNode);
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
			}
		}
		if (levelList.size() == totalFloors && levelList[totalFloors - 1].size() == width * height)
		{
			for (int i = 0; i < 3; i++) fovMapList.push_back(std::make_shared<TCODMap>(width, height));

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
				Creature creature = Creature(Position4(x, y, level, floor), ch, name, Position3(0, 0, 255), health, Armor(&creature, "TEMP", armorColor, armorDefense, armorDurability)); //missing armor name in file
				creatureList.push_back(std::make_shared<AICreature>(creature, fovMapList[creature.mapPosition.h - 1].get()));
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
			int x, y, floor;
			std::string type;

			if (!(item.attribute("type").empty()))	type = item.attribute("type").as_string();
			else return false;

			if (!(item.child("x").empty()))		x = item.child("x").text().as_int();
			else return false;

			if (!(item.child("y").empty()))		y = item.child("y").text().as_int();
			else return false;

			if (!(item.child("floor").empty())) floor = item.child("floor").text().as_int();
			else return false;

			if		(type == "SIR556")			mapItemList.push_back(std::make_shared<Item>(ep::item::sir556(nullptr, x, y, floor)));			
			else if (type == "556Magazine30")	mapItemList.push_back(std::make_shared<MagazineItem>(ep::magazineItem::cal556Magazine30(nullptr, x, y, floor)));
			else if (type == "45Magazine7")		mapItemList.push_back(std::make_shared<MagazineItem>(ep::magazineItem::cal45Magazine7(nullptr, x, y, floor)));
			else if (type == "L1R3Armor")		mapItemList.push_back(std::make_shared<Item>(ep::item::L1R3Armor(nullptr, x, y, floor)));
			else if (type == "Knife")			mapItemList.push_back(std::make_shared<Item>(ep::item::knife(nullptr, x, y, floor)));
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
			int x, y, floor;
			std::string type;

			if (!(container.attribute("type").empty()))	type = container.attribute("type").as_string();
			else return false;

			if (!(container.child("x").empty()))		x = container.child("x").text().as_int();
			else return false;

			if (!(container.child("y").empty()))		y = container.child("y").text().as_int();
			else return false;

			if (!(container.child("floor").empty()))	floor = container.child("floor").text().as_int();
			else return false;

			if (type == "SmallBackpack") mapContainerList.push_back(std::make_shared<Container>(ep::container::smallBackpack(nullptr, x, y, floor)));
		}
		return true;
	}
	return false;
}

std::shared_ptr<Block> Map::getTileFromCode(std::string code)
{
	static TCODRandom* RNG = TCODRandom::getInstance();
	if (!RNG) RNG->setDistribution(TCOD_DISTRIBUTION_LINEAR);

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

bool Map::inMapBounds(Position3 position) const
{
	return ((position.z >= 0 && position.z < totalFloors) && (position.x >= 0 && position.x < width) && (position.y >= 0 && position.y < height)); //can optimize by checking opposite
}

bool Map::getWalkability(Position4 position, bool checkCreatures) const
{
	if (!inMapBounds(position)) return false;

	if (checkCreatures)
	{
		for (auto& creature : creatureList)
		{
			if (position.z == creature->mapPosition.z && position.x == creature->mapPosition.x && position.y == creature->mapPosition.y)
			{
				if (creature->health > 0) return false;
			}
		}
	}

	bool walkableBool = true;
	unsigned char walkableFlag = getBlock(position)->walkableFlag;

	for (int i = 0; i < position.h; ++i)
	{
		if (walkableFlag & heightToBitFlag(position.h - i)) walkableBool = false;
	}

	if (walkableBool == true && walkableFlag & heightToBitFlag(0)) return true;
	return false;
}

bool Map::getSolidity(Position4& position) const
{
	return (getBlock(position)->walkableFlag & heightToBitFlag(position.h));
}

bool Map::getTransparency(Position4& position) const
{
	return (!(getBlock(position)->transparentFlag & heightToBitFlag(position.h)));
}

std::shared_ptr<Block> Map::getBlock(Position3 position) const
{
	if ((position.z >= 0 && position.z < totalFloors) && (position.x >= 0 && position.x < width) && (position.y >= 0 && position.y < height))
	{
		return levelList[position.z][position.x + position.y * width]; //returns bad blocks
	}
	else
	{
		return std::make_shared<Block>(ep::block::error);
	}
}

//----------------------------------------------------------------------------------------------------

World::World()
	: xOffset(0), yOffset(0)
{
	debugmap = std::make_shared<Map>("data/maps/debugmap.emp");
}

bool World::isExplored(Position3& position) const
{
 	return debugmap->levelList[position.z][position.x + position.y * debugmap->width]->explored;
}

void World::addSound(Sound sound)
{
	soundBuffer.push_back(std::make_shared<Sound>(sound));
}

void World::updateBlock(Position3 blockPosition, bool checkCreatures)
{
	Position4 position;

	for (int h = 0; h < 3; h++)
	{
		position = Position4(blockPosition.x, blockPosition.y, h + 1, blockPosition.z);

		debugmap->fovMapList[h]->setProperties(position.x, position.y, debugmap->getTransparency(position), debugmap->getWalkability(position, checkCreatures));
	}
}

TCODColor World::getBgColor(Position3& position) const
{
	return debugmap->levelList[position.z][position.x + position.y * debugmap->width]->tileList[0].backgroundColor;
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
	for (auto& fovMap : debugmap->fovMapList)
	{
		fovMap->computeFov(mapPosition.x, mapPosition.y, ENGINE->fovRad, ENGINE->lightWalls, ENGINE->fovtype);
	}
}

bool World::isInPlayerFov(Position4 position) const
{
	if (!debugmap->inMapBounds(position) || position.h < 1)
	{
		return false;
	}

	if (position.z == debugmap->player->mapPosition.z) //condense back
	{
		if (debugmap->fovMapList[position.h - 1]->isInFov(position.x, position.y))
		{
			debugmap->getBlock(position)->explored = true;
			return true;
		}
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
	xOffset = getOffset(debugmap->player->mapPosition.x, debugmap->width, MAPPANE->drawPane.consoleWidth);
	yOffset = getOffset(debugmap->player->mapPosition.y, debugmap->height, MAPPANE->drawPane.consoleHeight);

	if (INPUT->debug1->bind->isSwitched) AUDIO->playSound(PositionalStaticSound("Testing", Position4(20, 10, 0, 0), 0.0f, 100.0f)); //2d sound

	updateEntities(); //needs to be first to prevent bad fov checks
	computeFov(debugmap->player->mapPosition);

	for (auto& item : debugmap->mapItemList)
	{
		if (!item->onMap) item->onMap = true;
		item->updateTile();
	}

	for (auto& container : debugmap->mapContainerList)
	{
		if (!container->item->onMap) container->item->onMap = true;
		container->item->updateTile();
	}

	soundList = soundBuffer;
	if (soundBuffer.size() > 0) soundBuffer.clear();
}

//--------------------------------------------------------------------------------------------

void World::renderTiles(const Pane& pane) const
{
	for (int y = yOffset; y < pane.consoleHeight + yOffset; ++y)
	{
		for (int x = xOffset; x < pane.consoleWidth + xOffset; ++x)
		{
			if (SETTINGS->showWalkable) //render walkability
			{
				if (debugmap->getWalkability(Position4(x, y, debugmap->player->mapPosition.h, 0), true) == true)	pane.console->setCharBackground(x - xOffset, y - yOffset, TCODColor::green);
				else																								pane.console->setCharBackground(x - xOffset, y - yOffset, TCODColor::red);
			}
			else if (SETTINGS->showFOV) //render fov
			{
				if (isInPlayerFov(Position4(x, y, debugmap->player->mapPosition.h, 0)) == true) pane.console->setCharBackground(x - xOffset, y - yOffset, TCODColor::green);
				else																			pane.console->setCharBackground(x - xOffset, y - yOffset, TCODColor::red);
			}
			else debugmap->getBlock(Position3(x, y, debugmap->player->mapPosition.z))->render(Position4(x - xOffset, y - yOffset, debugmap->player->mapPosition.h, debugmap->player->mapPosition.z), pane);
		}
	}
}

void World::renderWorldItems(const Pane& pane) const
{
	for (auto& item : debugmap->mapItemList)
	{
		if (item->mapPosition.z == debugmap->player->mapPosition.z) item->render(pane);
	}

	for (auto& container : debugmap->mapContainerList)
	{
		if (container->item->mapPosition.z == debugmap->player->mapPosition.z) container->item->render(pane);
	}
}

void World::renderCreatures(const Pane& pane) const
{
	//render creature items first, then creatures in seperate pass??
	for (auto& creature : debugmap->creatureList) creature->render(pane);
	debugmap->player->render(pane);
}

void World::render(const Pane& pane) const
{
	/*
	RENDER ORDER
		All tiles
		dead creatures?
		item tiles
		creature items
		creatures
		player items
		player
	*/

	renderTiles(pane);
	renderWorldItems(pane);
	renderCreatures(pane);
}
