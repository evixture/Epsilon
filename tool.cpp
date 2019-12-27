#include "main.hpp"

Tool::Tool(std::string name, TCODColor color, int ch)
	:name(name), color(color), ch(ch), toolx(0), tooly(0), dx(0), dy(0), sourcex(0), sourcey(0), ammoType(MagazineData::AmmoType::NONE)
{}

Tool::Tool(std::string name, TCODColor color, int ch, MagazineData::AmmoType ammoType)
	:name(name), color(color), ch(ch), toolx(0), tooly(0), dx(0), dy(0), sourcex(0), sourcey(0), ammoType(ammoType)
{}

std::shared_ptr<MagazineData> Tool::getMagData()
{
	return std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false);
}

void Tool::reload(std::shared_ptr<MagazineData>& magazine)
{}

void Tool::changeFireMode()
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

Bullet::Bullet(int ch, int startx, int starty, int dx, int dy, int xbound, int ybound)
	:ch(ch), bulletx(startx), bullety(starty), tox(dx), toy(dy), xbound(xbound), ybound(ybound), hitWall(false), travel(BLine(bulletx, bullety, tox, toy)), moveClock(Clock(0))
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
						if (WORLD->getTile(travel.x + WORLD->xOffset, travel.y + WORLD->yOffset, WORLD->player->mapPosition.level)->tag == Tile::Tag::DESTRUCTIBLE)
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

//----------------------------------------------------------------------------------------------------

Firearm::Firearm(std::string name, TCODColor color, float fireRate, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode,char availibleFireModeFlag)
	:Tool(name, color, NULL, ammoType), maxFireTime(fireRate), maxReloadTime(reloadSpeed), fireMode(fireMode), availibleFireMode(availibleFireModeFlag), fireClock(0), reloadClock(0), selectedMagazine(std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false))
{}

void Firearm::updateWeaponChar(double angle)
{
	/*
	02|01
	--|--
	03|04
	*/
	
	if (dx >= 0 && dy >= 0) //04
	{
		if (angle <= 22.5)
		{
			if (reloadClock.step != 0)
			{
				ch = '/';
			}
			else
			{
				ch = TCOD_CHAR_HLINE;
			}
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			if (reloadClock.step != 0)
			{
				ch = TCOD_CHAR_HLINE;
			}
			else
			{
				ch = '\\';
			}
		}
		else if (angle >= 67.5)
		{
			if (reloadClock.step != 0)
			{
				ch = '\\';
			}
			else
			{
				ch = TCOD_CHAR_VLINE;
			}
		}
	}
	else if (dx >= 0 && dy <= 0) //01
	{
		if (angle >= -22.5)
		{
			if (reloadClock.step != 0)
			{
				ch = '/';
			}
			else
			{
				ch = TCOD_CHAR_HLINE;
			}
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			if (reloadClock.step != 0)
			{
				ch = TCOD_CHAR_VLINE;
			}
			else
			{
				ch = '/';
			}
		}
		else if (angle <= -67.5)
		{
			if (reloadClock.step != 0)
			{
				ch = '\\';
			}
			else
			{
				ch = TCOD_CHAR_VLINE;
			}
		}
	}
	else if (dx <= 0 && dy >= 0) //02
	{
		if (angle >= -22.5)
		{
			if (reloadClock.step != 0)
			{
				ch = '/';
			}
			else
			{
				ch = TCOD_CHAR_HLINE;
			}
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			if (reloadClock.step != 0)
			{
				ch = TCOD_CHAR_VLINE;
			}
			else
			{
				ch = '/';
			}
		}
		else if (angle <= -67.5)
		{
			if (reloadClock.step != 0)
			{
				ch = '\\';
			}
			else
			{
				ch = TCOD_CHAR_VLINE;
			}
		}
	}
	else if (dx <= 0 && dy <= 0) //03
	{
		if (angle <= 22.5)
		{
			if (reloadClock.step != 0)
			{
				ch = '/';
			}
			else
			{
				ch = TCOD_CHAR_HLINE;
			}
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			if (reloadClock.step != 0)
			{
				ch = TCOD_CHAR_HLINE;
			}
			else
			{
				ch = '\\';
			}
		}
		else if (angle >= 67.5)
		{
			if (reloadClock.step != 0)
			{
				ch = '\\';
			}
			else
			{
				ch = TCOD_CHAR_VLINE;
			}
		}
	}
}

std::shared_ptr<MagazineData> Firearm::getMagData()
{
	return selectedMagazine;
}

void Firearm::fireBullet()
{
	if (!(toolx == dx + toolx && tooly == dy + tooly))
	{
		fireClock.reset();
		bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ch, toolx, tooly, dx, dy, WORLD->debugmap->mapWidth, WORLD->debugmap->mapHeight));
		selectedMagazine->availableAmmo--;
	}
}

void Firearm::reload(std::shared_ptr<MagazineData>& magazine)
{
	if (magazine->isValid != false)
	{
		selectedMagazine = magazine;
		reloadClock.reset();
	}
	else selectedMagazine = magazine;
}

void Firearm::changeFireMode()
{
	//full->semi->safe->full
	if (fireMode == FireType::SAFE)
	{
		if (availibleFireMode & FireType::FULL)
		{
			fireMode = FireType::FULL;
		}
		else if (availibleFireMode & FireType::SEMI)
		{
			fireMode = FireType::SEMI;
		}
	}
	else if (fireMode == FireType::SEMI)
	{
		if (availibleFireMode & FireType::SAFE)
		{
			fireMode = FireType::SAFE;
		}
		else if (availibleFireMode & FireType::FULL)
		{
			fireMode = FireType::FULL;
		}
	}
	else if (fireMode == FireType::FULL)
	{
		if (availibleFireMode & FireType::SEMI)
		{
			fireMode = FireType::SEMI;
		}
		else if (availibleFireMode & FireType::SAFE)
		{
			fireMode = FireType::SAFE;
		}
	}
}

void Firearm::update(int x, int y, int mx, int my, double angle)
{
	dx = mx - x;
	dy = my - y;

	sourcex = x;
	sourcey = y;

	fireClock.capacity = (int)(maxFireTime * SETTINGS->fpsCount);
	reloadClock.capacity = (int)(maxReloadTime * SETTINGS->fpsCount);

	updateToolPosition(angle);
	updateWeaponChar(angle);
	
	if (fireClock.step == 0 && selectedMagazine->availableAmmo != 0 && reloadClock.step == 0) //fires bullet
	{
		if (fireMode == FireType::FULL && INPUT->leftMouseButton->isDown)
		{
			fireBullet();
		}
		else if (fireMode == FireType::SEMI && INPUT->leftMouseButton->isSwitched)
		{
			fireBullet();
		}
		else if (fireMode == FireType::SAFE)
		{

		}
	}
	fireClock.tickDown();

	if (bulletList.size() > selectedMagazine->ammoCapacity * 2) //clean up extra bullets
	{
		bulletList.pop_back();
	}

	for (auto& bullet : bulletList) //update bullets
	{
		bullet->update();
	}

	reloadClock.tickDown();
}

void Firearm::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(toolx, tooly, ch);
	pane->console->setCharForeground(toolx, tooly, color);

	for (auto& bullet : bulletList)
	{
		bullet->render(pane);
	}
}
