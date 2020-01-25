#include "main.hpp"

Block::Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), explored(false), tag(Block::Tag::STATIC)
{}

Block::Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), tag(tag), explored(false)
{}

std::shared_ptr<Tile> Block::getTileData(int height) const
{
	if (tileList[height]->ch != 0)
	{
		return tileList[height];
	}
	else if (tileList[height]->ch == 0)
	{
		for (int i = 0; i <= height; i++) //start at player height, then move down the property list to 
		{
			if (walkableFlag & heightToBitFlag(height - i))
			{
				//if (transparentFlag & heightToBitFlag(height - i))
				//{
					return tileList[height - i];
				//}
			}
		}
	}
	return std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999);
}

bool Block::getDestroyed()
{
	return false;
}

void Block::interact()
{
	return;
}

void Block::render(Position4 renderPosition, const std::shared_ptr<Pane>& pane) const
{
	//if player height it greater than tallest nontransparent tile, render the tallest tile's data, else render tile on player's level
		if (WORLD->isInFov(Position3(renderPosition.x + WORLD->xOffset, renderPosition.y + WORLD->yOffset, renderPosition.level)))
		{
			pane->console->setCharBackground(renderPosition.x,					renderPosition.y,					getTileData(renderPosition.height)->backgroundColor);
			pane->console->setCharForeground(renderPosition.x,					renderPosition.y,					getTileData(renderPosition.height)->foregroundColor);
			pane->console->setChar          (renderPosition.x,					renderPosition.y,					getTileData(renderPosition.height)->ch);
		}
		else if (WORLD->isExplored(Position3(renderPosition.x + WORLD->xOffset, renderPosition.y + WORLD->yOffset,	renderPosition.level)))
		{
			pane->console->setCharBackground(renderPosition.x,					renderPosition.y,					TCODColor::black);
			pane->console->setCharForeground(renderPosition.x,					renderPosition.y,					TCODColor::darkestGrey);
			pane->console->setChar			(renderPosition.x,					renderPosition.y,					getTileData(renderPosition.height)->ch);
		}
		else
		{
			pane->console->setCharBackground(renderPosition.x,					renderPosition.y,					TCODColor::black);
		}
	}
	

//----------------------------------------------------------------------------------------------------

Destructible::Destructible(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int strength)
	:Block(tileList, transparentFlag, walkableFlag, Block::Tag::DESTRUCTIBLE), strength(strength), destroyed(false)
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
		tileList = std::vector<std::shared_ptr<Tile>>
		{
			//std::make_shared<Tile>('%', tileList[0]->foregroundColor * TCODColor::lightGrey, tileList[0]->backgroundColor * TCODColor::darkGrey),
			std::make_shared<Tile>('%', tileList[0]->foregroundColor * TCODColor::lightGrey, tileList[0]->backgroundColor * TCODColor::darkGrey, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};

		
		walkableFlag = OOOOI;
		transparentFlag = OOOOI;
		destroyed = true;
	}
}

//----------------------------------------------------------------------------------------------------

Stair::Stair(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int moveDistance)
	:Block(tileList, transparentFlag, Block::FLOOR, Block::Tag::STAIR), moveDistance(moveDistance)
{}

void Stair::interact()
{
	WORLD->player->mapPosition.level += moveDistance;
}

Tile::Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int deceleration)
	:ch(ch), foregroundColor(foregroundColor), backgroundColor(backgroundColor), deceleration(deceleration)
{}
