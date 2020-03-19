#include "main.hpp"

AICreature::AICreature(Creature creature, TCODMap* fovMap)
	:Creature(creature), path(TCODPath(fovMap)), moveSpeedMode(1), debugBGColor(TCODColor::black), interest(0.0f), interestDecay(.05f), interestDecayClock(1.0f), pathStep(0), reactionFireClock(0.5f)
{
	inventory.push_back(std::make_shared<Container>(ep::container::smallBackpack(0, 0, 0, this)));
	inventory[1]->addItem(std::make_shared<Item>(ep::item::sip45(0, 0, 0, this)));
	inventory[1]->addItem(std::make_shared<Item>(ep::item::cal45Magazine7(0, 0, 0, this)));

	selectedItem = inventory[1]->itemList[0];
}

void AICreature::move()
{
	if (moveSpeedMode == 2)			baseMoveTime = .25f; //sprint
	else if (moveSpeedMode == 0)	baseMoveTime = 1.0f; //creep
	else							baseMoveTime = .5f;	 //walk

	moveSpeed = baseMoveTime / mapPosition.height;

	moveClock.timeBetweenUpdates = moveSpeed;
	moveClock.tickUp();

	for (int i = 1; i <= moveClock.numCalls; moveClock.numCalls--)
	{
		if (!path.isEmpty()) //has path to walk on
		{
			if (interest >= 0.5f && !WORLD->isInPlayerFov(mapPosition)) //change not in fov for more context, to allow movement in combat, etc, replace with if not in weapon effective range
			{
				WORLD->updateBlock(mapPosition, false);

				path.walk(&mapPosition.x, &mapPosition.y, true);

				WORLD->updateBlock(mapPosition, true);
			}
		}
	}
}


void AICreature::pickUpItem()
{
}

void AICreature::dropItem()
{
}

void AICreature::reload()
{
}

void AICreature::changeFireMode()
{
}

void AICreature::equipArmor()
{
}

void AICreature::useMelee()
{
}

void AICreature::updateTools()
{
	angle = getAngle(mapPosition.x, mapPosition.y, lookPosition.x, lookPosition.y);

	selectedItem->updateTool(mapPosition, lookPosition.x, lookPosition.y, true);
}

void AICreature::decayInterest()
{
	interestDecayClock.tickUp();

	for (int i = 1; i <= interestDecayClock.numCalls; interestDecayClock.numCalls--)
	{
		if (interest - interestDecay < 0)
		{
			interest = 0;
		}
		else
		{
			interest -= interestDecay;
		}
	}
}

void AICreature::reactToSounds()
{
	if (!WORLD->isInPlayerFov(mapPosition))
	{
		for (auto& sound : WORLD->soundList)
		{
			if (sound->sourcePosition.floor == mapPosition.floor)
			{
				double distance = getDistance(mapPosition.x, mapPosition.y, sound->sourcePosition.x, sound->sourcePosition.y);

				if (distance > 0.0f)
				{
					//interestChange = (4.0f / (distance - 3.0f)); OLD VOLUME EQUATION
					interestChange = (float)((15.0f / (distance + 30.0f)) * (sound->worldVolume / 50.f));
				}
				else
				{
					interestChange = 0.0f;
				}

				if (interestChange >= 0.5f)
				{
					if (interest + interestChange >= 1.0f)
					{
						interest = 1;
					}
					else
					{
						interest += interestChange;
					}
				}
			}

			if (interestChange >= 0.5f)
			{
				pathfindPosition = sound->sourcePosition;
				lookPosition = sound->sourcePosition;

				path.compute(mapPosition.x, mapPosition.y, pathfindPosition.x, pathfindPosition.y);
			}
		}
	}
}

void AICreature::update() //ai and behavior attributes update here
{
	renderPosition = Position3(offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset));

	if (health != 0) //if alive
	{
		if (INPUT->debug2Key->isSwitched) //debug pathfinding
		{	
			//interest = 1.0f;
			//pathfindPosition = (Position3(INPUT->mouse.cx - 1 + WORLD->xOffset, INPUT->mouse.cy - 3 + WORLD->yOffset, mapPosition.floor));
			//lookPosition = (Position3(INPUT->mouse.cx - 1 + WORLD->xOffset, INPUT->mouse.cy - 3 + WORLD->yOffset, mapPosition.floor));
			//
			//path.compute(mapPosition.x, mapPosition.y, pathfindPosition.x, pathfindPosition.y);

			//fire / reload
			//if (selectedMagazine->isValid == false)
			//{
			//	std::shared_ptr<MagazineData> mag = std::make_shared<MagazineData>(MagazineData::AmmoType::FOURTYFIVEACP, 7, 7, true);
			//	selectedMagazine = mag;
			//
			//	selectedItem->tool->reload(mag);
			//}
			//else
			//{
			//	selectedItem->tool->use(false, true);
			//}
		}

		decayInterest();
		reactToSounds();

		//should it check for all creatures if they are in its fov? if so, need to find out how to make individual fov maps
		if (WORLD->isInPlayerFov(mapPosition)) //if the player is in fov
		{
			interest = 1.0f; //set interest to max if player in fov

			//if focus position != player map position //creature's last frame of reference for player has changed, reacts with confusion? or other
			focusPosition = WORLD->debugmap->player->mapPosition;

			lookPosition = WORLD->debugmap->player->mapPosition; //add random coords (1, -1) for inaccuracy

			//static TCODRandom* RNG = TCODRandom::getInstance();
			//if (!RNG)
			//{
			//	RNG->setDistribution(TCOD_DISTRIBUTION_LINEAR);
			//}
			//
			//lookPosition = Position3(WORLD->debugmap->player->mapPosition.x + RNG->getInt(-1, 1), WORLD->debugmap->player->mapPosition.y + RNG->getInt(-1, 1), WORLD->debugmap->player->mapPosition.floor);
			debugBGColor = TCODColor::yellow;

			//---

			reactionFireClock.tickUp(); //replace later with something with more discretion
			for (int i = 1; i <= reactionFireClock.numCalls; reactionFireClock.numCalls--)
			{
				selectedItem->tool->use(false, true); //put on clock
			}
		}
		else //if player not in fov
		{
			path.compute(mapPosition.x, mapPosition.y, focusPosition.x, focusPosition.y); //change later to not update every frame

			debugBGColor = TCODColor::black;
		}

		updateTools();

		//reload on empty mag
		if (selectedMagazine->isValid == false)
		{
			std::shared_ptr<MagazineData> mag = std::make_shared<MagazineData>(MagazineData::AmmoType::FOURTYFIVEACP, 7, 7, true); //later check for mag in inventory
			selectedMagazine = mag;
			
			selectedItem->tool->reload(mag);
		}

		move();
	}
	else
	{
		health = 0; //prevent from taking further damage
		ch = '$'; //set char to dead symbol
		color = TCODColor::red;
	}
}

void AICreature::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(renderPosition.x, renderPosition.y, ch);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
	pane->console->setCharBackground(renderPosition.x, renderPosition.y, debugBGColor);

	if (health != 0)
	{
		selectedItem->renderTool(pane); //want to fix, selected item is still rendered when dead
	}
}


