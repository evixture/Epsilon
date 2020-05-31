#include "main.hpp"

AICreature::AICreature(Creature creature, TCODMap* fovMap)
	:Creature(creature), path(TCODPath(fovMap)), moveSpeedMode(1), debugBGColor(TCODColor::black), soundInterest(0.0f), visualInterest(0.0f), interestDecay(.05f), interestDecayClock(0.5f), 
	pathStep(0), reactionFireClock(1.0f), aggression(0.0f), inFov(false)
{
	inventory.push_back(std::make_shared<Container>(ep::container::smallBackpack(0, 0, 0)));
	inventory[1]->addItem(std::make_shared<Item>(ep::item::sip45(0, 0, 0)));
	inventory[1]->addItem(std::make_shared<Item>(ep::item::cal45Magazine7(0, 0, 0)));

	selectedItem = inventory[1]->itemList[0];
}

void AICreature::move()
{
	static int stepSound;
	static int stepSpeed;

	if (moveSpeedMode == 2)			baseMoveTime = .25f; //sprint
	else if (moveSpeedMode == 0)	baseMoveTime = 1.0f; //creep
	else							baseMoveTime = .5f;	 //walk

	moveSpeed = baseMoveTime / mapPosition.height;

	moveClock.timeBetweenUpdates = moveSpeed;
	moveClock.tickUp();

	for (int i = 1; i <= moveClock.numCalls; moveClock.numCalls--)
	{
		if (!inEffectiveRange() && (soundInterest >= 0.5f || visualInterest >= 0.5f)) //need to change order if want to change move behavior
		{
			WORLD->updateBlock(mapPosition, false);
			path.walk(&mapPosition.x, &mapPosition.y, true);
			WORLD->updateBlock(mapPosition, true);

			//play footstep sound
			if (baseMoveTime == .25f) stepSpeed = mapPosition.height;
			else stepSpeed = mapPosition.height - 1;
			if (stepSound >= stepSpeed)
			{
				AUDIO->playSound(PositionalTrackedSound(("top"), &mapPosition, 70.0f, 10.0f));
				stepSound = 0;
			}
			else
			{
				stepSound++;
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

void AICreature::takeDamage(int damage)
{
	if (health - damage > 0) //take normal damage
	{
		health -= damage;
		AUDIO->playSound(PositionalTrackedSound(("hoop"), &mapPosition, 75.0f, 120.0f));
	}
	else //if damage taken would have resulted in death
	{
		health = 0;
		AUDIO->playSound(PositionalTrackedSound(("jaw"), &mapPosition, 80.0f, 170.0f));
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
	return ((getDistance(mapPosition.x, mapPosition.y, focusPosition.x, focusPosition.y) <= selectedItem->tool->getMagazine().velocity * 0.15f) && selectedItem->tool->getMagazine().isValid); //change to take into account melee weapons
}

void AICreature::decayInterest()
{
	interestDecayClock.tickUp();

	calcVisInt = std::clamp<float>((30.0f / (float)getDistance(mapPosition.x, mapPosition.y, WORLD->debugmap->player->mapPosition.x, WORLD->debugmap->player->mapPosition.y)), 0.0f, 1.0f);

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

		if (!inFov) //decay out of fov
		{
			if (visualInterest - interestDecay < 0)
			{
				visualInterest = 0;
			}
			else
			{
				visualInterest -= interestDecay;
			}
		}
		else
		{
			if (visualInterest + .25f > calcVisInt)
			{
				visualInterest = calcVisInt;
			}
			else
			{
				visualInterest += 0.25f;
			}
		}
	}
}

void AICreature::reactToSounds()
{
	for (auto& sound : WORLD->soundList)
	{
		if (sound->getPosition().first == true && !(sound->getPosition().second.x == mapPosition.x && sound->getPosition().second.y == mapPosition.x)) //is 3d sound and not from itself
		{
			if (sound->getPosition().second.floor == mapPosition.floor)
			{
				float soundInterestChange;
				double distance = getDistance(mapPosition.x, mapPosition.y, sound->getPosition().second.x, sound->getPosition().second.y);

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
					if (soundInterest < soundInterestChange)
					{
						soundInterest = soundInterestChange;
					}
					
					pathfindPosition = getWalkableArea(sound->getPosition().second);
					lookPosition = sound->getPosition().second;

					//pathfind to sound source
					path.compute(mapPosition.x, mapPosition.y, pathfindPosition.x, pathfindPosition.y);
				}
			}
		}
	}
}


/*
AUDIO FIELD
  ---
 -----
---E---
 -----
  ---

VISUAL FIELD
     ++
   ++++
E++++++
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
	decayInterest();
	reactToSounds();

	if (inFov)
	{
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
			if (aggression >= 0.5f) selectedItem->tool->use(false, true);
		}
	}

	//reload on empty mag
	if (selectedItem->tool->getMagazine().availableAmmo <= 0)
	{
		auto reloadMag = MagazineData(MagazineData::AmmoType::FOURTYFIVEACP, 7, 7, true);
		
		selectedItem->tool->reload(reloadMag);
	}

	if (path.isEmpty() || path.size() == 0) debugBGColor = TCODColor::red;

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

void AICreature::render(const Pane& pane) const
{
	if (WORLD->debugmap->player->mapPosition.floor == mapPosition.floor)
	{
		if (inFov)
		{
			pane.console->setChar(renderPosition.x, renderPosition.y, ch);
			pane.console->setCharForeground(renderPosition.x, renderPosition.y, color);
		}
		else
		{
			pane.console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::darkestGrey);
		}

		if (false) //show pathfinding information
		{
			//render path
			int x, y;
			for (int i = 0; i < path.size(); i++)
			{
				path.get(i, &x, &y);

				pane.console->setCharBackground(x - WORLD->xOffset, y - WORLD->yOffset, TCODColor::pink);
			}
			pane.console->setCharBackground(renderPosition.x, renderPosition.y, debugBGColor);

			//render interest points
			//pane.console->setCharBackground(lookPosition.x - WORLD->xOffset, lookPosition.y - WORLD->yOffset, TCODColor::white);
			//pane.console->setCharBackground(focusPosition.x - WORLD->xOffset, focusPosition.y - WORLD->yOffset - 1, TCODColor::flame);
			pane.console->setCharBackground(pathfindPosition.x - WORLD->xOffset, pathfindPosition.y - WORLD->yOffset, TCODColor::purple);
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
