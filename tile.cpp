#include "main.hpp"

Tile::Tile(std::vector<std::shared_ptr<TileData>> tileList, unsigned char transparentFlag, unsigned char walkableFlag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), explored(false), tag(Tile::Tag::STATIC)
{}

Tile::Tile(std::vector<std::shared_ptr<TileData>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), tag(tag), explored(false)
{}

bool Tile::getDestroyed()
{
	return false;
}

void Tile::interact()
{
	return;
}

void Tile::render(Position renderPosition, const std::shared_ptr<Pane>& pane) const
{
	//if player height it greater than tallest nontransparent tile, render the tallest tile's data, else render tile on player's level
	if (transparentFlag & heightToBitFlag(WORLD->player->height))
	{
		if (WORLD->isInFov(Position(renderPosition.x + WORLD->xOffset, renderPosition.y + WORLD->yOffset, WORLD->player->mapPosition.level))) 
		{
			pane->console->setCharBackground(renderPosition.x, renderPosition.y, tileList[renderPosition.level]->backgroundColor);
			pane->console->setCharForeground(renderPosition.x, renderPosition.y, tileList[renderPosition.level]->foregroundColor);
			pane->console->setChar          (renderPosition.x, renderPosition.y, tileList[renderPosition.level]->ch);
		}
		else if (WORLD->isExplored(Position(renderPosition.x + WORLD->xOffset, renderPosition.y + WORLD->yOffset, WORLD->player->mapPosition.level)))
		{
			pane->console->setCharBackground(renderPosition.x, renderPosition.y, TCODColor::black);
			pane->console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::darkestGrey);
			pane->console->setChar(renderPosition.x, renderPosition.y, tileList[renderPosition.level]->ch);
		}
		else
		{
			pane->console->setCharBackground(renderPosition.x, renderPosition.y, TCODColor::black);
		}

	}
	//else
	//{
	//	for (int i = 0;)
	//}
}

//----------------------------------------------------------------------------------------------------

Destructible::Destructible(std::vector<std::shared_ptr<TileData>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int strength)
	:Tile(tileList, transparentFlag, walkableFlag, Tile::Tag::DESTRUCTIBLE), strength(strength), destroyed(false)
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
		tileList = std::vector<std::shared_ptr<TileData>>
		{
			std::make_shared<TileData>(' ', TCODColor::pink, TCODColor::pink),
			std::make_shared<TileData>(' ', TCODColor::pink, TCODColor::pink),
			std::make_shared<TileData>(' ', TCODColor::pink, TCODColor::pink),
			std::make_shared<TileData>(' ', TCODColor::pink, TCODColor::pink),
			std::make_shared<TileData>('%', tileList[3]->foregroundColor * TCODColor::lightGrey, tileList[3]->backgroundColor * TCODColor::darkGrey)
		};

		//ch = '%';
		//backgroundColor = backgroundColor * TCODColor::darkGrey;
		//foregroundColor = foregroundColor * TCODColor::lightGrey;
		walkableFlag = OOOOI;
		transparentFlag = OOOOI;
		destroyed = true;
	}
}

//----------------------------------------------------------------------------------------------------

Stair::Stair(std::vector<std::shared_ptr<TileData>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int moveDistance)
	:Tile(tileList, transparentFlag, Tile::FLOOR, Tile::Tag::STAIR), moveDistance(moveDistance)
{}

void Stair::interact()
{
	WORLD->player->mapPosition.level += moveDistance;
}

TileData::TileData(int ch, TCODColor foregroundColor, TCODColor backgroundColor)
	:ch(ch), foregroundColor(foregroundColor), backgroundColor(backgroundColor)
{}
