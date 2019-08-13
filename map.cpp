#include "main.hpp"

//Tile class
Tile::Tile()
	:bgcol(TCODColor::pink), destcol(TCODColor::green), blocksMove(false), blocksLight(false), destructible(false)
{
}

Tile::Tile(TCODColor bgcol, bool blocksMove, bool blocksLight, bool destructible)
	: bgcol(bgcol), destcol(TCODColor::green), blocksMove(blocksMove), blocksLight(blocksLight), destructible(destructible)
{
}

void Tile::destroy()
{
	bgcol = destcol;
}

//tilemap struct
TileMap::TileMap()
{
	//color, blocks move, blocks light, destructible

	tileMap.insert(std::make_pair
	("grass", Tile(TCODColor::darkestGreen, false, false, false)));
	tileMap.insert(std::make_pair
	("defWall", Tile(TCODColor::TCODColor::lighterSepia, false, false, false)));
}

TileMap::~TileMap()
{
}

//Text Map Class
TextMap::TextMap(const char* filePath)
	:filePath(filePath)
{
}

TextMap::~TextMap()
{
}

//creates map
void TextMap::textToVector(std::vector<std::shared_ptr<Tile>>& vector)
{
	std::ifstream mapFile;
	mapFile.open("data/maps/testmap.txt");

	if (mapFile.is_open())
	{
		while (!mapFile.eof())
		{
			switch (mapFile.get())
			{
			case '.':
				vector.push_back(std::make_shared<Tile>(TCODColor(0, 41, 41), false, false, false));
				break;
			case 'W':
				vector.push_back(std::make_shared<Tile>(TCODColor(63, 63, 63), true, false, false));
				break;
			default:
				break;
			}
		}
	}
}

//Map Class
Map::Map(int conw, int conh, int w, int h)
	:conw(conw), conh(conh), mapw(w), maph(h)
{
	//player init
	player = std::make_shared<Player>(1, 1);
	entityList.push_back(player);

	//main map init
	textMap->textToVector(tileList);

	mapWin = new TCODConsole(conw, conh);
	tcodMap = std::make_shared<TCODMap>(w, h);
}

Map::~Map()
{
	delete mapWin;
}

void Map::createMap()
{
	for (int x = 0; x < mapw; x++)
	{
		for (int y = 0; y < maph; y++)
		{
			tcodMap->setProperties(x, y, blocksLight(x, y), canWalk(x, y));
		}
	}
}

void Map::setWall(int x, int y)
{
}

void Map::setWindow(int x, int y)
{
}

void Map::setDest(int x, int y)
{
}

bool Map::canWalk(int x, int y)
{
	return !tileList[x + y * mapw]->blocksMove;
}

bool Map::blocksLight(int x, int y)
{
	return tileList[x + y * mapw]->blocksLight;
}

void Map::render()
{
	mapWin->setDefaultBackground(TCODColor::white);
	mapWin->clear();

	for (int x = 0; x < mapw; x++)
	{
		for (int y = 0; y < maph; y++)
		{
			mapWin->setCharBackground(x, y, tileList[x + y * mapw]->bgcol);
		}
	}

	for (auto entity : entityList)
	{
		entity->render(mapWin);
	}

	TCODConsole::blit(mapWin, 0, 0, conw, conh, TCODConsole::root, 1, 1);
}