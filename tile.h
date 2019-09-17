#include "main.hpp"

struct Tile
{
	int ch;
	TCODColor fgcol;
	TCODColor bgcol;
	int maxHeight;

	bool explored;
	bool walkable;
	bool transparent;

	Tile(int ch, TCODColor fgcol, TCODColor bgcol, int maxHeight, bool walkable, bool transparent);
};
