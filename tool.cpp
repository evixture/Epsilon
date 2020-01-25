#include "main.hpp"

Tool::Tool(std::string name, TCODColor color, int ch)
	:name(name), color(color), ch(ch), mapPosition(Position4(0, 0, 0, 0)), dx(0), dy(0), sourcePosition(Position4(0, 0, 0, 0)), ammoType(MagazineData::AmmoType::NONE), fireMode(SAFE), availibleFireMode(0)
{}

Tool::Tool(std::string name, TCODColor color, int ch, MagazineData::AmmoType ammoType)
	:name(name), color(color), ch(ch), mapPosition(Position4(0, 0, 0, 0)), dx(0), dy(0), sourcePosition(Position4(0, 0, 0, 0)), ammoType(ammoType), fireMode(SAFE), availibleFireMode(0)
{}

Tool::Tool(std::string name, TCODColor color, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag)
	: name(name), color(color), ch(NULL), mapPosition(Position4(0, 0, 0, 0)), dx(0), dy(0), sourcePosition(Position4(0, 0, 0, 0)), ammoType(ammoType), fireMode(fireMode), availibleFireMode(availibleFireModeFlag)
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

void Tool::update(Position4 sourcePosition, int mx, int my, double angle)
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

Bullet::Bullet(int ch, Position4 startPosition, int dx, int dy, int xbound, int ybound, int velocity, int mass)
	:ch(ch), startPosition(startPosition), tox(dx), toy(dy), xbound(xbound), ybound(ybound), hitWall(false), travel(BLine(startPosition.x, startPosition.y, tox, toy)),
	 mapPosition(startPosition), mass(mass), baseVelocity(velocity), currentVelocity(velocity), moveClock(Clock(float(1.0f / velocity), float(1.0f / velocity))), fallClock(Clock(getFallTime(mapPosition.height) - getFallTime(mapPosition.height - 1), getFallTime(mapPosition.height) - getFallTime(mapPosition.height - 1)))
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
	moveClock.update(true, false, false);
	fallClock.update(true, false, false);

	if (currentVelocity > 0 && mapPosition.height > 0)
	{
		if (moveClock.isAtZero()) //change to use position after step to pass through and then set map position to step position if good
		{
			if (WORLD->inMapBounds(mapPosition))
			{
				if (WORLD->getSolidity(mapPosition) == true)
				{
					if (WORLD->getTile(mapPosition)->tag == Block::Tag::DESTRUCTIBLE)
					{									  
						WORLD->getTile(mapPosition)->interact();
					}

					if (currentVelocity - WORLD->getTile(mapPosition)->getTileData(mapPosition.height)->deceleration <= 0)
					{
						currentVelocity = 0;
					}
					else
					{
						currentVelocity -= WORLD->getTile(mapPosition)->getTileData(mapPosition.height)->deceleration;
					}
					
					GUI->logWindow->pushMessage(LogWindow::Message("You hit a wall!", LogWindow::Message::MessageLevel::HIGH));
				}
				else if (WORLD->getSolidity(mapPosition) == false)
				{
					for (auto& creature : WORLD->creatureList)
					{
						if (creature->mapPosition == mapPosition) //also checks height, may give bad results
						{
							if (creature->health > 0)
							{
								int damage = int(float(currentVelocity / (baseVelocity * 2.0f)) * mass); //replace with creature take damage function
								creature->health -= damage; //deal bullet damage, shoudl replace with deal damage function of creature

								if (currentVelocity - 100 < 0) //ballistics
								{
									currentVelocity = 0;
								}
								else currentVelocity -= 100;
							}

							GUI->logWindow->pushMessage(LogWindow::Message("You hit a creature!", LogWindow::Message::MessageLevel::HIGH));
						}
					}
				}
				travel.step();
				moveClock.update(false, true, false);
			}
			else
			{
				currentVelocity = 0;
				mapPosition.height = 0;
			}
		}
		if (fallClock.isAtZero())
		{
			mapPosition.height--;
			fallClock.capacity = getFallTime(mapPosition.height) - getFallTime(mapPosition.height - 1);

			fallClock.update(false, true, false);
		}

		moveClock.capacity = 1.0f / currentVelocity;

	}

	mapPosition = Position4(travel.x, travel.y, mapPosition.height, startPosition.level); //second needed?
	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Bullet::render(const std::shared_ptr<Pane>& pane) const
{
	if (WORLD->player->mapPosition.level == startPosition.level)
	{
		if (currentVelocity > 0)
		{
			if (mapPosition.height > 0)
			{
				pane->console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::brass);

				if ((startPosition.x == travel.x && startPosition.y == travel.y))
				{
					pane->console->setChar(renderPosition.x, renderPosition.y, '*'); //muzzle flash ??
				}
				else
				{
					pane->console->setChar(renderPosition.x, renderPosition.y, ch);
				}
			}
			else
			{
				if (WORLD->getTile(mapPosition)->getTileData(mapPosition.height)->ch != 0)
				{
					WORLD->getTile(mapPosition)->getTileData(mapPosition.height)->ch = ch; //move to update
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------

Firearm::Firearm(std::string name, TCODColor color, float fireRate, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag)
	:Tool(name, color, ammoType, fireMode, availibleFireModeFlag), maxFireTime(fireRate), maxReloadTime(reloadSpeed),
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
		if (ammoType == MagazineData::AmmoType::FIVEPOINTFIVESIX)
		{
			bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ch, mapPosition, dx, dy, WORLD->debugmap->width, WORLD->debugmap->height, 300, 55));
		}
		else
		{
			bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ch, mapPosition, dx, dy, WORLD->debugmap->width, WORLD->debugmap->height, 80, 230));

		}
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

void Firearm::update(Position4 sourcePosition, int mx, int my, double angle)
{
	dx = mx - sourcePosition.x + WORLD->xOffset;
	dy = my - sourcePosition.y + WORLD->yOffset;

	this->sourcePosition = sourcePosition;

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	updateToolPosition(angle);

	mapPosition.height = sourcePosition.height;
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
	fireClock.update(true, false, false);

	if (bulletList.size() > selectedMagazine->ammoCapacity * 2) //clean up extra bullets
	{
		bulletList.pop_back();
	}

	for (auto& bullet : bulletList) //update bullets
	{
		bullet->update();
	}

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
