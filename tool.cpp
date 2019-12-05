#include "main.hpp"

//Tool Struct
Tool::Tool(const char* name, TCODColor color, int ch)
	:color(color), toolx(0), tooly(0), ch(ch), dx(0), dy(0), name(name), sourcex(0), sourcey(0)
{}

void Tool::updateToolPosition(double angle)
{
	if (dx >= 0 && dy >= 0)
	{
		if (angle <= 22.5)
		{
			toolx = sourcex + 1;
			tooly = sourcey;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = sourcex + 1;
			tooly = sourcey + 1;
		}
		else if (angle >= 67.5)
		{
			toolx = sourcex;
			tooly = sourcey + 1;
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			toolx = sourcex + 1;
			tooly = sourcey;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = sourcex + 1;
			tooly = sourcey - 1;
		}
		else if (angle <= -67.5)
		{
			toolx = sourcex;
			tooly = sourcey - 1;
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			toolx = sourcex - 1;
			tooly = sourcey;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			toolx = sourcex - 1;
			tooly = sourcey + 1;
		}
		else if (angle <= -67.5)
		{
			toolx = sourcex;
			tooly = sourcey + 1;
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			toolx = sourcex - 1;
			tooly = sourcey;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			toolx = sourcex - 1;
			tooly = sourcey - 1;
		}
		else if (angle >= 67.5)
		{
			toolx = sourcex;
			tooly = sourcey - 1;
		}
	}
}

void Tool::update(int x, int y, int mx, int my, double angle)
{
	dx = mx - x;
	dy = my - y;

	sourcex = x;
	sourcey = y;

	updateToolPosition(angle);
}

void Tool::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(toolx, tooly, ch);
	pane->console->setCharForeground(toolx, tooly, color);
}

//----------------------------------------------------------------------------------------------------

//Bullet Struct
Bullet::Bullet(int ch, int startx, int starty, int dx, int dy, int xbound, int ybound)
	:bulletx(startx), bullety(starty), xbound(xbound), ybound(ybound), hitWall(false), tox(dx), toy(dy), travel(BLine(bulletx, bullety, tox, toy)), moveClock(Clock(0)), ch(ch)//, renderPosition(Position(startx, starty, WORLD->player->mapPosition.level))
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
						if (WORLD->getTile(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, WORLD->player->mapPosition.level)->tag == Tile::DESTRUCTIBLE)
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

	//renderPosition = offsetPosition(Position(travel.x, travel.y, WORLD->player->mapPosition.level), WORLD->xOffset, WORLD->yOffset);
}

void Bullet::render(const std::shared_ptr<Pane>& pane) const
{
	if (!hitWall)
	{
		pane->console->setCharForeground(travel.x, travel.y, TCODColor::brass);
		pane->console->setChar(travel.x, travel.y, ch);
	}
}

//----------------------------------------------------------------------------------------------------

//Weapon Struct
Weapon::Weapon(const char* name, TCODColor color, int ammoCap, int numberMags, float fireRate, float reloadSpeed, FireType fireType)
	: Tool(name, color, NULL), baseFireCap(fireRate), fireClock(0), ammoCap(ammoCap), ammoAmount(ammoCap), numberMags(numberMags), reloadClock(0), baseReloadTimer(reloadSpeed), fireType(fireType)
{}

void Weapon::updateWeaponChar(double angle)
{
	if (dx >= 0 && dy >= 0)
	{
		if (angle <= 22.5)
		{
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			ch = '/';
		}
		else if (angle <= -67.5)
		{
			ch = TCOD_CHAR_VLINE;
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			ch = TCOD_CHAR_HLINE;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			ch = '\\';
		}
		else if (angle >= 67.5)
		{
			ch = TCOD_CHAR_VLINE;
		}
	}
}

void Weapon::fireBullet()
{
	if (!(toolx == dx + toolx && tooly == dy + tooly))
	{
		fireClock.reset();
		bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ch, toolx, tooly, dx, dy, WORLD->debugmap->mapWidth, WORLD->debugmap->mapHeight));
		ammoAmount--;
	}
}

void Weapon::reload()
{
	if (numberMags != 0 && ammoAmount != ammoCap)
	{
		ammoAmount = ammoCap;
		numberMags--;
		reloadClock.reset();
	}
}

void Weapon::update(int x, int y, int mx, int my, double angle)
{
	dx = mx - x;
	dy = my - y;

	sourcex = x;
	sourcey = y;

	fireClock.capacity = (int)(baseFireCap * SETTINGS->fpsCount);
	reloadClock.capacity = (int)(baseReloadTimer * SETTINGS->fpsCount);

	updateToolPosition(angle);
	updateWeaponChar(angle);
	

	//Fire bullet
	if (fireClock.step == 0 && ammoAmount != 0 && reloadClock.step == 0)
	{
		if (fireType == Weapon::FULL && INPUT->leftMouseButton->isDown)
		{
			fireBullet();
		}
		else if (fireType == Weapon::SEMI && INPUT->leftMouseButton->isSwitched)
		{
			fireBullet();
		}
		else if (fireType == Weapon::SAFE)
		{

		}
	}
	fireClock.tickDown();

	//CLEAR BULLETS
	if (bulletList.size() > ammoCap * 2)
	{
		bulletList.pop_back();
	}

	if (reloadClock.step == 0)
	{
		if (engine->settings->input->r->isSwitched)
		{
			reload();
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

//----------------------------------------------------------------------------------------------------


