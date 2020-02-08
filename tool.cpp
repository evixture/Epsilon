#include "main.hpp"

Tool::Tool(std::string name, TCODColor color, int ch)
	:name(name), color(color), ch(ch), mapPosition(Position4(0, 0, 0, 0)), dx(0), dy(0), sourcePosition(Position4(0, 0, 0, 0)), ammoType(MagazineData::AmmoType::NONE), fireMode(SAFE), availibleFireMode(0)
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

void Tool::equip(Armor& armor)
{
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
	mapPosition(startPosition), mass(mass), baseVelocity(velocity), currentVelocity(velocity), moveClock(1.0f / velocity), fallClock(0)//, moveNumCalls(0), fallNumCalls(0)
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
		if (currentVelocity > 0 && mapPosition.height > 0)
		{
			//moveNumCalls += SETTINGS->lastFrameTime.asSeconds() / (1.0f / currentVelocity);
			moveClock.tickUp();

			for (int i = 1; i < moveClock.numCalls; moveClock.numCalls--)
			{
				if (WORLD->inMapBounds(mapPosition))
				{
					if (WORLD->getSolidity(mapPosition) == true) //if hit a wall
					{
						if (WORLD->getTile(mapPosition)->tag == Block::Tag::DESTRUCTIBLE)
						{
							WORLD->getTile(mapPosition)->destroy(mass);
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
						for (auto& creature : WORLD->creatureList) //if hit a creature
						{
							if (creature->mapPosition.x == mapPosition.x && creature->mapPosition.y == mapPosition.y && creature->mapPosition.level == mapPosition.level) //also checks height, may give bad results
							{
								if (creature->health > 0)
								{
									creature->takeDamage(this);
								}

								GUI->logWindow->pushMessage(LogWindow::Message("You hit a creature!", LogWindow::Message::MessageLevel::HIGH));
							}
						}
					}
					travel.step();
				}
				else
				{
					currentVelocity = 0;
					mapPosition.height = 0;
				}

				mapPosition = Position4(travel.x, travel.y, mapPosition.height, startPosition.level);

			}

		}
	
		if (currentVelocity > 0 && mapPosition.height > 0)
		{
			//fallNumCalls += SETTINGS->lastFrameTime.asSeconds() / (getFallTime(mapPosition.height) - getFallTime(mapPosition.height - 1));

			fallClock.timeBetweenUpdates = (getFallTime(mapPosition.height) - getFallTime(mapPosition.height - 1)); //need??
			fallClock.tickUp();

			for (int i = 1; i < fallClock.numCalls; fallClock.numCalls--)
			{
				mapPosition.height--;
			}
			mapPosition = Position4(travel.x, travel.y, mapPosition.height, startPosition.level);
		}

	mapPosition = Position4(travel.x, travel.y, mapPosition.height, startPosition.level); //needed??
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

Firearm::Firearm(std::string name, TCODColor color, int shotsPerSecond, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag)
	:Tool(name, color, ammoType, fireMode, availibleFireModeFlag), fireRPM(shotsPerSecond), reloadTime(reloadSpeed),
	//fireClock(Clock(fireRate, 0.0f)), reloadClock(Clock(reloadSpeed, 0.0f)),
	selectedMagazine(std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false)), fireClock(shotsPerSecond), reloadClock(reloadSpeed)//, fireNumCalls(0), reloadNumCalls(0)
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
			if (!(reloadClock.numCalls >= 0))
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
		if (fireMode == FireType::SEMI)
		{
			fireClock.numCalls = 1; //check later to see if it prevents overfireing
		}
		else
		{
			fireClock.tickUp();
		}

		for (int i = 1; i <= fireClock.numCalls; fireClock.numCalls--)
		{
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
}

void Firearm::reload(std::shared_ptr<MagazineData>& magazine)
{
	if (magazine->isValid != false)
	{
		if (reloadClock.numCalls >= 0.0f) //if it can reload
		{
			selectedMagazine = magazine;
			//reloadClock.numCalls -= reloadTime;
			reloadClock.addTime(reloadTime);
		}
	}
//	else selectedMagazine = magazine;
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
	
	if (fireClock.numCalls >= 0.0f && selectedMagazine->availableAmmo != 0 && reloadClock.numCalls >= 0.0f) //fires bullet
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

	if (bulletList.size() > selectedMagazine->ammoCapacity) //clean up extra bullets
	{
		bulletList.pop_back();
	}

	for (int i = 0; i < bulletList.size(); ++i) //update bullets
	{
		if (bulletList[i]->currentVelocity <= 0)
		{
			bulletList.erase(bulletList.begin() + i);
		}
		else
		{
			bulletList[i]->update();
		}
	}

	if (reloadClock.numCalls < 0.0f)
	{
		reloadClock.tickUp();
	}
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

Armor::Armor(std::string name, TCODColor color, int defense, int durability)
	:Tool(name, color, 234), defense(defense), durability(durability)
{
}

void Armor::equip(Armor& armor) //if the passed armor is not equal to the armor of this, then replace passed armor with this armor
{
	if (this->defense == armor.defense && this->durability == armor.durability) //if armor is already equipped
	{
		armor = Armor("", TCODColor::pink, 0, 0); //Armor("", TCODColor::pink, 0, 0)
	}
	else
	{
		armor = Armor(this->name, this->color, this->defense, this->durability);
	}
}
