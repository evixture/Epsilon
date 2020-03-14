#include "main.hpp"

Block::Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), explored(false), tag(Block::Tag::STATIC)
{}

Block::Block(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, Tag tag)
	:tileList(tileList), transparentFlag(transparentFlag), walkableFlag(walkableFlag), tag(tag), explored(false)
{}

std::shared_ptr<Tile> Block::getTileData(int height) const
{
	if (tileList[height]->ch != 0) //no tiles in tileList
	{
		return tileList[height];
	}
	else if (tileList[height]->ch == 0)
	{
		for (int i = 0; i <= height; ++i) //start at player height, then move down the property list to 
		{
			if (walkableFlag & heightToBitFlag(height - i))
			{
				return tileList[height - i];
			}
		}
	}
	return std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999);
}

bool Block::getDestroyed()
{
	return false;
}

void Block::destroy(int)
{
}

void Block::interact()
{
}

void Block::render(Position4 renderPosition, const std::shared_ptr<Pane>& pane) const
{
	//if player height it greater than tallest nontransparent tile, render the tallest tile's data, else render tile on player's floor
	Position4 position = Position4(renderPosition.x + WORLD->xOffset, renderPosition.y + WORLD->yOffset, renderPosition.height, renderPosition.floor);
	std::shared_ptr<Tile> tile = getTileData(renderPosition.height);

		if (WORLD->isInPlayerFov(position))
		{
			pane->console->setCharBackground(renderPosition.x,					renderPosition.y,					tile->backgroundColor);
			pane->console->setCharForeground(renderPosition.x,					renderPosition.y,					tile->foregroundColor);
			pane->console->setChar          (renderPosition.x,					renderPosition.y,					tile->ch);
		}
		else if (WORLD->isExplored(position))
		{
			pane->console->setCharBackground(renderPosition.x,					renderPosition.y,					TCODColor::black);
			pane->console->setCharForeground(renderPosition.x,					renderPosition.y,					TCODColor::darkestGrey);
			pane->console->setChar			(renderPosition.x,					renderPosition.y,					tile->ch);
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

void Destructible::destroy(int damage)
{
	if (strength - damage >= 0)
	{
		strength -= damage;
	}
	else
	{
		strength = 0;
	}


	if (strength <= 0) //if it has no strength left
	{
		tileList = std::vector<std::shared_ptr<Tile>>
		{
			std::make_shared<Tile>('%', tileList[0]->foregroundColor * TCODColor::lightGrey, tileList[0]->backgroundColor * TCODColor::darkGrey, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0),
			std::make_shared<Tile>(0, TCODColor::pink, TCODColor::pink, 0)
		};

		walkableFlag = OOOOI;
		transparentFlag = OOOOI;
		destroyed = true;
	}
}

bool Destructible::getDestroyed()
{
	return destroyed;
}

//----------------------------------------------------------------------------------------------------

Stair::Stair(std::vector<std::shared_ptr<Tile>> tileList, unsigned char transparentFlag, unsigned char walkableFlag, int moveDistance)
	:Block(tileList, transparentFlag, Block::FLOOR, Block::Tag::STAIR), moveDistance(moveDistance)
{}

void Stair::interact()
{
	WORLD->debugmap->player->mapPosition.floor += moveDistance;
}

Tile::Tile(int ch, TCODColor foregroundColor, TCODColor backgroundColor, int deceleration)
	:ch(ch), foregroundColor(foregroundColor), backgroundColor(backgroundColor), deceleration(deceleration)
{}

////defined after map is created ERROR
//Block			Block::grass0 =				Block(DATA_Grass0, OOOOI, OOOOI);
//Block			Block::grass1 =				Block(DATA_Grass1, OOOOI, OOOOI);
//Block			Block::grass2 =				Block(DATA_Grass2, OOOOI, OOOOI);
//Block			Block::grass3 =				Block(DATA_Grass3, OOOOI, OOOOI);
//Block			Block::flower =				Block(DATA_BasicFlower, OOIII, OOOOI);
//Block			Block::floor =				Block(DATA_BasicFloor, OOOOI, OOOOI);
//Block			Block::concrete =			Block(DATA_BasicConcrete, OOOOI, OOOOI);
//Block			Block::shingle =			Block(DATA_BasicShingle, OOOOI, OOOOI);
//Block			Block::door =				Block(DATA_BasicDoor, IIIII, OOOOI);
//Destructible	Destructible::wall =		Destructible(DATA_BasicWall, IIIII, IIIII, 1000);
//Destructible	Destructible::window =		Destructible(DATA_BasicWindow, OOIII, IIIII, 100);
//Destructible	Destructible::tableLeg =	Destructible(DATA_BasicTableLeg, OOIII, OOIII, 500);
//Destructible	Destructible::tableTop =	Destructible(DATA_BasicTableTop, OOIOI, OOIOI, 500);
//Stair			Stair::upStair =			Stair(DATA_UpStair, OOOOI, OOIII, 1);
//Stair			Stair::downStair =			Stair(DATA_DownStair, OOOOI, OOIII, -1);
//Block			Block::sky =				Block(DATA_BasicSky, OOOOO, OOOOO);
//Block			Block::error =				Block(DATA_Error, IIIII, IIIII);

//Block			grass0 = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOI, OOOOI);
//Block			grass1 = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOI, OOOOI);
//Block			grass2 = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOI, OOOOI);
//Block			grass3 = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOI, OOOOI);
//Block			flower = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOIII, OOOOI);
//Block			floor = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOI, OOOOI);
//Block			concrete = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOI, OOOOI);
//Block			shingle = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOI, OOOOI);
//Block			door = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, IIIII, OOOOI);
//Block			sky = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOO, OOOOO);
//Block			error = Block(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, IIIII, IIIII);
//Destructible	wall = Destructible(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, IIIII, IIIII, 1000);
//Destructible	window = Destructible(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOIII, IIIII, 100);
//Destructible	tableLeg = Destructible(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOIII, OOIII, 500);
//Destructible	tableTop = Destructible(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOIOI, OOIOI, 500);
//Stair			upStair = Stair(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOI, OOIII, 1);
//Stair			downStair = Stair(std::vector<std::shared_ptr<Tile>>{std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999), std::make_shared<Tile>('%', TCODColor::pink, TCODColor::pink, 999)}, OOOOI, OOIII, -1);
