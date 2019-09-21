#include "main.hpp"

struct Tile
{
	int ch;
	int height;

	TCODColor fgcol;
	TCODColor bgcol;

	bool explored;
	bool walkable;
	bool transparent;

	Tile(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, bool transparent);
};
