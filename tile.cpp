#include "main.hpp"

#define WORLD engine->gui->mapPane->world

//Tile struct
Tile::Tile(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, bool transparent)
	:ch(ch), fgcol(fgcol), bgcol(bgcol), height(height), walkable(walkable), transparent(transparent), explored(false)
{}


void Tile::render(int x, int y, std::shared_ptr<Pane> pane)
{
	if (WORLD->isInFov(x, y, WORLD->player->level))
	{
		pane->console->setCharBackground(x, y, WORLD->getBgColor(x, y, WORLD->player->level));
		pane->console->setCharForeground(x, y, WORLD->getFgColor(x, y, WORLD->player->level));
		pane->console->setChar          (x, y, WORLD->getCh(x, y, WORLD->player->level));
	}
	else if (WORLD->isExplored(x, y, WORLD->player->level))
	{
		pane->console->setCharBackground(x, y, TCODColor::darkestGrey);
		pane->console->setCharForeground(x, y, TCODColor::darkerGrey);
		pane->console->setChar          (x, y, WORLD->getCh(x, y, WORLD->player->level));
	}
	else
	{
		pane->console->setCharBackground(x, y, TCODColor::black);
		pane->console->setCharForeground(x, y, TCODColor::darkerGrey);
	}
}