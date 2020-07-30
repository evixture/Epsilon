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

Projectile::Projectile(const Creature* owner, int ch, std::string name, TCODColor color, const Position4 startPosition, Position2 targetPosition, int velocity, float mass)
	: Entity(startPosition, ch, name, color), owner(owner), startPosition(startPosition), targetPosition(targetPosition), velocity(velocity), mass(mass),
	fTravel(FLine(startPosition, targetPosition)), moveClock(1.0f / velocity), fallClock(0), nextPosition(startPosition), inFov(false), onGround(false)
{
}

void Projectile::doProjectileDamage(std::shared_ptr<Creature>& creature)
{
	int damage = 0; //can be removed?

	if (creature->health != 0)
	{
		if (creature->equippedArmor.durability > 0) //if the armor durability is high enough
		{
			if (velocity - creature->equippedArmor.defense > 0) //if bullet is fast enough to pass through armor
			{
				creature->equippedArmor.durability -= velocity; //should happen before taking damage to prevent high damage
				velocity -= creature->equippedArmor.defense;

				damage = calculateProjectileDamage(false, velocity, mass);

				creature->takeDamage(damage);
			}
			else //if the bullet is stopped by the armor
			{
				creature->equippedArmor.durability -= velocity;
			}

			if (creature->equippedArmor.durability < 0)
			{
				creature->equippedArmor.durability = 0;
			}
		}
		else
		{
			damage = calculateProjectileDamage(false, velocity, mass);

			creature->takeDamage(damage);
		}
		velocity = 0; //stop bullet

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

	if (velocity > 0)
	{
		moveClock.tickUp();
		for (int i = 1; i < moveClock.numCalls; moveClock.numCalls--)
		{
			if (WORLD->debugmap->inMapBounds(nextPosition))
			{
				int decel = WORLD->debugmap->getBlock(nextPosition)->tileList[mapPosition.h].deceleration;
				if (velocity - decel < 0)	velocity = 0;
				else								velocity -= decel;

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

				if (velocity > 0) fTravel.stepLine();
			}
			else
			{
				velocity = 0;
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
		if (velocity > 0)
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

Bullet::Bullet(const Creature* owner, std::string name, int ch, const Position4 startPosition, Position2 targetPosition, int velocity, float mass)
	: Projectile(owner, ch, name, TCODColor::copper, startPosition, targetPosition, velocity, mass)
{
}

void Bullet::doProjectileDamage(std::shared_ptr<Creature>& creature)
{
	int damage = 0; //can be removed?

	if (creature->health != 0)
	{
		if (creature->equippedArmor.durability > 0) //if the armor durability is high enough
		{
			if (velocity - creature->equippedArmor.defense > 0) //if bullet is fast enough to pass through armor
			{
				creature->equippedArmor.durability -= velocity; //should happen before taking damage to prevent high damage
				velocity -= creature->equippedArmor.defense;

				damage = calculateProjectileDamage(true, velocity, mass);

				creature->takeDamage(damage);

				velocity -= 100; //slowdown after going through body
			}
			else //if the bullet is stopped by the armor
			{
				creature->equippedArmor.durability -= velocity;
				velocity = 0;
			}

			if (creature->equippedArmor.durability < 0)
			{
				creature->equippedArmor.durability = 0;
			}
		}
		else
		{
			damage = calculateProjectileDamage(true, velocity, mass);

			creature->takeDamage(damage);

			velocity -= 100; //slowdown after going through body
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

	if (velocity > 0)
	{
		moveClock.tickUp();

		GUI->logWindow->pushMessage(Message("STEPS: " + std::to_string(moveClock.numCalls), Message::MessageLevel::MEDIUM));

		for (int i = 1; i < moveClock.numCalls; moveClock.numCalls--)
		{
			if (WORLD->debugmap->inMapBounds(nextPosition))
			{
				int decel = WORLD->debugmap->getBlock(nextPosition)->tileList[mapPosition.h].deceleration;
				if (velocity - decel < 0)	velocity = 0;
				else						velocity -= decel;

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

				if (velocity > 0) fTravel.stepLine();

				mapPosition = Position4(fTravel.getPosition().x, fTravel.getPosition().y, mapPosition.h, startPosition.z);
				nextPosition = Position4(fTravel.getNextPosition().x, fTravel.getNextPosition().y, mapPosition.h, mapPosition.z);
			}
			else
			{
				velocity = 0;
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

	renderPosition = getRenderPosition(mapPosition);
}

void Bullet::render(const Pane& pane) const
{
	if (inFov)
	{
		if (velocity > 0)
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

