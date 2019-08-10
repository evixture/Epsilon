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

Tile::~Tile()
{
}

void Tile::destroy()
{
	bgcol = destcol;
}

void Tile::dispColor()
{
}

void Tile::render()
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
				vector.push_back(std::make_shared<Tile>(TCODColor::crimson, false, false, false));
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
	player = std::make_shared<Entity>(1, 1, '@', TCODColor::blue, "player");
	entityList.push_back(player);

	//main map init
	/*for (int i = 0; i < (w * h); i++)
	{
		tileList.push_back(std::make_shared<Tile>());
	}*/

	textMap->textToVector(tileList);

	mapWin = new TCODConsole(conw, conh);
	tcodMap = std::make_shared<TCODMap>(w, h);
}

Map::~Map()
{
	delete mapWin;
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