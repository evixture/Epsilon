#include "main.hpp"

//Tool Struct
Tool::Tool(const char* name, TCODColor color)
	:color(color), toolx(0), tooly(0), ch(NULL), dx(0), dy(0), name(name), sourcex(0), sourcey(0)
{}

void Tool::update(int x, int y, int mx, int my, double angle)
{
	ch = 249;
	dx = mx - x;
	dy = my - y;

	sourcex = x;
	sourcey = y;

	if (dx >= 0 && dy >= 0)
	{
		if (angle <= 22.5)
		{
			toolx = x + 1;
			tooly = y;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = x + 1;
			tooly = y + 1;
		}
		else if (angle >= 67.5)
		{
			toolx = x;
			tooly = y + 1;
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			toolx = x + 1;
			tooly = y;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = x + 1;
			tooly = y - 1;
		}
		else if (angle <= -67.5)
		{
			toolx = x;
			tooly = y - 1;
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			toolx = x - 1;
			tooly = y;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = x - 1;
			tooly = y + 1;
		}
		else if (angle <= -67.5)
		{
			toolx = x;
			tooly = y + 1;
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			toolx = x - 1;
			tooly = y;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = x - 1;
			tooly = y - 1;
		}
		else if (angle >= 67.5)
		{
			toolx = x;
			tooly = y - 1;
		}
	}
}

void Tool::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(toolx, tooly, ch);
	pane->console->setCharForeground(toolx, tooly, color);
}

//Bullet Struct
Bullet::Bullet(int ch, int startx, int starty, int dx, int dy, int xbound, int ybound)
	:bulletx(startx), bullety(starty), xbound(xbound), ybound(ybound), hitWall(false), tox(dx), toy(dy), travel(BLine(bulletx, bullety, tox, toy)), moveClock(Clock(0)), ch(ch)
{
	do
	{
		if (tox != 0)
		{
			tox *= 2;
		}
		if (toy != 0)
		{
			toy *= 2;
		}
	} while (((tox + bulletx < xbound && tox + bulletx > 0) && tox != 0) || ((toy + bullety < ybound && toy + bullety > 0) && toy != 0));

	tox += bulletx;
	toy += bullety;

	travel = BLine(bulletx, bullety, tox, toy);
}

void Bullet::update()
{
	moveClock.capacity = (int)(engine->settings->fpsCount * .005f);
	moveClock.tickDownWithReset();

	if (moveClock.step == 0)
	{
		if (!hitWall)
		{
			if (bulletx < xbound && bullety < ybound)
			{
				if (WORLD->inMapBounds(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, WORLD->player->mapPosition.level)
					&& WORLD->getWalkability(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, WORLD->player->mapPosition.level) != false)
				{
					travel.step();
				}
				else
				{
					if (WORLD->inMapBounds(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, WORLD->player->mapPosition.level))
					{
						if (WORLD->getTile(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, WORLD->player->mapPosition.level)->tag == "destructible")
						{
							WORLD->getTile(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, WORLD->player->mapPosition.level)->interact();
						}
						hitWall = true;
					}
					else
					{
						hitWall = true;
					}
				}
			}
			else
			{
				hitWall = true;
			}
		}
	}
}

void Bullet::render(const std::shared_ptr<Pane>& pane) const
{
	if (!hitWall)
	{
		pane->console->setCharForeground(travel.x, travel.y, TCODColor::brass);
		pane->console->setChar(travel.x, travel.y, ch);
	}
}

//Weapon Struct
Weapon::Weapon(const char* name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed)
	: Tool(name, color), baseFireCap(fireRate), fireClock(0), ammoCap(ammoCap), ammoAmount(ammoCap), numberMags(numberMags), reloadClock(0), baseReloadTimer(reloadSpeed)
{}

void Weapon::update(int x, int y, int mx, int my, double angle)
{
	dx = mx - x;
	dy = my - y;

	sourcex = x;
	sourcey = y;

	fireClock.capacity = (int)(baseFireCap * SETTINGS->fpsCount);
	reloadClock.capacity = (int)(baseReloadTimer * SETTINGS->fpsCount);

	if (dx >= 0 && dy >= 0)
	{
		if (angle <= 22.5)
		{
			toolx = x + 1;
			tooly = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = x + 1;
			tooly = y + 1;
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			toolx = x;
			tooly = y + 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			toolx = x + 1;
			tooly = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = x + 1;
			tooly = y - 1;
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			toolx = x;
			tooly = y - 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			toolx = x - 1;
			tooly = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = x - 1;
			tooly = y + 1;
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			toolx = x;
			tooly = y + 1;
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			toolx = x - 1;
			tooly = y;
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = x - 1;
			tooly = y - 1;
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			toolx = x;
			tooly = y - 1;
			ch = TCOD_CHAR_VLINE;
		}
	}

	//Fire bullet
	if (INPUT->leftMouseClick && fireClock.step == 0 && ammoAmount != 0 && reloadClock.step == 0)
	{
		if (!(toolx == dx + toolx && tooly == dy + tooly))
		{
			fireClock.reset();
			bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ch, toolx, tooly, dx, dy, WORLD->debugmap->mapWidth, WORLD->debugmap->mapHeight));
			ammoAmount--;
		}
	}
	fireClock.tickDown();

	//CLEAR BULLETS
	if (bulletList.size() > ammoCap)
	{
		bulletList.pop_back();
	}

	if (reloadClock.step == 0)
	{
		if (engine->settings->input->r->isSwitched)
		{
			if (numberMags != 0 && ammoAmount != ammoCap)
			{
				ammoAmount = ammoCap;
				numberMags--;
				reloadClock.reset();
			}
		}
	}
	reloadClock.tickDown();

	for (auto& bullet : bulletList)
	{
		bullet->update();
	}
}

void Weapon::render(const std::shared_ptr<Pane>& pane) const
{
	if (reloadClock.step == 0)
	{
		pane->console->setChar(toolx, tooly, ch);
		pane->console->setCharForeground(toolx, tooly, color);
	}

	for (auto& bullet : bulletList)
	{
		bullet->render(pane);
	}
}

//ITEM STRUCT
Item::Item(int size, std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool, Position position)
	: tile(tile), tool(tool), mapPosition(position), renderPosition(position), size(size), distToEnt(0)
{
}

void Item::updateTool(int x, int y, int mx, int my, double angle)
{
	tool->update(x, y, mx, my, angle);

	mapPosition = Position(tool->sourcex + WORLD->xOffset, tool->sourcey + WORLD->yOffset, 0);

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Item::renderTool(const std::shared_ptr<Pane>& pane) const
{
	tool->render(pane);
}

void Item::updateTile()
{
	distToEnt = getDistance(WORLD->player->mapPosition.x, WORLD->player->mapPosition.y, mapPosition.x, mapPosition.y);
	//mapPosition = Position(tool->toolx, tool->tooly, 0);

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Item::renderTile(const std::shared_ptr<Pane>& pane) const
{
	tile->render(renderPosition.x, renderPosition.y, pane);

	if (distToEnt < 5)
	{
		pane->console->setCharBackground(renderPosition.x, renderPosition.y, tile->backgroundColor + TCODColor::darkGrey);
	}
}

Container::Container(int itemCapacity, std::shared_ptr<Item> containerItem)
	:itemCapacity(itemCapacity), currentSize(0), containerItem(containerItem)
{
}

bool Container::addItem(std::shared_ptr<Item> item)
{
	int spaceUsed = 0;

	for (auto& i : itemList)
	{
		spaceUsed += i->size;
	}

	if (spaceUsed + item->size <= itemCapacity)
	{
		itemList.push_back(item);
		return true;
	}

	return false;
}
