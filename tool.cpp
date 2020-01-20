#include "main.hpp"

Tool::Tool(std::string name, TCODColor color, int ch)
	:name(name), color(color), ch(ch), mapPosition(Position(0, 0, 0)), dx(0), dy(0), sourcePosition(Position(0, 0, 0)), ammoType(MagazineData::AmmoType::NONE)
{}

Tool::Tool(std::string name, TCODColor color, int ch, MagazineData::AmmoType ammoType)
	:name(name), color(color), ch(ch), mapPosition(Position(0, 0, 0)), dx(0), dy(0), sourcePosition(Position(0, 0, 0)), ammoType(ammoType)
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

void Tool::update(Position sourcePosition, int mx, int my, double angle)
{
	dx = mx - sourcePosition.x;
	dy = my - sourcePosition.y;

	this->sourcePosition = sourcePosition;

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	updateToolPosition(angle);
}

void Tool::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(renderPosition.x, renderPosition.y, ch);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
}

//----------------------------------------------------------------------------------------------------

Bullet::Bullet(int ch, Position startPosition, int dx, int dy, int xbound, int ybound)
	:ch(ch), startPosition(startPosition), tox(dx), toy(dy), xbound(xbound), ybound(ybound), hitWall(false), travel(BLine(startPosition.x, startPosition.y, tox, toy)),
	moveClock(Clock(.005f, 0.005f)), mapPosition(startPosition)
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
	//moveClock.capacity = (int)(engine->settings->fpsCount * .005f);
	//moveClock.tickDownWithReset();
	moveClock.update(true, false, false);

	mapPosition = Position(travel.x, travel.y, startPosition.level);

	if (moveClock.isAtZero())
	{
		if (!hitWall) // if it has not hit a solid wall yet
		{
			if (startPosition.x < xbound && startPosition.y < ybound) //if in map bound
			{
				if (WORLD->inMapBounds(mapPosition)
					&& WORLD->getWalkability(mapPosition, 3) != false) // if in map bounds and travel position is walkable //REPLACE WITH BULLET HEIGHT
				{
					for (auto& creature : WORLD->creatureList)
					{
						if (mapPosition == creature->mapPosition)
						{
							creature->health -= 50; //deal bullet damage, should be replaced later
							GUI->logWindow->pushMessage(LogWindow::Message("You hit a creature!", LogWindow::Message::MessageLevel::HIGH));
						}
					}

					travel.step();
				}
				else //else if bullet has hit something that is not walkable
				{
					if (WORLD->inMapBounds(mapPosition))
					{										  
						if (WORLD->getTile(mapPosition)->tag == Tile::Tag::DESTRUCTIBLE)
						{									  
							WORLD->getTile(mapPosition)->interact();
						}
						hitWall = true;
					}
					else
					{
						hitWall = true;
					}
				}
			}
			else // else not in map bound
			{
				hitWall = true;
			}
		}
		moveClock.update(false, true, false);
	}

	mapPosition = Position(travel.x, travel.y, startPosition.level); //second needed?
	//map pos assignment orig here
	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Bullet::render(const std::shared_ptr<Pane>& pane) const
{
	if (!hitWall)
	{
		if (WORLD->player->mapPosition.level == startPosition.level)
		{
			pane->console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::brass);

			if (startPosition.x == travel.x && startPosition.y == travel.y) pane->console->setChar(renderPosition.x, renderPosition.y, '*'); //muzzle flash ??
			else pane->console->setChar(renderPosition.x, renderPosition.y, ch);
		}
	}
}

//----------------------------------------------------------------------------------------------------

Firearm::Firearm(std::string name, TCODColor color, float fireRate, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag)
	:Tool(name, color, NULL, ammoType), maxFireTime(fireRate), maxReloadTime(reloadSpeed), fireMode(fireMode), availibleFireMode(availibleFireModeFlag),
	fireClock(Clock(fireRate, 0.0f)), reloadClock(Clock(reloadSpeed, 0.0f)), selectedMagazine(std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false))
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
			if (!reloadClock.isAtZero())
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
		fireClock.update(false, true, false);
		bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ch, mapPosition, dx, dy, WORLD->debugmap->width, WORLD->debugmap->height));
		selectedMagazine->availableAmmo--;
	}
}

void Firearm::reload(std::shared_ptr<MagazineData>& magazine)
{
	if (magazine->isValid != false)
	{
		selectedMagazine = magazine;
		reloadClock.update(false, true, false);
	}
	else selectedMagazine = magazine;
}

void Firearm::changeFireMode()
{
	//full->semi->safe->full
	if (fireMode == FireType::SAFE)
	{
		if (availibleFireMode & FireType::FULL) //if availible fire mode has FULL fire mode bit set
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

void Firearm::update(Position sourcePosition, int mx, int my, double angle)
{
	dx = mx - sourcePosition.x + WORLD->xOffset;
	dy = my - sourcePosition.y + WORLD->yOffset;

	this->sourcePosition = sourcePosition;

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	//fireClock.capacity = (int)(maxFireTime * SETTINGS->fpsCount);
	//reloadClock.capacity = (int)(maxReloadTime * SETTINGS->fpsCount);

	updateToolPosition(angle);

	mapPosition.level = sourcePosition.level;

	updateWeaponChar(angle);
	
	if (fireClock.isAtZero() && selectedMagazine->availableAmmo != 0 && reloadClock.isAtZero()) //fires bullet
	{
		if (fireMode == FireType::FULL && INPUT->leftMouseButton->isDown)
		{
			fireBullet();
			fireClock.update(false, true, false);
		}
		else if (fireMode == FireType::SEMI && INPUT->leftMouseButton->isSwitched)
		{
			fireBullet();
			fireClock.update(false, true, false);
		}
		else if (fireMode == FireType::SAFE)
		{

		}
	}
	//fireClock.tickDown();
	fireClock.update(true, false, false);

	if (bulletList.size() > selectedMagazine->ammoCapacity * 2) //clean up extra bullets
	{
		bulletList.pop_back();
	}

	for (auto& bullet : bulletList) //update bullets
	{
		bullet->update();
	}

	//reloadClock.tickDown();
	reloadClock.update(true, false, false);
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
