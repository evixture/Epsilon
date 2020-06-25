#include "main.hpp"

Tool::Tool(const Creature* owner, std::string name, TCODColor color, int ch)
	: name(name), color(color), ch(ch), mapPosition(Position4(0, 0, 0, 0)), dx(0), dy(0), sourcePosition(Position4(0, 0, 0, 0)), owner(owner),
	ammoType(MagazineData::AmmoType::NONE), fireMode(SAFE), availibleFireMode(0), isHeld(false), type(Tool::Type::TOOL), angle(0.0f), effectiveRange(5)
{
}

Tool::Tool(const Creature* owner, std::string name, TCODColor color, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag)
	: name(name), color(color), ch(NULL), mapPosition(Position4(0, 0, 0, 0)), dx(0), dy(0), sourcePosition(Position4(0, 0, 0, 0)), owner(owner),
	ammoType(ammoType), fireMode(fireMode), availibleFireMode(availibleFireModeFlag), isHeld(false), type(Tool::Type::TOOL), angle(0.0f), effectiveRange(5)
{
}

std::pair<bool, MagazineData&> Tool::getMagazine()
{
	MagazineData nullMag = MagazineData(MagazineData::AmmoType::NONE, 0, 0);
	return std::pair<bool, MagazineData&>(false, nullMag);
}

bool Tool::reload(MagazineData& magazine)
{
	return false;
}

void Tool::changeFireMode()
{
	return;
}

void Tool::useMelee()
{
	return;
}

void Tool::updateToolPosition(int targetX, int targetY)
{
	angle = abs(getAngle(sourcePosition.x, sourcePosition.y, targetX, targetY));

	if (isnan(angle)) return;

	if (angle < 22.5f)
	{
		if (dx <= 0)
		{
			mapPosition.x = sourcePosition.x - 1;
			mapPosition.y = sourcePosition.y;
			return;
		}
		else
		{
			mapPosition.x = sourcePosition.x + 1;
			mapPosition.y = sourcePosition.y;
			return;
		}
	}
	else if (angle > 65.7f)
	{
		if (dy <= 0)
		{
			mapPosition.x = sourcePosition.x;
			mapPosition.y = sourcePosition.y - 1;
			return;
		}
		else
		{
			mapPosition.x = sourcePosition.x;
			mapPosition.y = sourcePosition.y + 1;
			return;
		}
	}
	else //45 degree
	{
		if (dx <= 0)
		{
			if (dy <= 0) //top left
			{
				mapPosition.x = sourcePosition.x - 1;
				mapPosition.y = sourcePosition.y - 1;
				return;
			}
			else //bottom left
			{
				mapPosition.x = sourcePosition.x - 1;
				mapPosition.y = sourcePosition.y + 1;
				return;
			}
		}
		else
		{
			if (dy <= 0) //top right
			{
				mapPosition.x = sourcePosition.x + 1;
				mapPosition.y = sourcePosition.y - 1;
				return;
			}
			else //bottom right
			{
				mapPosition.x = sourcePosition.x + 1;
				mapPosition.y = sourcePosition.y + 1;
				return;
			}
		}
	}
}

void Tool::changeBarColor(TCODColor& color)
{
}

void Tool::equip(Armor& armor)
{
}

void Tool::use(bool hold, bool swtch)
{
}

void Tool::updatePositions(Position4& sourcePosition, int& targetX, int& targetY)
{
	this->sourcePosition = sourcePosition;
	mapPosition.h = sourcePosition.h;
	mapPosition.z = sourcePosition.z;

	if (!((targetX - sourcePosition.x == 0) && (targetY - sourcePosition.y == 0))) //if cursor is at source position
	{
		dx = targetX - sourcePosition.x; //takes mouse input, applies to all creatures, but only useful for player
		dy = targetY - sourcePosition.y;
	}
	else
	{
		mapPosition.x = sourcePosition.x + dx;
		mapPosition.y = sourcePosition.y + dy;
	}

	updateToolPosition(targetX, targetY);

	renderPosition = getRenderPosition(mapPosition);
}

void Tool::update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld)
{
	this->isHeld = isHeld;
	updatePositions(sourcePosition, targetX, targetY);
}

void Tool::render(const Pane& pane) const
{
	if (this->isHeld)
	{
		pane.console->setChar(renderPosition.x, renderPosition.y, ch);
		pane.console->setCharForeground(renderPosition.x, renderPosition.y, color);
	}
}

//----------------------------------------------------------------------------------------------------

Melee::Melee(Tool tool, int bluntDamage, int sharpDamage)
	: Tool(tool), bluntDamage(bluntDamage), sharpDamage(sharpDamage)
{
	type = Tool::Type::MELEE;
	effectiveRange = 1;
}

void Melee::useMelee()
{
	for (auto& creature : WORLD->debugmap->creatureList)
	{
		if (mapPosition == creature->mapPosition)
		{
			if (creature.get() != owner)
			{
				doMeleeDamage(creature);

				AUDIO->playSound(Sound(("hit"), 60.0f, 90.0f)); //tracked?

				break;
			}
		}
	}

	if (WORLD->debugmap->getBlock(mapPosition)->destroy((bluntDamage + sharpDamage), mapPosition.h))
	{
		AUDIO->playSound(PositionalStaticSound(("crash"), mapPosition, 85.0f, 100.0f));
	}

	WORLD->updateBlock(mapPosition, false);
}

void Melee::doMeleeDamage(std::shared_ptr<Creature>& creature)
{
	if (creature->health > 0) //if creature is alive //HOW TO CHECK IF NOT HOLDING CREATURE
	{																				
		float sharpDamageResult = sharpDamage * (1.0f - (creature->equippedArmor.defense / 400.0f));
		float bluntDamageResult = bluntDamage * 1.0f; //should bluntdamage do less damage at higher armor?

		int totalDamage = int(sharpDamageResult + bluntDamageResult);

		creature->takeDamage(totalDamage);

		GUI->logWindow->pushMessage(LogWindow::Message((creature->name + " was hit for " + std::to_string(totalDamage) + " damage!"), LogWindow::Message::MessageLevel::MEDIUM));
	}
}

void Melee::update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld)
{
	this->isHeld = isHeld;
	updatePositions(sourcePosition, targetX, targetY);

	if (INPUT->primaryUseButton->isSwitched && this->isHeld) useMelee();
}

void Melee::render(const Pane& pane) const
{
	if (this->isHeld)
	{
		pane.console->setChar(renderPosition.x, renderPosition.y, ch);
		pane.console->setCharForeground(renderPosition.x, renderPosition.y, color);
	}
}

//----------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------

Firearm::Firearm(const Creature* owner, std::string name, TCODColor color, int fireRPS, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag)
	:Melee(Tool(owner, name, color, ammoType, fireMode, availibleFireModeFlag), /* MELEE DAMAGE */ 25, 0), fireRPS(fireRPS), reloadTime(reloadSpeed),
	usedMag(MagazineData(ammoType, 0, 0)), fireClock(1.0f / fireRPS), reloadClock(reloadSpeed)
{
	type = Tool::Type::FIREARM;

	switch (ammoType)
	{
	case MagazineData::AmmoType::FOURTYFIVEACP:
		effectiveRange = 10;
		break;
	case MagazineData::AmmoType::FIVEPOINTFIVESIX:
		effectiveRange = 20;
		break;
	}

	reloadClock.numCalls = 1.0f;
	fireClock.numCalls = 1.0f;
}

void Firearm::updateToolPosition(int targetX, int targetY)
{
	/*
	02|01
	--|--
	03|04
	*/

	angle = abs(getAngle(sourcePosition.x, sourcePosition.y, targetX, targetY));

	if (isnan(angle) || (dx == 0 && dy == 0)) return;

	if (angle < 22.5f) //left / right
	{
		if (dx <= 0) //left
		{
			mapPosition.x = sourcePosition.x - 1;
			mapPosition.y = sourcePosition.y;

		}
		else //right
		{
			mapPosition.x = sourcePosition.x + 1;
			mapPosition.y = sourcePosition.y;
		}

		if ((reloadClock.numCalls < 1)) //if reloading
		{
			ch = '/';
		}
		else
		{
			ch = TCOD_CHAR_HLINE;
		}
	}
	else if (angle > 65.7f) //up / down
	{
		if (dy <= 0) //up
		{
			mapPosition.x = sourcePosition.x;
			mapPosition.y = sourcePosition.y - 1;
		}
		else //down
		{
			mapPosition.x = sourcePosition.x;
			mapPosition.y = sourcePosition.y + 1;
		}

		if ((reloadClock.numCalls < 1)) //if reloading
		{
			ch = '\\';
		}
		else
		{
			ch = TCOD_CHAR_VLINE;
		}
	}
	else //45 degree
	{
		if (dx <= 0)
		{
			if (dy <= 0) //top left
			{
				mapPosition.x = sourcePosition.x - 1;
				mapPosition.y = sourcePosition.y - 1;

				if ((reloadClock.numCalls < 1)) //if reloading
				{
					ch = TCOD_CHAR_HLINE;
				}
				else
				{
					ch = '\\';
				}
			}
			else //bottom left
			{
				mapPosition.x = sourcePosition.x - 1;
				mapPosition.y = sourcePosition.y + 1;

				if ((reloadClock.numCalls < 1)) //if reloading
				{
					ch = TCOD_CHAR_VLINE;
				}
				else
				{
					ch = '/';
				}
			}

		}
		else
		{
			if (dy <= 0) //top right
			{
				mapPosition.x = sourcePosition.x + 1;
				mapPosition.y = sourcePosition.y - 1;

				if ((reloadClock.numCalls < 1)) //if reloading
				{
					ch = TCOD_CHAR_VLINE;
				}
				else
				{
					ch = '/';
				}
			}
			else //bottom right
			{
				mapPosition.x = sourcePosition.x + 1;
				mapPosition.y = sourcePosition.y + 1;

				if ((reloadClock.numCalls < 1)) //if reloading
				{
					ch = TCOD_CHAR_HLINE;
				}
				else
				{
					ch = '\\';
				}
			}
		}
	}
}

std::pair<bool, MagazineData&> Firearm::getMagazine()
{
	return std::pair<bool, MagazineData&>(true, usedMag);
}

void Firearm::fireBullet()
{
	if (!(mapPosition.x == dx + mapPosition.x && mapPosition.y == dy + mapPosition.y))
	{
		for (float i = 1.0f; i <= fireClock.numCalls; fireClock.numCalls--)
		{
			switch (getMagazine().second.ammoType)
			{
			case MagazineData::AmmoType::FOURTYFIVEACP:
				bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ep::bullet::cal45(owner, "cal45", mapPosition, Position2(mapPosition.x + dx, mapPosition.y + dy))));
				break;
			case MagazineData::AmmoType::FIVEPOINTFIVESIX:
				bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ep::bullet::cal556(owner, "cal556", mapPosition, Position2(mapPosition.x + dx, mapPosition.y + dy))));
				break;
			}

			//bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(owner, ch, mapPosition, dx, dy, WORLD->debugmap->width, WORLD->debugmap->height, getMagazine().second.velocity, getMagazine().second.mass));

			usedMag.availableAmmo--;

			AUDIO->playSound(PositionalTrackedSound(("cop"), &mapPosition, 100.0f, 80.0f));
		}
	}
}

bool Firearm::reload(MagazineData& magazine)
{
	if (this->isHeld && reloadClock.numCalls >= 1.0f)
	{
		std::swap(this->usedMag, magazine);
		reloadClock.addTime(reloadTime);
		AUDIO->playSound(PositionalTrackedSound(("check chick"), &mapPosition, 65.0f, 40.0f));

		return true;
	}
	return false;
}

void Firearm::changeFireMode()
{
	if (isHeld)
	{
		switch (fireMode)
		{
		case FireType::SAFE:
			if (availibleFireMode & FireType::FULL)			fireMode = FireType::FULL;
			else if (availibleFireMode & FireType::SEMI)	fireMode = FireType::SEMI;
			break;

		case FireType::SEMI:
			if (availibleFireMode & FireType::SAFE)			fireMode = FireType::SAFE;
			else if (availibleFireMode & FireType::FULL)	fireMode = FireType::FULL;
			break;

		case FireType::FULL:
			if (availibleFireMode & FireType::SEMI)			fireMode = FireType::SEMI;
			else if (availibleFireMode & FireType::SAFE)	fireMode = FireType::SAFE;
			break;
		}
		AUDIO->playSound(Sound(("tip"), 55.0f, 50.0f));
	}
}

void Firearm::changeBarColor(TCODColor& color)
{
	if (usedMag.ammoCapacity != 0) color = TCODColor::lerp(TCODColor::red, TCODColor::darkerGreen, (float(usedMag.availableAmmo) / float(usedMag.ammoCapacity)));
	else color = TCODColor::orange;
}

void Firearm::use(bool hold, bool swtch)
{
	if (isHeld)
	{
		if (fireClock.numCalls >= 1.0f && usedMag.availableAmmo > 0 && reloadClock.numCalls >= 1.0f) //fires bullet
		{
			if ((fireMode == FireType::FULL && (hold || swtch)) || (fireMode == FireType::SEMI && swtch)) fireBullet();
		}
	}
}

void Firearm::update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld)
{
	this->isHeld = isHeld;
	updatePositions(sourcePosition, targetX, targetY);

	if (bulletList.size() > usedMag.ammoCapacity * 5) bulletList.pop_back(); //clean up extra bullets if there are more than 5 magazines worth of bullets on the map

	for (int i = 0; i < bulletList.size(); ++i) //update bullets
	{
		if (bulletList[i]->currentVelocity <= 0) bulletList.erase(bulletList.begin() + i);
		else bulletList[i]->update();
	}

	if (fireClock.numCalls < 1.0f) fireClock.tickUp();
	if (reloadClock.numCalls < 1.0f) reloadClock.tickUp();
}

void Firearm::render(const Pane& pane) const
{
	if (this->isHeld)
	{
		pane.console->setChar(renderPosition.x, renderPosition.y, ch);
		pane.console->setCharForeground(renderPosition.x, renderPosition.y, color);
	}

	for (auto& bullet : bulletList)
	{
		bullet->render(pane);
	}
}

Armor::Armor(const Creature* owner, std::string name, TCODColor color, int defense, int durability)
	: Tool(owner, name, color, ep::character::ballisticVest), defense(defense), durability(durability)
{
	type = Tool::Type::ARMOR;
}

void Armor::equip(Armor& armor) //if the passed armor is not equal to the armor of this, then replace passed armor with this armor
{
	if (isHeld)
	{
		if (this->defense == armor.defense && this->durability == armor.durability) //if armor is already equipped
		{
			armor = Armor(owner, "", TCODColor::pink, 0, 0); //??
		}
		else
		{
			armor = Armor(owner, this->name, this->color, this->defense, this->durability);

			AUDIO->playSound(Sound(("full ip"), 60.0f, 100.0f));
		}
	}
}

