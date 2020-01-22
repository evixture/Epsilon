#include "main.hpp"

Tile::Tile(std::vector<std::shared_ptr<TileData>> tileList, unsigned char transparentFlag, unsigned char walkableFlag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), explored(false), tag(Tile::Tag::STATIC)
{}

Tile::Tile(std::vector<std::shared_ptr<TileData>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), tag(tag), explored(false)
{}

std::shared_ptr<TileData> Tile::getTileData(int height) const
{
	//if (walkableFlag & heightToBitFlag(height))
	//{
		//if (transparentFlag & heightToBitFlag(height))
		//{
		//	return tileList[height];
		//}
		if (tileList[height]->ch != 0)
		{
			return tileList[height];
		}
	//}
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
	return std::make_shared<TileData>('%', TCODColor::pink, TCODColor::pink);
}

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
		if (WORLD->isInFov(Position(renderPosition.x + WORLD->xOffset, renderPosition.y + WORLD->yOffset, WORLD->player->mapPosition.level))) 
		{
			pane->console->setCharBackground(renderPosition.x, renderPosition.y, getTileData(renderPosition.level)->backgroundColor);
			pane->console->setCharForeground(renderPosition.x, renderPosition.y, getTileData(renderPosition.level)->foregroundColor);
			pane->console->setChar          (renderPosition.x, renderPosition.y, getTileData(renderPosition.level)->ch);
		}
		else if (WORLD->isExplored(Position(renderPosition.x + WORLD->xOffset, renderPosition.y + WORLD->yOffset, WORLD->player->mapPosition.level)))
		{
			pane->console->setCharBackground(renderPosition.x, renderPosition.y, TCODColor::black);
			pane->console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::darkestGrey);
			pane->console->setChar(renderPosition.x, renderPosition.y, getTileData(renderPosition.level)->ch);
		}
		else
		{
			pane->console->setCharBackground(renderPosition.x, renderPosition.y, TCODColor::black);
		}
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
