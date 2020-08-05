#include "main.hpp"

Tile::Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int deceleration, char nameID, int strength)
	: ch(ch), foregroundColor(foregroundColor), backgroundColor(backgroundColor), deceleration(deceleration), strength(strength), nameID(nameID)
{
}

//----------------------------------------------------------------------------------------------------

Block::Block(std::array<Tile, 4> tileList, unsigned char transparentFlag, unsigned char solidityFlag, Tag tag)
	: tileList(tileList), transparentFlag(transparentFlag), solidityFlag(solidityFlag), tag(tag), explored(false), destroyed(false)
{
}

Tile Block::getTileData(int h) const
{
	if (tileList[h].ch == 0)
	{
		for (int i = 0; i <= h; ++i) //start at player height, then move down the property list to 
		{
			if (solidityFlag & heightToBitFlag(h - i)) return tileList[h - i];
		}
	}
	else if (tileList[h].ch != 0) return tileList[h]; //no tiles in tileList

	return Tile('%', TCODColor::pink, TCODColor::pink, 999, -1, -1);
}

//bool Block::destroy(float velocity, float mass, int h)
//{
//	if (!destroyed)
//	{
//		if (tileList[h].strength != -1) //if can be damaged
//		{
//			if	(tileList[h].strength - damage >= 0) tileList[h].strength -= damage;
//			else tileList[h].strength = 0;
//
//			for (auto& tile : tileList)
//			{
//				if (tile.strength == 0) //if it has no strength left
//				{
//					destroyed = true;
//					break;
//				}
//			}
//
//			if (destroyed)
//			{
//				tileList = std::array<Tile, 4>
//				{
//					Tile('%', tileList[0].foregroundColor * TCODColor::lightGrey, tileList[0].backgroundColor * TCODColor::darkGrey, 0, 21),
//					Tile(0, TCODColor::pink, TCODColor::pink, 0, 21),
//					Tile(0, TCODColor::pink, TCODColor::pink, 0, 21),
//					Tile(0, TCODColor::pink, TCODColor::pink, 0, 21)
//				};
//
//				solidityFlag = ep::tileFlag::OOOOI;
//				transparentFlag = ep::tileFlag::OOOOI;
//
//				return true;
//			}
//		}
//	}
//	return false;
//}

void Block::interact()
{
	return;
}

void Block::interact(Projectile* projectile)
{
	/*
	if (!destroyed)
	{
		if (tileList[h].strength != -1) //if can be damaged
		{
			if	(tileList[h].strength - damage >= 0) tileList[h].strength -= damage;
			else tileList[h].strength = 0;

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
				tileList = std::array<Tile, 4>
				{
					Tile('%', tileList[0].foregroundColor * TCODColor::lightGrey, tileList[0].backgroundColor * TCODColor::darkGrey, 0, 21),
					Tile(0, TCODColor::pink, TCODColor::pink, 0, 21),
					Tile(0, TCODColor::pink, TCODColor::pink, 0, 21),
					Tile(0, TCODColor::pink, TCODColor::pink, 0, 21)
				};

				solidityFlag = ep::tileFlag::OOOOI;
				transparentFlag = ep::tileFlag::OOOOI;

				return true;
			}
		}
	}
	return false;
	*/

	if (!destroyed)
	{
		if (tileList[projectile->mapPosition.h].strength != -1)
		{
			if (tileList[projectile->mapPosition.h].strength - projectile->mass >= 0)
			{
				tileList[projectile->mapPosition.h].strength -= projectile->mass;
			}
			else
			{
				tileList[projectile->mapPosition.h].strength = 0;
				destroyed = true;
			}

			if (projectile->velocity < tileList[projectile->mapPosition.h].deceleration)
			{
				projectile->velocity = 0;
			}
			else
			{
				projectile->velocity -= tileList[projectile->mapPosition.h].deceleration;
			}

			if (destroyed)
			{
				tileList = std::array<Tile, 4>
				{
					Tile('%', tileList[0].foregroundColor* TCODColor::lightGrey, tileList[0].backgroundColor* TCODColor::darkGrey, 0, 21),
						Tile(0, TCODColor::pink, TCODColor::pink, 0, 21),
						Tile(0, TCODColor::pink, TCODColor::pink, 0, 21),
						Tile(0, TCODColor::pink, TCODColor::pink, 0, 21)
				};

				solidityFlag = ep::tileFlag::OOOOI;
				transparentFlag = ep::tileFlag::OOOOI;

				WORLD->updateBlock(projectile->nextPosition, false);
				AUDIO->playSound(PositionalStaticSound(("crash"), projectile->nextPosition, 85.0f, 100.0f));
			}
		}
	}
}

void Block::interact(Melee* melee)
{
	if (!destroyed)
	{
		if (tileList[melee->mapPosition.h].strength != -1)
		{
			if (tileList[melee->mapPosition.h].strength - ((melee->bluntDamage + melee->sharpDamage) / 10.0f) >= 0)
			{
				tileList[melee->mapPosition.h].strength -= ((melee->bluntDamage + melee->sharpDamage) / 10.0f);
			}
			else
			{
				tileList[melee->mapPosition.h].strength = 0;
				destroyed = true;
			}

			if (destroyed)
			{
				tileList = std::array<Tile, 4>
				{
					Tile('%', tileList[0].foregroundColor* TCODColor::lightGrey, tileList[0].backgroundColor* TCODColor::darkGrey, 0, 21),
						Tile(0, TCODColor::pink, TCODColor::pink, 0, 21),
						Tile(0, TCODColor::pink, TCODColor::pink, 0, 21),
						Tile(0, TCODColor::pink, TCODColor::pink, 0, 21)
				};

				solidityFlag = ep::tileFlag::OOOOI;
				transparentFlag = ep::tileFlag::OOOOI;

				WORLD->updateBlock(melee->mapPosition, false);
				AUDIO->playSound(PositionalStaticSound(("crash"), melee->mapPosition, 85.0f, 100.0f));
			}
		}
	}
}

void Block::render(Position4 renderPosition, const Pane& pane) const
{
	Position4 position = Position4(renderPosition.x + WORLD->xOffset, renderPosition.y + WORLD->yOffset, renderPosition.h, renderPosition.z); //change later to be optimized version
	Tile tile = getTileData(renderPosition.h);

	if (WORLD->isInPlayerFov(position))
	{
		pane.console->setCharBackground	(renderPosition.x,	renderPosition.y,	tile.backgroundColor);
		pane.console->setCharForeground	(renderPosition.x,	renderPosition.y,	tile.foregroundColor);
		pane.console->setChar			(renderPosition.x,	renderPosition.y,	tile.ch);
	}
	else if (WORLD->isExplored(position))
	{
		pane.console->setCharBackground	(renderPosition.x,	renderPosition.y,	TCODColor::black);
		pane.console->setCharForeground	(renderPosition.x,	renderPosition.y,	TCODColor::darkestGrey);
		pane.console->setChar			(renderPosition.x,	renderPosition.y,	tile.ch);
	}
	else
	{
		pane.console->setCharBackground	(renderPosition.x,	renderPosition.y,	TCODColor::black);
	}
}

//----------------------------------------------------------------------------------------------------

Stair::Stair(std::array<Tile, 4> tileList, unsigned char transparentFlag, unsigned char solidityFlag, int moveDistance)
	: Block(tileList, transparentFlag, ep::tileFlag::OOOOI, Block::Tag::STAIR), moveDistance(moveDistance)
{
}

void Stair::interact()
{
	WORLD->debugmap->player->mapPosition.z += moveDistance;
	WORLD->debugmap->refreshFOV(WORLD->debugmap->player->mapPosition.z);
}

