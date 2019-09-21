#include "main.hpp"

//Tile struct
Tile::Tile(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, bool transparent)
	:ch(ch), fgcol(fgcol), bgcol(bgcol), height(height), walkable(walkable), transparent(transparent), explored(false)
{}
