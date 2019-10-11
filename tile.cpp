#include "main.hpp"

//Tile struct
Tile::Tile(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable)
	:ch(ch), fgcol(fgcol), bgcol(bgcol), height(height), walkable(walkable), explored(false), tag("static")
{}

Tile::Tile(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, const char* tag)
	: ch(ch), fgcol(fgcol), bgcol(bgcol), height(height), walkable(walkable), explored(false), tag(tag)
{}

bool Tile::getDestroyed()
{
	return false;
}

void Tile::interact()
{
	return;
}

void Tile::render(int x, int y, const std::shared_ptr<Pane>& pane) const
{
	if (WORLD->isInFov(x, y, WORLD->player->level))
	{
		pane->console->setCharBackground(x, y, bgcol);
		pane->console->setCharForeground(x, y, fgcol);
		pane->console->setChar          (x, y, ch);
	}
	else if (WORLD->isExplored(x, y, WORLD->player->level))
	{
		pane->console->setCharBackground(x, y, TCODColor::darkestGrey);
		pane->console->setCharForeground(x, y, TCODColor::darkerGrey);
		pane->console->setChar          (x, y, ch);
	}
	else
	{
		pane->console->setCharBackground(x, y, TCODColor::black);
		pane->console->setCharForeground(x, y, TCODColor::darkerGrey);
	}
}

Destructible::Destructible(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, int strength)
	: Tile(ch, fgcol, bgcol, height, walkable, "destructible"), strength(strength), destroyed(false)
{
}

bool Destructible::getDestroyed()
{
	return destroyed;
}

void Destructible::interact()
{
	if (strength > 0)
	{
		strength--;
	}
	else
	{
		ch = '%';
		bgcol = bgcol * TCODColor::darkGrey;
		fgcol = fgcol * TCODColor::lightGrey;
		walkable = true;
		destroyed = true;
	}

}

Stair::Stair(int ch, TCODColor fgcol, TCODColor bgcol, int height, bool walkable, int moveDist)
	: Tile(ch, fgcol, bgcol, height, true, "stair"), moveDist(moveDist)
{
}

void Stair::interact()
{
	engine->gui->mapPane->world->player->level += moveDist;
}