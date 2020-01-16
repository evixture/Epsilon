#include "main.hpp"

/*

pugi::xml_node mapNode = document.child("map");

			if (!mapNode.child("name").empty())
			{
				name = mapNode.child("name").text().as_string();
			}
			else
			{
				//error
			}

			if (!mapNode.child("levels").empty())
			{
				totalFloors = mapNode.child("levels").text().as_int();
			}
			else
			{
				//error
			}

			if (!mapNode.child("width").empty())
			{
				width = mapNode.child("width").text().as_int();
			}
			else
			{
				//error
			}

			if (!mapNode.child("height").empty())
			{
				height = mapNode.child("height").text().as_int();
			}
			else
			{
				//error
			}

			if (!mapNode.child("floor").empty())
			{
				levelList = std::vector < std::vector < std::shared_ptr < Tile >>>(totalFloors);

				std::string s_tileCodeList;

				for (auto& level : levelList)
				{
					level.reserve(width * height);
				}

				for (pugi::xml_node floorNode = mapNode.child("floor"); floorNode; floorNode = floorNode.next_sibling("floor")) //wont repeat at the end of the tile list
				{
					pugi::xml_attribute floorAttr;

					for (auto& attr : floorNode.attributes())
					{
						floorAttr = attr;
					}
					int currentFloor;
					std::stringstream(floorAttr.name()) >> currentFloor;

					//xmlDoc.child("map").child("name").attribute("category").value() || TRY THIS LATER

					s_tileCodeList = floorNode.text().as_string();
					std::string code;

					for (int tileLocation = 1; tileLocation < width * height * 3; tileLocation += 3) //check range
					{
						code = s_tileCodeList[tileLocation];
						code += s_tileCodeList[tileLocation + 1];

						levelList[currentFloor].push_back(getTileFromCode(code));
					}
				}
			}
		}
		else
		{
			//error
		}


*/

Map::Map(std::string filePath)
	:filePath(filePath), height(NULL), width(NULL), totalFloors(NULL)
{
	TCODRandom* RNG = TCODRandom::getInstance();
	RNG->setDistribution(TCOD_DISTRIBUTION_LINEAR);

	int currentFloor = 0;

	pugi::xml_document document;
	pugi::xml_parse_result result = document.load_file("data/maps/testxmlmap.xml");

	if (result)
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
						levelList = std::vector < std::vector < std::shared_ptr < Tile >>>(totalFloors);
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
						int floor = mapDataNode.attribute("level").as_int(); //check if not empty

						for (int tileLocation = 1; tileLocation < width * height * 3; tileLocation += 3) //check range
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

	//std::ifstream fileIn(filePath, std::ios::in);
	//
	//if (fileIn.is_open())
	//{
	//	std::string s_mapName;
	//	std::string s_tempTile;
	//	
	//	fileIn >> s_mapName >> totalFloors >> width >> height;
	//	name = s_mapName.c_str();
	//
	//	//reserve vec size beforehand to inc perf?
	//	levelList = std::vector < std::vector < std::shared_ptr < Tile >>> (totalFloors);
	//
	//	for (auto& level : levelList)
	//	{
	//		level.reserve(width * height);
	//	}
	//	
	//	while (!fileIn.eof())
	//	{
	//		if (fileIn.good())
	//		{
	//			fileIn >> s_tempTile;
	//
	//			if (levelList[currentFloor].size() == width * height)
	//			{
	//				currentFloor++;
	//			}
	//
	//			switch (s_tempTile[0])
	//			{
	//			case '~':
	//				//map layer divider
	//				break;
	//
	//			//FLOORS
	//			case 'f':
	//				switch (s_tempTile[1])
	//				{
	//				case '.':
	//				{
	//					int grassRand = RNG->getInt(0, 3);
	//
	//					if (grassRand == 0)
	//					{
	//						levelList[currentFloor].push_back(TILE_Grass0);
	//					}
	//					else if (grassRand == 1)
	//					{
	//						levelList[currentFloor].push_back(TILE_Grass1);
	//					}
	//					else if (grassRand == 2)
	//					{
	//						levelList[currentFloor].push_back(TILE_Grass2);
	//					}
	//					else if (grassRand == 3)
	//					{
	//						levelList[currentFloor].push_back(TILE_Grass3);
	//					}
	//				}
	//					break;
	//				case '_':
	//					levelList[currentFloor].push_back(TILE_BasicFloor);
	//					break;
	//				case '!':
	//					levelList[currentFloor].push_back(TILE_BasicConcrete);
	//					break;
	//				case '*':
	//					levelList[currentFloor].push_back(TILE_BasicFlower);
	//					break;
	//				case '3':
	//					levelList[currentFloor].push_back(TILE_BasicShingle);
	//					break;
	//				default:
	//					levelList[currentFloor].push_back(TILE_error);
	//					break;
	//				}
	//				break;
	//
	//			//WALLS
	//			case 'w':
	//				switch (s_tempTile[1])
	//				{
	//				case '=':
	//					levelList[currentFloor].push_back(DESTRUCTIBLE_BasicWall);
	//					break;
	//				case 'O':
	//					levelList[currentFloor].push_back(DESTRUCTIBLE_BasicWindow);
	//					break;
	//				}
	//				break;
	//
	//			//DOORS
	//			case 'd':
	//				switch (s_tempTile[1])
	//				{
	//				case '#':
	//					levelList[currentFloor].push_back(TILE_BasicDoor);
	//					break;
	//				}
	//				break;
	//
	//			//SKIES
	//			case 's':
	//				switch (s_tempTile[1])
	//				{
	//				case '`':
	//					levelList[currentFloor].push_back(TILE_BasicSky);
	//					break;
	//				}
	//				break;
	//
	//			//PART HEIGHT
	//			case 'p':
	//				switch (s_tempTile[1])
	//				{
	//				case 'n':
	//					levelList[currentFloor].push_back(TILE_BasicTable);
	//					break;
	//				}
	//				break;
	//			case 't':
	//				switch (s_tempTile[1])
	//				{
	//				case '/':
	//					levelList[currentFloor].push_back(STAIR_UpStair);
	//					break;
	//				case '\\':
	//					levelList[currentFloor].push_back(STAIR_DownStair);
	//					break;
	//				}
	//				break;
	//
	//			//ERROR TILE
	//			default:
	//				levelList[currentFloor].push_back(TILE_error);
	//				break;
	//			}
	//		}
	//	}
	//}

/*

switch (s_tileCodeList[tileLocation])
						{
						case '~':
							//map layer divider
							break;

							//FLOORS
						case 'f':
							switch (s_tileCodeList[tileLocation + 1])
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
								break;
							}
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
							switch (s_tileCodeList[tileLocation + 1])
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
							switch (s_tileCodeList[tileLocation + 1])
							{
							case '#':
								levelList[currentFloor].push_back(TILE_BasicDoor);
								break;
							}
							break;

							//SKIES
						case 's':
							switch (s_tileCodeList[tileLocation + 1])
							{
							case '`':
								levelList[currentFloor].push_back(TILE_BasicSky);
								break;
							}
							break;

							//PART HEIGHT
						case 'p':
							switch (s_tileCodeList[tileLocation + 1])
							{
							case 'n':
								levelList[currentFloor].push_back(TILE_BasicTable);
								break;
							}
							break;
						case 't':
							switch (s_tileCodeList[tileLocation + 1])
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

*/
}

std::shared_ptr<Tile> Map::getTileFromCode(std::string code)
{
	static TCODRandom* RNG = TCODRandom::getInstance();
	RNG->setDistribution(TCOD_DISTRIBUTION_LINEAR); //check perf

	switch (code[0])
	{
	case '~':
		//map layer divider
		break;

		//FLOORS
	case 'f':
		switch (code[1])
		{
		case '.':
		{
			int grassRand = RNG->getInt(0, 3);

			if (grassRand == 0)
			{
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

		//WALLS
	case 'w':
		switch (code[1])
		{
		case '=':
			return DESTRUCTIBLE_BasicWall;
			break;
		case 'O':
			return DESTRUCTIBLE_BasicWindow;
			break;
		}
		break;

		//DOORS
	case 'd':
		switch (code[1])
		{
		case '#':
			return TILE_BasicDoor;
			break;
		}
		break;

		//SKIES
	case 's':
		switch (code[1])
		{
		case '`':
			return TILE_BasicSky;
			break;
		}
		break;

		//PART HEIGHT
	case 'p':
		switch (code[1])
		{
		case 'n':
			return TILE_BasicTable;
			break;
		}
		break;
	case 't':
		switch (code[1])
		{
		case '/':
			return STAIR_UpStair;
			break;
		case '\\':
			return STAIR_DownStair;
			break;
		}
		break;

		//ERROR TILE
	default:
		return TILE_error;
		break;
	}
}

//----------------------------------------------------------------------------------------------------

World::World()
	:xOffset(0), yOffset(0)
{
	debugmap = std::make_shared<Map>("data/maps/testxmlmap.xml");

	creatureList.push_back(player = std::make_shared<Player>(Position(2, 2, 0)));

	fovMap = std::make_shared<TCODMap>(debugmap->width, debugmap->height);

	//first floor
	addItem(ITEM_SIR556(4, 7, 0, player.get()));
	addItem(ITEM_SIR556(33, 21, 0, player.get()));
	addItem(MAGAZINE_556Magazine30(5, 10, 0, player.get()));
	addItem(MAGAZINE_556Magazine30(22, 36, 0, player.get()));
	addItem(MAGAZINE_556Magazine30(31, 25, 0, player.get()));
	addItem(MAGAZINE_556Magazine30(56, 6, 0, player.get()));


	addItem(ITEM_SIP45(21, 45, 0, player.get()));
	addItem(MAGAZINE_45Magazine7(10, 10, 0, player.get()));
	addItem(MAGAZINE_45Magazine7(36, 50, 0, player.get()));
	addItem(MAGAZINE_45Magazine7(49, 36, 0, player.get()));

	addContainer(CONTAINER_SmallBackpack(35, 35, 0, player.get()));

	addCreature(std::make_shared<Creature>(Position(13, 38, 0), 'E', "Creature", TCODColor::white, 100, 0)); //replace colors
	addCreature(std::make_shared<Creature>(Position(16, 22, 0), 'H', "Creature", TCODColor::blue, 100, 0));
	addCreature(std::make_shared<Creature>(Position(64, 21, 0), 'F', "Creature", TCODColor::purple, 100, 0));
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

bool World::isExplored(Position position) const
{
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
	if (fovMap->isInFov(position.x, position.y) && position.level == player->mapPosition.level)
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
