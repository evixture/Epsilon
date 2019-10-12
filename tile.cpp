#include "main.hpp"

//Tile struct
Tile::Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int height, bool walkable)
	:ch(ch), foregroundColor(foregroundColor), backgroundColor(backgroundColor), height(height), walkable(walkable), explored(false), tag("static")
{}

Tile::Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int height, bool walkable, const char* tag)
	: ch(ch), foregroundColor(foregroundColor), backgroundColor(backgroundColor), height(height), walkable(walkable), explored(false), tag(tag)
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
	if (WORLD->isInFov(x, y, WORLD->player->mapPosition.level))
	{
		pane->console->setCharBackground(x, y, backgroundColor);
		pane->console->setCharForeground(x, y, foregroundColor);
		pane->console->setChar          (x, y, ch);
	}
	else if (WORLD->isExplored(x, y, WORLD->player->mapPosition.level))
	{
		pane->console->setCharBackground(x, y, TCODColor::black);
		pane->console->setCharForeground(x, y, TCODColor::darkestGrey);
		pane->console->setChar          (x, y, ch);
	}
	else
	{
		pane->console->setCharBackground(x, y, TCODColor::black);
		//pane->console->setCharForeground(x, y, TCODColor::darkerGrey);
	}
}

Destructible::Destructible(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int height, bool walkable, int strength)
	: Tile(ch, foregroundColor, backgroundColor, height, walkable, "destructible"), strength(strength), destroyed(false)
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
		backgroundColor = backgroundColor * TCODColor::darkGrey;
		foregroundColor = foregroundColor * TCODColor::lightGrey;
		walkable = true;
		destroyed = true;
	}

}

Stair::Stair(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int height, bool walkable, int moveDist)
	: Tile(ch, foregroundColor, backgroundColor, height, true, "stair"), moveDist(moveDist)
{
}

void Stair::interact()
{
	engine->gui->mapPane->world->player->mapPosition.level += moveDist;
}