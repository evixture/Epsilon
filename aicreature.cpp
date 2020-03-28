#include "main.hpp"

AICreature::AICreature(Creature creature, TCODMap* fovMap)
	:Creature(creature), path(TCODPath(fovMap)), moveSpeedMode(1), debugBGColor(TCODColor::black), soundInterest(0.0f), visualInterest(0.0f), interestDecay(.05f), interestDecayClock(1.0f), 
	pathStep(0), reactionFireClock(1.0f), aggression(0.0f), inFov(false)
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
		if (soundInterest >= 0.5f || visualInterest >= 0.5f) //change not in fov for more context, to allow movement in combat, etc, replace with if not in weapon effective range
		{
			if (inFov) //move check to act(), and allow move to be called around in infov and outfov
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
		//else nav idly around the map?
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

void AICreature::takeDamage(int damage)
{
	if (health - damage > 0) //take normal damage
	{
		health -= damage;
	}
	else //if damage taken would have resulted in death
	{
		health = 0;
	}

	aggression += damage / 100.0f;
	aggression = std::clamp<float>(aggression, -1.0f, 1.0f);
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
		if (soundInterest - interestDecay < 0)
		{
			soundInterest = 0;
		}
		else
		{
			soundInterest -= interestDecay;
		}

		if (visualInterest - interestDecay < 0)
		{
			visualInterest = 0;
		}
		else
		{
			visualInterest -= interestDecay;
		}
	}
}

void AICreature::reactToSounds()
{
	if (!inFov)
	{
		for (auto& sound : WORLD->soundList)
		{
			if (sound->sourcePosition.floor == mapPosition.floor)
			{
				float soundInterestChange;
				double distance = getDistance(mapPosition.x, mapPosition.y, sound->sourcePosition.x, sound->sourcePosition.y);

				if (distance > 0.0f)
				{
					soundInterestChange = (float)((15.0f / (distance + 30.0f)) * (sound->worldVolume / 50.f));
				}
				else
				{
					soundInterestChange = 0.0f;
				}

				if (soundInterestChange >= 0.5f)
				{
					if (soundInterest + soundInterestChange >= 1.0f)
					{
						soundInterest = 1.0f;
					}
					else
					{
						soundInterest += soundInterestChange;
					}

					pathfindPosition = sound->sourcePosition;
					lookPosition = sound->sourcePosition;

					//pathfind to sound source
					path.compute(mapPosition.x, mapPosition.y, pathfindPosition.x, pathfindPosition.y);
				}
			}
		}
	}
}

/*
(NOT ENTIRELY ACCURATE)

AUDIO FIELD
  ---
 -----
---E---
 -----
  ---

VISUAL FIELD
      ++
    ++++
E ++++++
    ++++
	  ++

COMBINED
  ---   ++
 -----++++
---E++++++
 -----++++
  ---   ++
*/

void AICreature::behave()
{
	if (INPUT->debug2Key->isSwitched) //debug
	{
	}

	decayInterest();
	reactToSounds();

	if (inFov)
	{
		float calcVisInt = std::clamp<float>((15.0f / (float)getDistance(mapPosition.x, mapPosition.y, WORLD->debugmap->player->mapPosition.x, WORLD->debugmap->player->mapPosition.y)), 0.0f, 1.0f);
		if (calcVisInt > visualInterest)
		{
			visualInterest = calcVisInt; //update visual interest with the greater value
		}

		focusPosition = WORLD->debugmap->player->mapPosition;
		lookPosition = WORLD->debugmap->player->mapPosition; //add random coords (1, -1) for inaccuracy
		pathfindPosition = getWalkableArea(WORLD->debugmap->player->mapPosition); //player tile is not walkable
	}
	else
	{
		//pathfind to last known player positiom
	}
	path.compute(mapPosition.x, mapPosition.y, pathfindPosition.x, pathfindPosition.y); //change later to not update every frame
}

void AICreature::act()
{
	updateTools();

	if (inFov)
	{
		reactionFireClock.tickUp(); //replace later with something with more discretion
		for (int i = 1; i <= reactionFireClock.numCalls; reactionFireClock.numCalls--)
		{
			if (aggression >= 0.5f)
			{
				selectedItem->tool->use(false, true); //put on clock
			}
		}
	}

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

	move();
}

void AICreature::update() //ai and behavior attributes update here
{
	inFov = WORLD->isInPlayerFov(mapPosition);
	renderPosition = Position3(offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset));

	if (health != 0) //if alive
	{
		GUI->logWindow->pushMessage(LogWindow::Message("vi " + std::to_string(visualInterest), LogWindow::Message::MessageLevel::MEDIUM));
		GUI->logWindow->pushMessage(LogWindow::Message("si " + std::to_string(soundInterest), LogWindow::Message::MessageLevel::MEDIUM));

		behave(); //take in surroundings and change attributes
		act(); //take actions based on attribute values
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
		if (inFov)
		{
			pane->console->setChar(renderPosition.x, renderPosition.y, ch);
			pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
		}
		else
		{
			//pane->console->setChar(renderPosition.x, renderPosition.y, '?'); //should render char out of fov??
			//pane->console->setChar(renderPosition.x, renderPosition.y, '?'); //should render char out of fov??
			pane->console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::darkestGrey);
		}

		if (false) //_DEBUG
		{
			pane->console->setCharBackground(renderPosition.x, renderPosition.y, debugBGColor);

			//render interest points
			pane->console->setCharBackground(lookPosition.x - WORLD->xOffset, lookPosition.y - WORLD->yOffset, TCODColor::white);
			pane->console->setCharBackground(focusPosition.x - WORLD->xOffset, focusPosition.y - WORLD->yOffset - 1, TCODColor::flame);
			pane->console->setCharBackground(pathfindPosition.x - WORLD->xOffset, pathfindPosition.y - WORLD->yOffset - 2, TCODColor::purple);

			//render path
			int x, y;
			for (int i = 0; i < path.size(); i++)
			{
				path.get(i, &x, &y);

				pane->console->setCharBackground(x - WORLD->xOffset, y - WORLD->yOffset, TCODColor::pink);
			}
		}
	
		if (health != 0)
		{
			if (inFov)
			{
				selectedItem->renderTool(pane); //want to fix, selected item is still rendered when dead
			}
		}
	}
}

//NOTES

//distance of 30 at 45 degrees is ~20x and y

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

//should it check for all creatures if they are in its fov? if so, need to find out how to make individual fov maps
//recalculate fov for each creature update??
