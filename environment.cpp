#include "main.hpp"

Entity::Entity(Position4 pos, int ch, std::string name, TCODColor color)
	:mapPosition(pos), renderPosition(pos), ch(ch), color(color), name(name)
{}

void Entity::update()
{
	return;
}

void Entity::render(const Pane& pane) const
{
	pane.console->setChar(mapPosition.x, mapPosition.y, ch);
	pane.console->setCharForeground(mapPosition.x, mapPosition.y, color);
}

//----------------------------------------------------------------------------------------------------

Projectile::Projectile(const Creature* owner, int ch, std::string name, TCODColor color, const Position4 startPosition, Position2 targetPosition, int velocity, int mass)
	: Entity(startPosition, ch, name, color), owner(owner), startPosition(startPosition), targetPosition(targetPosition), baseVelocity(velocity), currentVelocity(velocity), mass(mass),
	fTravel(FLine(startPosition, targetPosition)), moveClock(1.0f / velocity), fallClock(0), nextPosition(startPosition), inFov(false), onGround(false)
{
}

void Projectile::doProjectileDamage(std::shared_ptr<Creature>& creature)
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

				damage = int(float(currentVelocity / (baseVelocity * 2.0f)) * mass); //2.0f can be changed to manage ttk and bullet damage //velocities can be changed, the difference between the two are whats important

				creature->takeDamage(damage);
			}
			else //if the bullet is stopped by the armor
			{
				creature->equippedArmor.durability -= currentVelocity;
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
		}
		currentVelocity = 0; //stop bullet

		if (damage > 0)
		{
			GUI->logWindow->pushMessage(Message((creature->name + " shot for " + std::to_string(damage) + " damage!"), Message::MessageLevel::MEDIUM)); //damage message
		}
		else
		{
			GUI->logWindow->pushMessage(Message(("Bullet was blocked!"), Message::MessageLevel::MEDIUM)); //damage message
		}
	}
}

void Projectile::update()
{
	inFov = WORLD->isInPlayerFov(mapPosition);

	if (currentVelocity > 0)
	{
		moveClock.tickUp();
		for (int i = 1; i < moveClock.numCalls; moveClock.numCalls--)
		{
			if (WORLD->debugmap->inMapBounds(nextPosition))
			{
				int decel = WORLD->debugmap->getBlock(nextPosition)->tileList[mapPosition.h].deceleration;
				if (currentVelocity - decel < 0)	currentVelocity = 0;
				else								currentVelocity -= decel;

				if (WORLD->debugmap->getBlock(nextPosition)->destroy(mass, mapPosition.h)) //damage something and if it destroys
				{
					WORLD->updateBlock(nextPosition, false);
					AUDIO->playSound(PositionalStaticSound(("crash"), nextPosition, 85.0f, 100.0f));
				}

				for (auto& creature : WORLD->debugmap->creatureList) //check if hit creatures
				{
					if (Position3(creature->mapPosition) == Position3(mapPosition) && mapPosition.h <= creature->mapPosition.h && creature.get() != owner)
					{
						doProjectileDamage(creature);
					}
				}

				if (currentVelocity > 0) fTravel.stepLine();
			}
			else
			{
				currentVelocity = 0;
				mapPosition.h = 0;
			}
		}
	}

	if (mapPosition.h > 1)
	{
		fallClock.timeBetweenUpdates = (getFallTime(mapPosition.h) - getFallTime(mapPosition.h - 1));
		fallClock.tickUp();
		for (int i = 1; i < fallClock.numCalls; fallClock.numCalls--) mapPosition.h--;
	}
	else
	{
		onGround = true;
	}

	mapPosition = Position4(fTravel.getPosition().x, fTravel.getPosition().y, mapPosition.h, startPosition.z);
	renderPosition = getRenderPosition(mapPosition);

	nextPosition = Position4(fTravel.getNextPosition().x, fTravel.getNextPosition().y, mapPosition.h, mapPosition.z);
}

void Projectile::render(const Pane& pane) const
{
	if (inFov)
	{
		if (currentVelocity > 0)
		{
			if (mapPosition.h > 0) //in the air
			{
				pane.console->setCharForeground(renderPosition.x, renderPosition.y, color);

				if (startPosition.x == fTravel.getPosition().x && startPosition.y == fTravel.getPosition().y)
				{
					pane.console->setChar(renderPosition.x, renderPosition.y, ch); //muzzle flash
				}
				else
				{
					pane.console->setChar(renderPosition.x, renderPosition.y, ch);
				}
			}
			//else //on the ground, use the tile beyond this point
			//{
			//	pane.console->setChar(renderPosition.x, renderPosition.y, ch);
			//	pane.console->setCharForeground(renderPosition.x, renderPosition.y, WORLD->debugmap->getBlock(mapPosition)->tileList[0].foregroundColor);
			//}
		}
	}
}

//----------------------------------------------------------------------------------------------------

Bullet::Bullet(const Creature* owner, std::string name, int ch, const Position4 startPosition, Position2 targetPosition, int velocity, int mass)
	: Projectile(owner, ch, name, TCODColor::copper, startPosition, targetPosition, velocity, mass)
{
}

void Bullet::doProjectileDamage(std::shared_ptr<Creature>& creature)
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

				damage = int(float(currentVelocity / (baseVelocity * 2.0f)) * mass); //2.0f can be changed to manage ttk and bullet damage //velocities can be changed, the difference between the two are whats important

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
			GUI->logWindow->pushMessage(Message((creature->name + " shot for " + std::to_string(damage) + " damage!"), Message::MessageLevel::MEDIUM)); //damage message
		}
		else
		{
			GUI->logWindow->pushMessage(Message(("Bullet was blocked!"), Message::MessageLevel::MEDIUM)); //damage message
		}
	}
}

void Bullet::update()
{
	inFov = WORLD->isInPlayerFov(mapPosition);

	if (currentVelocity > 0)
	{
		moveClock.tickUp();
		for (int i = 1; i < moveClock.numCalls; moveClock.numCalls--)
		{
			if (WORLD->debugmap->inMapBounds(nextPosition))
			{
				int decel = WORLD->debugmap->getBlock(nextPosition)->tileList[mapPosition.h].deceleration;
				if (currentVelocity - decel < 0)	currentVelocity = 0;
				else								currentVelocity -= decel;

				if (WORLD->debugmap->getBlock(nextPosition)->destroy(mass, mapPosition.h)) //damage something and if it destroys
				{
					WORLD->updateBlock(nextPosition, false);
					AUDIO->playSound(PositionalStaticSound(("crash"), nextPosition, 85.0f, 100.0f));
				}

				for (auto& creature : WORLD->debugmap->creatureList) //check if hit creatures
				{
					if (Position3(creature->mapPosition) == Position3(mapPosition) && mapPosition.h <= creature->mapPosition.h && creature.get() != owner)
					{
						doProjectileDamage(creature);
					}
				}

				if (currentVelocity > 0) fTravel.stepLine();
			}
			else
			{
				currentVelocity = 0;
				mapPosition.h = 0;
			}
		}
	}

	if (mapPosition.h > 1)
	{
		fallClock.timeBetweenUpdates = (getFallTime(mapPosition.h) - getFallTime(mapPosition.h - 1));
		fallClock.tickUp();
		for (int i = 1; i < fallClock.numCalls; fallClock.numCalls--) mapPosition.h--;
	}

	mapPosition = Position4(fTravel.getPosition().x, fTravel.getPosition().y, mapPosition.h, startPosition.z);
	renderPosition = getRenderPosition(mapPosition);

	nextPosition = Position4(fTravel.getNextPosition().x, fTravel.getNextPosition().y, mapPosition.h, mapPosition.z);
}

void Bullet::render(const Pane& pane) const
{
	if (inFov)
	{
		if (currentVelocity > 0)
		{
			if (mapPosition.h > 0) //in the air
			{
				pane.console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::brass); //check later

				if (startPosition.x == fTravel.getPosition().x && startPosition.y == fTravel.getPosition().y)
				{
					pane.console->setChar(renderPosition.x, renderPosition.y, '*'); //muzzle flash
				}
				else
				{
					pane.console->setChar(renderPosition.x, renderPosition.y, ch);
				}
			}
			else //on the ground
			{
				pane.console->setChar(renderPosition.x, renderPosition.y, ch);
				pane.console->setCharForeground(renderPosition.x, renderPosition.y, WORLD->debugmap->getBlock(mapPosition)->tileList[0].foregroundColor);
			}
		}
	}
}

