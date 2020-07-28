#include "main.hpp"

AICreature::AICreature(Creature creature, TCODMap* fovMap)
	:Creature(creature), path(TCODPath(fovMap)), moveSpeedMode(1), debugBGColor(TCODColor::black), soundInterest(0.0f), visualInterest(0.0f), interestDecay(.05f), interestDecayClock(0.5f), 
	pathStep(0), reactionFireClock(1.0f), aggression(0.0f), inFov(false), attitude(0), actionClock(Clock(1.0f)), actionIndex(0), lastKnownMapPosition(creature.mapPosition), discovered(false)
{
	inventory = ep::inventory::testInventory(this);

	selectedItem = inventory[0]->item; //select hands
}

void AICreature::move()
{
	/*
		STANCE AND SPEED  WITH ATTRIBUTES:
			default movement is walking, but sprint active if far from effective range and creep active if stealth
			default stance is standing, but lower if stealth
	*/

	static int stepSound;
	static int stepSpeed;

	if (moveSpeedMode == 2)			baseMoveTime = .25f; //sprint
	else if (moveSpeedMode == 0)	baseMoveTime = 1.0f; //creep
	else							baseMoveTime = .5f;	 //walk

	moveSpeed = baseMoveTime / mapPosition.h;

	moveClock.timeBetweenUpdates = moveSpeed;
	moveClock.tickUp();

	for (int i = 1; i <= moveClock.numCalls; moveClock.numCalls--)
	{
		if (((!inEffectiveRange() && inFov) || !inFov) && (soundInterest >= 0.5f || visualInterest >= 0.5f)) //need to change order if want to change move behavior
		{
			WORLD->updateBlock(mapPosition, false);
			path.walk(&mapPosition.x, &mapPosition.y, true);
			WORLD->updateBlock(mapPosition, true);

			//play footstep sound
			if (baseMoveTime == .25f) stepSpeed = mapPosition.h;
			else stepSpeed = mapPosition.h - 1;
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


bool AICreature::pickUpItem()
{
	for (auto& item : WORLD->debugmap->mapItemList)
	{
		if (item->onMap && item->mapPosition.x == mapPosition.x && item->mapPosition.y == mapPosition.y && item->mapPosition.z == mapPosition.z)
		{
			if (item->pickUp(this)) //got picked up
			{
				GUI->logWindow->pushMessage(Message("Picked up " + item->tool->name, Message::MessageLevel::MEDIUM));
				AUDIO->playSound(PositionalTrackedSound(("pick up"), &mapPosition, 60.0f, 30.0f));

				return true;
			}
		}
	}

	for (auto& container : WORLD->debugmap->mapContainerList)
	{
		if (container->item->onMap && container->item->mapPosition.x == mapPosition.x && container->item->mapPosition.y == mapPosition.y && container->item->mapPosition.z == mapPosition.z)
		{
			container->pickUp(this);

			GUI->logWindow->pushMessage(Message("Picked up " + container->item->tool->name, Message::MessageLevel::MEDIUM));
			AUDIO->playSound(PositionalTrackedSound(("pick up"), &mapPosition, 60.0f, 30.0f));

			return true;
		}
	}

	return false;
}

void AICreature::dropItem() //prob here
{
	if (containerIndex != -1)
	{
		if (itemIndex >= 0)
		{
			AUDIO->playSound(PositionalTrackedSound(("drop"), &mapPosition, 65.0f, 30.0f));

			selectedItem->drop();
		}
		else if (itemIndex <= -1)
		{
			if (selectedItem->type != Item::ItemType::HAND)
			{
				AUDIO->playSound(PositionalTrackedSound(("drop"), &mapPosition, 65.0f, 30.0f));

				inventory[containerIndex]->drop(this);
			}
		}
	}
}

bool AICreature::reload()
{
	for (auto& container : inventory)
	{
		for (auto& item : container->itemList)
		{
			if (item->getMagazineData().first == true) // if it is actually a magazine
			{
				if (item->getMagazineData().second.ammoType == selectedItem->tool->ammoType) // if it has the same type of ammo as the current weapon
				{
					if (item->getMagazineData().second.availableAmmo != 0) // if the magazine is not empty
					{
						return selectedItem->reload(item->getMagazineData().second); //returns true on a proper reload
					}
				}
			}
		}
	}
	return false;
}

void AICreature::changeFireMode()
{
	selectedItem->changeFireMode();
}

void AICreature::equipArmor()
{
	selectedItem->equip(equippedArmor); //bool return??
}

void AICreature::useMelee()
{
	selectedItem->useMelee();
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

		//do when killed
		for (int c = (int)inventory.size() - 1; c > 0; c--) //drop items on death, but not hands (c == 0)
		{
			inventory[c]->item->updateTool(mapPosition, targetPosition.x, targetPosition.y, false);
			WORLD->debugmap->mapContainerList.push_back(inventory[c]);
			inventory.erase(inventory.begin() + c);
		}
	}

	aggression += damage / 200.0f;
	aggression = std::clamp<float>(aggression, attitude, 1.0f); //possible to kill friendlies (0 att), can almost kill neutrals (64 att)
}

void AICreature::updateTools()
{
	angle = getAngle(mapPosition.x, mapPosition.y, targetPosition.x, targetPosition.y);
	selectedItem->updateTool(mapPosition, targetPosition.x, targetPosition.y, true);
}

void AICreature::moveSelectorUp()
{
	if (!(itemIndex == -1 && containerIndex == -1)) // if not hands
	{
		if (itemIndex > -1)
		{
			itemIndex--;
		}
		else if (itemIndex == -1)
		{
			if (containerIndex > 0)
			{
				containerIndex--;
				itemIndex = (int)(inventory[containerIndex]->itemList.size() - 1);
			}
		}
		else if (itemIndex == -2)
		{
			if (itemIndex + 3 <= inventory[containerIndex]->itemList.size())
			{
				++itemIndex;
			}
		}
	}
}

void AICreature::moveSelectorDown()
{
	if (!(itemIndex == -1 && containerIndex == -1)) //if not hands
	{
		if (itemIndex + 1 < inventory[containerIndex]->itemList.size())
		{
			++itemIndex;
		}
		else if (itemIndex + 1 >= inventory[containerIndex]->itemList.size())
		{
			if (containerIndex < inventory.size() - 1)
			{
				++containerIndex;
				itemIndex = -1;
			}
			else if (itemIndex == -1)
			{
				if (itemIndex + 2 <= inventory[containerIndex]->itemList.size())
				{
					++itemIndex;
				}
			}
		}
	}
}

void AICreature::filterIndexes()
{
	//INDEX FILTERING
	//
	//ITEM INDEXES
	//	-2 : no item selected
	//	-1 : container selected
	//	0+ : item in vector

	if (containerIndex + 1 > (inventory.size()))
	{
		containerIndex = (int)(inventory.size() - 1);
	}
	else if (containerIndex == -1 && inventory.size() > 0)
	{
		containerIndex = 0;
	}

	if (containerIndex != -1)
	{
		if (itemIndex != -2)
		{
			if (itemIndex + 1 > inventory[containerIndex]->itemList.size())
			{
				itemIndex = (int)(inventory[containerIndex]->itemList.size() - 1);
			}

			if (itemIndex != -1)
			{
				selectedItem = inventory[containerIndex]->itemList[itemIndex];
			}
			else if (itemIndex == -1)
			{
				selectedItem = inventory[containerIndex]->item;
			}
		}
	}
}

void AICreature::selectAppropriateItem(float range)
{
	bool canFindOptimal = false;
	float deltaRange = 100;
	int cInd = 999;
	int iInd = 999;

	if (attitude < .25f) //if not very aggressive, keep hands out
	{
		cInd = 0;
		iInd = -1;
		canFindOptimal = true;
	}
	else
	{
		for (int c = 0; c < inventory.size(); c++)
		{
			for (int i = 0; i < inventory[c]->itemList.size(); i++)
			{
				if (abs(range - inventory[c]->itemList[i]->tool->effectiveRange) < deltaRange) //filter through items to find one with the closest range
				{
					deltaRange = abs(range - inventory[c]->itemList[i]->tool->effectiveRange);
					cInd = c;
					iInd = i;
					canFindOptimal = true;
				}
			}
		}
	}

	if (canFindOptimal) //a check to make sure that there is a valid item and the indexes for it are in range
	{
		std::shared_ptr<Item> optimalItem;
		if		(iInd == -1) optimalItem = inventory[cInd]->item; //container item
		else	optimalItem = inventory[cInd]->itemList[iInd]; //actual item

		/*
			hands		[0, -1]	[0]
			containerA	[1, -1]	[1]
				tool	[1, 0]  [1, 0]
				knife	[1, 1]	[1, 1]	=
			containerB	[2, -1]	[2]
				gun		[2, 0]	[2, 0]	*
		*/

		while (optimalItem != selectedItem) //optimize check
		{
			if (containerIndex == cInd)
			{
				if (itemIndex == iInd || iInd == -1) return; //end search if item or container item found
				else if (itemIndex > iInd) moveSelectorUp();
				else if (itemIndex < iInd) moveSelectorDown();
			}
			else if (containerIndex > cInd) moveSelectorUp();
			else if (containerIndex < cInd) moveSelectorDown();
		}
		canFindOptimal = false;
	}
}

bool AICreature::inEffectiveRange()
{
	return ((getDistance(mapPosition.x, mapPosition.y, focusPosition.x, focusPosition.y) <= selectedItem->tool->effectiveRange)); //change to take into account melee weapons
}

void AICreature::decayInterest()
{
	interestDecayClock.tickUp();

	calcVisInt = std::clamp<float>((30.0f / (float)getDistance(mapPosition.x, mapPosition.y, WORLD->debugmap->player->mapPosition.x, WORLD->debugmap->player->mapPosition.y)), 0.0f, 1.0f);

	for (int i = 1; i <= interestDecayClock.numCalls; interestDecayClock.numCalls--)
	{
		if (soundInterest - interestDecay < 0)	soundInterest = 0;
		else									soundInterest -= interestDecay;

		if (!inFov) //decay out of fov
		{
			if (visualInterest - interestDecay < 0) visualInterest = 0;
			else									visualInterest -= interestDecay;
		}
		else
		{
			if (visualInterest + .25f > calcVisInt) visualInterest = calcVisInt;
			else									visualInterest += 0.25f;
		}
	}
}

void AICreature::reactToSounds()
{
	for (auto& sound : WORLD->soundList)
	{
		if (sound->getPosition().first == true && !(sound->getPosition().second.x == mapPosition.x && sound->getPosition().second.y == mapPosition.x)) //is 3d sound and not from itself
		{
			if (sound->getPosition().second.z == mapPosition.z)
			{
				float soundInterestChange;
				double distance = getDistance(mapPosition.x, mapPosition.y, sound->getPosition().second.x, sound->getPosition().second.y);

				if (distance > 0.0f) soundInterestChange = (float)((15.0f / (distance + 30.0f)) * (sound->worldVolume / 50.f));
				else soundInterestChange = 0.0f;

				if (soundInterestChange >= 0.5f)
				{
					if (soundInterest < soundInterestChange) soundInterest = soundInterestChange;
					
					pathfindPosition = getWalkableArea(sound->getPosition().second);
					targetPosition = sound->getPosition().second;

					//pathfind to sound source
					path.compute(mapPosition.x, mapPosition.y, pathfindPosition.x, pathfindPosition.y);
				}
			}
		}
	}
}

void AICreature::behave()
{
	decayInterest();
	reactToSounds();

	if (inFov)
	{
		focusPosition = WORLD->debugmap->player->mapPosition;
		targetPosition = WORLD->debugmap->player->mapPosition; //add random coords (1, -1) for inaccuracy
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
	Position3 deltaStance = this->stance - WORLD->debugmap->player->stance;
	if (deltaStance.x >= 0) if (attitude < deltaStance.x / 255.0f) attitude = deltaStance.x / 255.0f;
	if (deltaStance.y >= 0) if (attitude < deltaStance.y / 255.0f) attitude = deltaStance.y / 255.0f;
	if (deltaStance.z >= 0) if (attitude < deltaStance.z / 255.0f) attitude = deltaStance.z / 255.0f;
	if (aggression < attitude) aggression = attitude; //keep aggression updated

	if (inFov)
	{
		reactionFireClock.tickUp(); //replace later with something with more discretion
		for (int i = 1; i <= reactionFireClock.numCalls; reactionFireClock.numCalls--)
		{
			if (aggression >= 0.5f) selectedItem->use(false, true); //attack
		}
	}

	if (path.isEmpty() || path.size() == 0) debugBGColor = TCODColor::red;

	actionClock.tickUp();
	for (int i = 1; i <= actionClock.numCalls; actionClock.numCalls--)
	{
		/*
			Behaviors to add
				pick up useful tools
				equip armor if better
		*/

		if (selectedItem->tool->getMagazine().first == true && selectedItem->tool->getMagazine().second.availableAmmo <= 0)
		{
			if (!reload()) //reload if possible, else drop the weapon
			{
				dropItem();
				filterIndexes();
			}
		}

		if (actionIndex % 5 == 0) //every 5 seconds
		{
			selectAppropriateItem((float)getDistance(mapPosition.x, mapPosition.y, focusPosition.x, focusPosition.y));
			filterIndexes();
		}

		if (actionIndex > 10) actionIndex = 1; //tick up every second, reset after 9
		else actionIndex++;
	}

	filterIndexes();
	updateTools();

	move();
}

void AICreature::update() //ai and behavior attributes update here
{
	inFov = WORLD->isInPlayerFov(mapPosition);
	if (inFov) discovered = true;

	if (health != 0) //if alive
	{
		behave(); //take in surroundings and change attributes
		act(); //take actions based on attribute values
	}
	else
	{
		health = 0; //prevent from taking further damage
		ch = '$'; //set char to dead symbol
		color = TCODColor::red;
	}

	if (inFov) lastKnownMapPosition = mapPosition; //need renderPosition?

	renderPosition = getRenderPosition(lastKnownMapPosition);
}

void AICreature::render(const Pane& pane) const
{
	if (WORLD->debugmap->player->mapPosition.z == mapPosition.z)
	{
		if (discovered)
		{
			if (inFov)
			{
				pane.console->setChar(renderPosition.x, renderPosition.y, ch);
				pane.console->setCharForeground(renderPosition.x, renderPosition.y, color);
			}
			else
			{
				pane.console->setCharForeground(renderPosition.x, renderPosition.y, TCODColor::darkerGrey);
				pane.console->setChar(renderPosition.x, renderPosition.y, '?');
			}
		}

		if (SETTINGS->showAIMind) //show pathfinding information
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
			if (inFov) selectedItem->render(pane);
		}
	}
}
