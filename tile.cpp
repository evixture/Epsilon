#include "main.hpp"

Tile::Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, unsigned char transparentFlag, unsigned char walkableFlag)
	:ch(ch), foregroundColor(foregroundColor), backgroundColor(backgroundColor), transparentFlag(transparentFlag), walkableFlag(walkableFlag), explored(false), tag(Tile::Tag::STATIC)
{}

Tile::Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag)
	:ch(ch), foregroundColor(foregroundColor), backgroundColor(backgroundColor), transparentFlag(transparentFlag), walkableFlag(walkableFlag), tag(tag), explored(false)
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
	if (WORLD->isInFov(Position(x + WORLD->xOffset, y + WORLD->yOffset, WORLD->player->mapPosition.level))) 
	{
		pane->console->setCharBackground(x, y, backgroundColor);
		pane->console->setCharForeground(x, y, foregroundColor);
		pane->console->setChar          (x, y, ch);
	}
	else if (WORLD->isExplored(Position(x + WORLD->xOffset, y + WORLD->yOffset, WORLD->player->mapPosition.level)))
	{
		pane->console->setCharBackground(x, y, TCODColor::black);
		pane->console->setCharForeground(x, y, TCODColor::darkestGrey);
		pane->console->setChar          (x, y, ch);
	}
	else
	{
		pane->console->setCharBackground(x, y, TCODColor::black);
	}
}

//----------------------------------------------------------------------------------------------------

Destructible::Destructible(int ch, TCODColor foregroundColor, TCODColor backgroundColor, unsigned char transparentFlag, unsigned char walkableFlag, int strength)
	:Tile(ch, foregroundColor, backgroundColor, transparentFlag, walkableFlag, Tile::Tag::DESTRUCTIBLE), strength(strength), destroyed(false)
{}

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
		walkableFlag = OOOOI;
		transparentFlag = OOOOI;
		destroyed = true;
	}
}

//----------------------------------------------------------------------------------------------------

Stair::Stair(int ch, TCODColor foregroundColor, TCODColor backgroundColor, unsigned char transparentFlag, unsigned char walkableFlag, int moveDistance)
	:Tile(ch, foregroundColor, backgroundColor, transparentFlag, Tile::FLOOR, Tile::Tag::STAIR), moveDistance(moveDistance)
{}

void Stair::interact()
{
	WORLD->player->mapPosition.level += moveDistance;
}