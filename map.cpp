#include "main.hpp"

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

TextMap::TextMap(const char* filePath)
	:filePath(filePath)
{
}

//need to return
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
			}
		}
	}
}

Map::Map(int conw, int conh, int w, int h)
	:conw(conw), conh(conh), mapw(w), maph(h)
{
	for (int i = 0; i < (w * h); i++)
	{
		tileList.push_back(std::make_shared<Tile>());
	}
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
			mapWin->setCharBackground(x, y, TCODColor::pink);
		}
	}

	//test rendering
	//mapWin->printf(1, 1, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
	//mapWin->printf(1, 2, "The quick brown fox jumps over the lazy dog");

	TCODConsole::blit(mapWin, 0, 0, conw, conh, TCODConsole::root, 1, 1);
}