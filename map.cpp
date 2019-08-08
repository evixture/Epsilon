#include "main.hpp"

Tile::Tile()
	:bgcol(TCODColor::pink), destcol(TCODColor::green), blocksMove(false), blocksLight(false), destructible(false)
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

Map::Map(int conw, int conh, int w, int h)
{
	for (int i = w * h; i < w * h; i++);
	{
		tiles.push_back(std::make_shared<Tile>());
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
	return !tiles[x + y * w]->blocksMove;
}

bool Map::blocksLight(int x, int y)
{
	return tiles[x + y * w]->blocksLight;
}

void Map::render()
{
}