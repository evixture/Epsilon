#include "main.hpp"

Tool::Tool(std::string name, TCODColor color, int ch)
	:name(name), color(color), ch(ch), mapPosition(Position(0, 0, 0)), dx(0), dy(0), sourcePosition(Position(0, 0, 0)), ammoType(MagazineData::AmmoType::NONE)
{}

Tool::Tool(std::string name, TCODColor color, int ch, MagazineData::AmmoType ammoType)
	:name(name), color(color), ch(ch), mapPosition(Position(0, 0, 0)), dy(0), sourcePosition(Position(0, 0, 0)), ammoType(ammoType)
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
			mapPosition.x = sourcePosition.x + 1;
			mapPosition.y = sourcePosition.y;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			mapPosition.x = sourcePosition.x + 1;
			mapPosition.y = sourcePosition.y + 1;
		}
		else if (angle >= 67.5)
		{
			mapPosition.x = sourcePosition.x;
			mapPosition.y = sourcePosition.y + 1;
		}
	}
	else if (dx >= 0 && dy <= 0)
	{
		if (angle >= -22.5)
		{
			mapPosition.x = sourcePosition.x + 1;
			mapPosition.y = sourcePosition.y;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			mapPosition.x = sourcePosition.x + 1;
			mapPosition.y = sourcePosition.y - 1;
		}
		else if (angle <= -67.5)
		{
			mapPosition.x = sourcePosition.x;
			mapPosition.y = sourcePosition.y - 1;
		}
	}
	else if (dx <= 0 && dy >= 0)
	{
		if (angle >= -22.5)
		{
			mapPosition.x = sourcePosition.x - 1;
			mapPosition.y = sourcePosition.y;
		}
		else if (angle <= -22.5 && angle >= -67.5)
		{
			mapPosition.x = sourcePosition.x - 1;
			mapPosition.y = sourcePosition.y + 1;
		}
		else if (angle <= -67.5)
		{
			mapPosition.x = sourcePosition.x;
			mapPosition.y = sourcePosition.y + 1;
		}
	}
	else if (dx <= 0 && dy <= 0)
	{
		if (angle <= 22.5)
		{
			mapPosition.x = sourcePosition.x - 1;
			mapPosition.y = sourcePosition.y;
		}
		else if (angle >= 22.5 && angle <= 67.5)
		{
			mapPosition.x = sourcePosition.x - 1;
			mapPosition.y = sourcePosition.y - 1;
		}
		else if (angle >= 67.5)
		{
			mapPosition.x = sourcePosition.x;
			mapPosition.y = sourcePosition.y - 1;
		}
	}
}

void Tool::update(int x, int y, int mx, int my, double angle)
{
	dx = mx - x;
	dy = my - y;

	sourcePosition.x = x; //check passed param
	sourcePosition.y = y;

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	updateToolPosition(angle);
}

void Tool::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(renderPosition.x, renderPosition.y, ch);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
}

//----------------------------------------------------------------------------------------------------

Bullet::Bullet(int ch, int startx, int starty, int dx, int dy, int xbound, int ybound)
	:ch(ch), startPosition(Position(startx, starty, 0)), tox(dx), toy(dy), xbound(xbound), ybound(ybound), hitWall(false), travel(BLine(startPosition.x, startPosition.y, tox, toy)), moveClock(Clock(0))
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
	} while (((tox + startPosition.x < xbound && tox + startPosition.x > 0) && tox != 0) || ((toy + startPosition.y < ybound && toy + startPosition.y > 0) && toy != 0));

	tox += startPosition.x;
	toy += startPosition.y;

	travel = BLine(startPosition.x, startPosition.y, tox, toy);
}

void Bullet::update()
{
	moveClock.capacity = (int)(engine->settings->fpsCount * .005f);
	moveClock.tickDownWithReset();

	if (moveClock.step == 0)
	{
		if (!hitWall)
		{
			if (startPosition.x < xbound && startPosition.y < ybound)
			{
				if (WORLD->inMapBounds(travel.x, travel.y, WORLD->player->mapPosition.level)
					&& WORLD->getWalkability(travel.x, travel.y, WORLD->player->mapPosition.level) != false)
				{
					travel.step();
				}
				else
				{
					if (WORLD->inMapBounds(travel.x, travel.y, WORLD->player->mapPosition.level))
					{
						if (WORLD->getTile(travel.x, travel.y, WORLD->player->mapPosition.level)->tag == Tile::Tag::DESTRUCTIBLE)
						{
							WORLD->getTile(travel.x, travel.y, WORLD->player->mapPosition.level)->interact();
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

	renderPosition = offsetPosition(Position(travel.x, travel.y, 0), WORLD->xOffset, WORLD->yOffset);
}

void Bullet::render(const std::shared_ptr<Pane>& pane) const
{
	if (!hitWall)
	{
		pane->console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::brass);
		pane->console->setChar(renderPosition.x, renderPosition.y, ch);
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
	if (!(mapPosition.x == dx + mapPosition.x && mapPosition.y == dy + mapPosition.y))
	{
		fireClock.reset();
		bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ch, mapPosition.x, mapPosition.y, dx, dy, WORLD->debugmap->width, WORLD->debugmap->height));
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
	dx = mx - x + WORLD->xOffset;
	dy = my - y + WORLD->yOffset;

	sourcePosition.x = x; //check passed param
	sourcePosition.y = y;

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

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
	pane->console->setChar(renderPosition.x, renderPosition.y, ch);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);

	for (auto& bullet : bulletList)
	{
		bullet->render(pane);
	}
}
