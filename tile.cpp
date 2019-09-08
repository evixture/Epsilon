#include "main.hpp"

//Tile struct
Tile::Tile(int ch, TCODColor fgcol, TCODColor bgcol, int maxHeight, bool walkable, bool transparent)
	:ch(ch), fgcol(fgcol), bgcol(bgcol), maxHeight(maxHeight), walkable(walkable), transparent(transparent), explored(false)
{}
