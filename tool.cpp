#include "main.hpp"

Tool::Tool(std::string name, TCODColor color, int ch)
	:name(name), color(color), ch(ch), mapPosition(Position4(0, 0, 0, 0)), dx(0), dy(0), sourcePosition(Position4(0, 0, 0, 0)), 
	ammoType(MagazineData::AmmoType::NONE), fireMode(SAFE), availibleFireMode(0), isHeld(false), type(Tool::Type::TOOL), angle(0.0f)
{}

Tool::Tool(std::string name, TCODColor color, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag)
	: name(name), color(color), ch(NULL), mapPosition(Position4(0, 0, 0, 0)), dx(0), dy(0), sourcePosition(Position4(0, 0, 0, 0)), 
	ammoType(ammoType), fireMode(fireMode), availibleFireMode(availibleFireModeFlag), isHeld(false), type(Tool::Type::TOOL), angle(0.0f)
{}

MagazineData& Tool::getMagData()
{
	auto nullMag = MagazineData(MagazineData::AmmoType::NONE, 0, 0, false);
	return nullMag;
}

void Tool::reload(MagazineData& magazine)
{}

void Tool::changeFireMode()
{}

void Tool::useMelee()
{
}

void Tool::updateToolPosition(int targetX, int targetY)
{
	//double absAngle = abs(angle);

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
			else //bottom rigth
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
	mapPosition.height = sourcePosition.height;
	mapPosition.floor = sourcePosition.floor;

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

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset); // has to be after update tool position
}

void Tool::update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld)
{
	this->isHeld = isHeld;
	updatePositions(sourcePosition, targetX, targetY);

	if (this->isHeld)
	{
		//behavior if it is held
	}

}

void Tool::render(const std::shared_ptr<Pane>& pane) const
{
	if (this->isHeld)
	{
		pane->console->setChar(renderPosition.x, renderPosition.y, ch);
		pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
	}
}

//----------------------------------------------------------------------------------------------------

Melee::Melee(Tool tool, int bluntDamage, int sharpDamage)
	: Tool(tool), bluntDamage(bluntDamage), sharpDamage(sharpDamage)
{
	type = Tool::Type::MELEE;
}

void Melee::useMelee()
{
	for (auto& creature : WORLD->debugmap->creatureList)
	{
		//if ((sourcePosition.x == creature->mapPosition.x && sourcePosition.y == creature->mapPosition.y && sourcePosition.floor == creature->mapPosition.floor) || //if the creature is on top of the creature
		//	(mapPosition.x == creature->mapPosition.x && mapPosition.y == creature->mapPosition.y && mapPosition.floor == creature->mapPosition.floor)) //if the melee tool is on top of the creature
		if (mapPosition.x == creature->mapPosition.x && mapPosition.y == creature->mapPosition.y)
		{
			if (creature != WORLD->debugmap->player)
			{
				doMeleeDamage(creature);
				break;
			}
		}
	}

	//if it cannot find a creature

	WORLD->debugmap->getBlock(mapPosition)->destroy((bluntDamage + sharpDamage), mapPosition.height);

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

	if (this->isHeld)
	{
		if (INPUT->primaryUseButton->isSwitched)
		{
			useMelee();
		}
	}

}

void Melee::render(const std::shared_ptr<Pane>& pane) const
{
	if (this->isHeld)
	{
		pane->console->setChar(renderPosition.x, renderPosition.y, ch);
		pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
	}
}

//----------------------------------------------------------------------------------------------------

Bullet::Bullet(int ch, Position4 startPosition, int dx, int dy, int xbound, int ybound, int velocity, int mass)
	:ch(ch), startPosition(startPosition), tox(dx), toy(dy), xbound(xbound), ybound(ybound), travel(BLine(startPosition.x, startPosition.y, tox, toy)),
	mapPosition(startPosition), mass(mass), baseVelocity(velocity), currentVelocity(velocity), moveClock(1.0f / velocity), fallClock(0)
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

void Bullet::doBulletDamage(std::shared_ptr<Creature>& creature)
{
	int damage = 0;

	if (creature->health != 0)
	{
		if (creature->equippedArmor.durability > 0) //if the armor durability is high enough
		{
			if (currentVelocity - creature->equippedArmor.defense > 0) //if bullet is fast enough to pass through armor
			{
				creature->equippedArmor.durability -= currentVelocity; //should happen before taking damage to prevent high damage
				currentVelocity -= creature->equippedArmor.defense;

				damage = int(float(currentVelocity / (baseVelocity * 2.0f)) * mass); //2.0f can be changed to manage ttk and bullet damage

				creature->takeDamage(damage);

				currentVelocity -= 100; //slowdown after going through body
			}
			else //if the bullet is stopped by the armor
			{
				creature->equippedArmor.durability -= currentVelocity;
				currentVelocity = 0;
			}

			if (creature->equippedArmor.durability < 0)
			{
				creature->equippedArmor.durability = 0;
			}
		}
		else
		{
			damage = int(float(currentVelocity / (baseVelocity * 2.0f)) * mass);
			
			creature->takeDamage(damage);

			currentVelocity -= 100; //slowdown after going through body
		}

		if (damage > 0)
		{
			GUI->logWindow->pushMessage(LogWindow::Message((creature->name + " shot for " + std::to_string(damage) + " damage!"), LogWindow::Message::MessageLevel::MEDIUM)); //damage message
		}
		else
		{
			GUI->logWindow->pushMessage(LogWindow::Message(("Bullet was blocked!"), LogWindow::Message::MessageLevel::MEDIUM)); //damage message
		}
	}
}

void Bullet::update()
{
	if (currentVelocity > 0 && mapPosition.height > 0)
	{
		moveClock.tickUp();

		for (int i = 1; i < moveClock.numCalls; moveClock.numCalls--)
		{
			if (WORLD->debugmap->inMapBounds(mapPosition))
			{
				WORLD->debugmap->getBlock(mapPosition)->destroy(mass, mapPosition.height);
				WORLD->updateBlock(mapPosition, false); //check if pos needs to be reassigned before

				int decel = WORLD->debugmap->getBlock(mapPosition)->tileList[mapPosition.height].deceleration;

				if (currentVelocity - decel < 0)
				{
					currentVelocity = 0;
				}
				else
				{
					currentVelocity -= WORLD->debugmap->getBlock(mapPosition)->tileList[mapPosition.height].deceleration;
				}			
			
				for (auto& creature : WORLD->debugmap->creatureList) //if hit a creature
				{
					if (creature->mapPosition.x == mapPosition.x && creature->mapPosition.y == mapPosition.y && creature->mapPosition.floor == mapPosition.floor && mapPosition.height <= creature->mapPosition.height)
					{
						if (!(mapPosition == startPosition))
						{
							if (creature->health != 0) //check for later
							{
								doBulletDamage(creature);
							}
						}
					}
				}
				
				travel.step();
				mapPosition = Position4(travel.x, travel.y, mapPosition.height, startPosition.floor);

			}
			else
			{
				currentVelocity = 0;
				mapPosition.height = 0;
			}
		}
	}

	if (currentVelocity > 0 && mapPosition.height > 0)
	{
		fallClock.timeBetweenUpdates = (getFallTime(mapPosition.height) - getFallTime(mapPosition.height - 1));
		fallClock.tickUp();

		for (int i = 1; i < fallClock.numCalls; fallClock.numCalls--)
		{
			mapPosition.height--;
		}
		mapPosition = Position4(travel.x, travel.y, mapPosition.height, startPosition.floor);
	}

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Bullet::render(const std::shared_ptr<Pane>& pane) const
{
	if (WORLD->debugmap->player->mapPosition.floor == startPosition.floor)
	{
		if (currentVelocity > 0)
		{
			if (mapPosition.height > 0) //in the air
			{
				//if (WORLD->isInPlayerFov(mapPosition))
				//{
					pane->console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::brass); //check later

					if ((startPosition.x == travel.x && startPosition.y == travel.y))
					{
						pane->console->setChar(renderPosition.x, renderPosition.y, '*'); //muzzle flash
					}
					else
					{
						pane->console->setChar(renderPosition.x, renderPosition.y, ch);
					}
				//}
			}
			else //on the ground
			{
				//if (WORLD->isInPlayerFov(mapPosition)) //not in fov when on ground
				//{
					pane->console->setChar(renderPosition.x, renderPosition.y, ch);
					pane->console->setCharForeground(renderPosition.x, renderPosition.y, WORLD->debugmap->getBlock(mapPosition)->tileList[0].foregroundColor);
				//}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------

Firearm::Firearm(std::string name, TCODColor color, int shotsPerSecond, float reloadSpeed, MagazineData::AmmoType ammoType, FireType fireMode, char availibleFireModeFlag)
	:Melee(Tool(name, color, ammoType, fireMode, availibleFireModeFlag), /* MELEE DAMAGE */ 25, 0), fireRPS(shotsPerSecond), reloadTime(reloadSpeed),
	usedMag(MagazineData(ammoType, 0, 0, false)), fireClock(1.0f / shotsPerSecond), reloadClock(reloadSpeed)//, fireNumCalls(0), reloadNumCalls(0)
{
	type = Tool::Type::FIREARM;

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
	
	//double absAngle = abs(angle);

	angle = abs(getAngle(sourcePosition.x, sourcePosition.y, targetX, targetY));

	if (isnan(angle) || (dx == 0 && dy == 0))
	{
		return;
	}

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

MagazineData& Firearm::getMagData()
{
	return usedMag;
}

void Firearm::fireBullet()
{
	if (!(mapPosition.x == dx + mapPosition.x && mapPosition.y == dy + mapPosition.y))
	{
		//if (fireMode == FireType::SEMI)
		//{
		//	fireClock.numCalls = 1; //check later to see if it prevents overfireing
		//}
		//else
		//{
		//	fireClock.tickUp();
		//}

		//fireClock.tickUp(); //move up to other fire command

		for (float i = 1.0f; i <= fireClock.numCalls; fireClock.numCalls--)
		{
			//if (ammoType == MagazineData::AmmoType::FIVEPOINTFIVESIX)
			//{
			//	//bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ep::bullet::cal556(ch, mapPosition, dx, dy, WORLD->debugmap->width, WORLD->debugmap->height)));
			//}
			//else
			//{
			//	//bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ep::bullet::cal45(ch, mapPosition, dx, dy, WORLD->debugmap->width, WORLD->debugmap->height)));
			//}

			//Bullet(int ch, Position4 startPosition, int dx, int dy, int xbound, int ybound, int velocity, int mass)
			bulletList.insert(bulletList.begin(), std::make_shared<Bullet>(ch, mapPosition, dx, dy, WORLD->debugmap->width, WORLD->debugmap->height, getMagData().velocity, getMagData().mass));

			usedMag.availableAmmo--;

			WORLD->soundBuffer.push_back(std::make_shared<Sound>(mapPosition, 120, 100));
		}
	}
}

void Firearm::reload(MagazineData& magazine)
{
	if (this->isHeld)
	{
		//if (magazine.isValid != false)
		//{

			if (reloadClock.numCalls >= 1.0f) //if it can reload
			{
				std::swap(this->usedMag, magazine);

				//selectedMagazine = magazine;
				reloadClock.addTime(reloadTime);
			}
		//}
	//	else selectedMagazine = magazine;
	}
}

void Firearm::changeFireMode()
{
	if (isHeld)
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
}

void Firearm::changeBarColor(TCODColor& color)
{
	if (usedMag.ammoCapacity != 0)
	{
		color = TCODColor::lerp(TCODColor::red, TCODColor::darkerGreen, (float(usedMag.availableAmmo) / float(usedMag.ammoCapacity)));
	}
	else
	{
		color = TCODColor::orange;
	}
}

void Firearm::use(bool hold, bool swtch)
{
	if (isHeld)
	{
		if (fireClock.numCalls >= 1.0f && usedMag.availableAmmo > 0 && reloadClock.numCalls >= 1.0f) //fires bullet
		{
			if (fireMode == FireType::FULL && (hold || swtch))// && INPUT->primaryUseButton->isDown)
			{
				fireBullet();
			}
			else if (fireMode == FireType::SEMI && swtch)// && INPUT->primaryUseButton->isSwitched)
			{
				fireBullet();
			}
			else if (fireMode == FireType::SAFE)
			{

			}
		}
	}
}

void Firearm::update(Position4& sourcePosition, int& targetX, int& targetY, bool& isHeld)
{
	this->isHeld = isHeld;
	updatePositions(sourcePosition, targetX, targetY);
	
	if (this->isHeld)
	{
		//GUI->logWindow->pushMessage(LogWindow::Message(std::string("reload : " + std::to_string(reloadClock.numCalls) + " fire : " + std::to_string(fireClock.numCalls)), LogWindow::Message::MessageLevel::MEDIUM));
	}

	if (bulletList.size() > usedMag.ammoCapacity * 5) //clean up extra bullets if there are more than 5 magazines worth of bullets on the map
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

	if (fireClock.numCalls < 1.0f)
	{
		fireClock.tickUp();
	}

	if (reloadClock.numCalls < 1.0f)
	{
		reloadClock.tickUp();
	}
}

void Firearm::render(const std::shared_ptr<Pane>& pane) const
{
	if (this->isHeld)
	{
		pane->console->setChar(renderPosition.x, renderPosition.y, ch);
		pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
	}

	for (auto& bullet : bulletList)
	{
		bullet->render(pane);
	}
}

Armor::Armor(std::string name, TCODColor color, int defense, int durability)
	:Tool(name, color, ep::character::ballisticVest), defense(defense), durability(durability)
{
	type = Tool::Type::ARMOR;
}

void Armor::equip(Armor& armor) //if the passed armor is not equal to the armor of this, then replace passed armor with this armor
{
	if (isHeld)
	{
		if (this->defense == armor.defense && this->durability == armor.durability) //if armor is already equipped
		{
			armor = Armor("", TCODColor::pink, 0, 0);
		}
		else
		{
			armor = Armor(this->name, this->color, this->defense, this->durability);
		}
	}
}

