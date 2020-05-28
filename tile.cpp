#include "main.hpp"

Tile::Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int deceleration, std::string name, int strength)
	:ch(ch), foregroundColor(foregroundColor), backgroundColor(backgroundColor), deceleration(deceleration), strength(strength), name(name)
{}

//----------------------------------------------------------------------------------------------------

Block::Block(std::vector<Tile> tileList, unsigned char transparentFlag, unsigned char walkableFlag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), explored(false), tag(Block::Tag::STATIC)
{}

Block::Block(std::vector<Tile> tileList, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), tag(tag), explored(false)
{}

Tile Block::getTileData(int height) const
{
	if (tileList[height].ch == 0)
	{
		for (int i = 0; i <= height; ++i) //start at player height, then move down the property list to 
		{
			if (walkableFlag & heightToBitFlag(height - i))
			{
				return tileList[height - i];
			}
		}
	}
	else if (tileList[height].ch != 0) //no tiles in tileList
	{
		return tileList[height];
	}
	return Tile('%', TCODColor::pink, TCODColor::pink, 999, "ERROR", -1);
}

bool Block::destroy(int damage, int height)
{
	if (!destroyed)
	{
		if (tileList[height].strength != -1) //if can be damaged
		{
			if (tileList[height].strength - damage >= 0)
			{
				tileList[height].strength -= damage;
			}
			else
			{
				tileList[height].strength = 0;
			}

			for (auto& tile : tileList)
			{
				if (tile.strength == 0) //if it has no strength left
				{
					destroyed = true;
					break;
				}
			}

			if (destroyed)
			{
				tileList = std::vector<Tile>
				{
					Tile('%', tileList[0].foregroundColor * TCODColor::lightGrey, tileList[0].backgroundColor * TCODColor::darkGrey, 0, "Rubble"),
					Tile(0, TCODColor::pink, TCODColor::pink, 0, "Rubble"),
					Tile(0, TCODColor::pink, TCODColor::pink, 0, "Rubble"),
					Tile(0, TCODColor::pink, TCODColor::pink, 0, "Rubble")
				};

				walkableFlag = ep::tileFlag::OOOOI;
				transparentFlag = ep::tileFlag::OOOOI;

				return true;
			}
		}
	}

	return false;
}

void Block::interact()
{
}

void Block::render(Position4 renderPosition, const Pane& pane) const
{
	//if player height it greater than tallest nontransparent tile, render the tallest tile's data, else render tile on player's floor
	Position4 position = Position4(renderPosition.x + WORLD->xOffset, renderPosition.y + WORLD->yOffset, renderPosition.height, renderPosition.floor);
	Tile tile = getTileData(renderPosition.height);

	if (WORLD->isInPlayerFov(position))
	{
		pane.console->setCharBackground(renderPosition.x,					renderPosition.y,					tile.backgroundColor);
		pane.console->setCharForeground(renderPosition.x,					renderPosition.y,					tile.foregroundColor);
		pane.console->setChar          (renderPosition.x,					renderPosition.y,					tile.ch);
	}
	else if (WORLD->isExplored(position))
	{
		pane.console->setCharBackground(renderPosition.x,					renderPosition.y,					TCODColor::black);
		pane.console->setCharForeground(renderPosition.x,					renderPosition.y,					TCODColor::darkestGrey);
		pane.console->setChar			(renderPosition.x,					renderPosition.y,					tile.ch);
	}
	else
	{
		pane.console->setCharBackground(renderPosition.x,					renderPosition.y,					TCODColor::black);
	}
}

//----------------------------------------------------------------------------------------------------

Stair::Stair(std::vector<Tile> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int moveDistance)
	:Block(tileList, transparentFlag, ep::tileFlag::OOOOI, Block::Tag::STAIR), moveDistance(moveDistance)
{}

void Stair::interact()
{
	WORLD->debugmap->player->mapPosition.floor += moveDistance;
	WORLD->debugmap->refreshFOV(WORLD->debugmap->player->mapPosition.floor);
}

