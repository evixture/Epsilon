#include "main.hpp"

AICreature::AICreature(Creature creature, TCODMap* fovMap)
	:Creature(creature), path(TCODPath(fovMap)), moveSpeedMode(1), debugBGColor(TCODColor::black), interest(0.0f), interestDecay(.05f), interestDecayClock(1.0f), pathStep(0), reactionFireClock(1.0f)
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
		//if (!path.isEmpty()) //has path to walk on
		//{
			if (interest >= 0.5f) //change not in fov for more context, to allow movement in combat, etc, replace with if not in weapon effective range
			{
				if (WORLD->isInPlayerFov(mapPosition))
				{
					if (!inEffectiveRange())
					{
						WORLD->updateBlock(mapPosition, false);

						path.walk(&mapPosition.x, &mapPosition.y, true);

						WORLD->updateBlock(mapPosition, true);

						debugBGColor = TCODColor::green;
					}
				}
				else
				{
					WORLD->updateBlock(mapPosition, false);

					path.walk(&mapPosition.x, &mapPosition.y, true);

					WORLD->updateBlock(mapPosition, true);

					debugBGColor = TCODColor::blue;
				}
			}
		//}
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

bool AICreature::inEffectiveRange()
{
	if (getDistance(mapPosition.x, mapPosition.y, focusPosition.x, focusPosition.y) <= selectedMagazine->velocity * 0.15f) //change to take into account melee weapons
	{
		return true;
	}
	return false;
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

				//pathfind to sound source
				path.compute(mapPosition.x, mapPosition.y, pathfindPosition.x, pathfindPosition.y);
			}
		}
	}
}

void AICreature::update() //ai and behavior attributes update here
{
	/*
	
	bullet with 80 speed travels 30 tiles before falling (0.35)

	range is the max distance from a creature that the selected weapon is effective
	range for weapon = velocity x .35
	
	
	*/
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

			//reactionFireClock.tickUp(); //replace later with something with more discretion
			//for (int i = 1; i <= reactionFireClock.numCalls; reactionFireClock.numCalls--)
			//{
			//	//selectedItem->tool->use(false, true); //put on clock
			//}
		}
		else //if player not in fov
		{
			//pathfind to last known player positiom
			path.compute(mapPosition.x, mapPosition.y, focusPosition.x, focusPosition.y); //change later to not update every frame

			//debugBGColor = TCODColor::black;
		}

		updateTools();

		//reload on empty mag
		if (selectedMagazine->isValid == false || selectedMagazine->availableAmmo <= 0)
		{
			std::shared_ptr<MagazineData> mag = std::make_shared<MagazineData>(MagazineData::AmmoType::FOURTYFIVEACP, 7, 7, true); //later check for mag in inventory
			selectedMagazine = mag;
			
			selectedItem->tool->reload(mag);
		}

		if (path.isEmpty() || path.size() == 0)
		{
			debugBGColor = TCODColor::red;

		}
			path.compute(mapPosition.x, mapPosition.y, lookPosition.x - 1, lookPosition.y); // problem does not work??

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
	if (WORLD->debugmap->player->mapPosition.floor == mapPosition.floor)
	{
		pane->console->setChar(renderPosition.x, renderPosition.y, ch);
		pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
		pane->console->setCharBackground(renderPosition.x, renderPosition.y, debugBGColor);

		//render interest points
		pane->console->setCharBackground(lookPosition.x - WORLD->xOffset, lookPosition.y - WORLD->yOffset, TCODColor::white);
		pane->console->setCharBackground(focusPosition.x - WORLD->xOffset, focusPosition.y - WORLD->yOffset - 1, TCODColor::flame);
		pane->console->setCharBackground(pathfindPosition.x - WORLD->xOffset, pathfindPosition.y - WORLD->yOffset - 2, TCODColor::purple);

		int x, y;
		for (int i = 0; i < path.size(); i++)
		{
			path.get(i, &x, &y);

			pane->console->setCharBackground(x - WORLD->xOffset, y - WORLD->yOffset, TCODColor::pink);
		}

		if (health != 0)
		{
			selectedItem->renderTool(pane); //want to fix, selected item is still rendered when dead
		}
	}
}


