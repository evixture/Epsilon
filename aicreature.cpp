#include "main.hpp"

AICreature::AICreature(Creature creature, TCODMap* fovMap)
	:Creature(creature), path(TCODPath(fovMap)), moveSpeedMode(1), debugBGColor(TCODColor::black), interest(0.0f), interestDecay(.025f), interestDecayClock(1.0f), pathStep(0)
{
	inventory.push_back(CONTAINER_SmallBackpack(0, 0, 0, this));
	inventory[0]->addItem(ITEM_SIP45(0, 0, 0, this));
	inventory[0]->addItem(std::make_shared<Item>(ITEM_45Magazine7(0, 0, 0, this)));

	selectedItem = inventory[0]->itemList[0];
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
			WORLD->updateBlock(mapPosition, false);

			path.walk(&mapPosition.x, &mapPosition.y, true);

			WORLD->updateBlock(mapPosition, true);		
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
	angle = getAngle(mapPosition.x, mapPosition.y, destX, destY);

	selectedItem->updateTool(mapPosition, destX, destY, true);
}

void AICreature::reactToSounds()
{
	for (auto& sound : WORLD->soundList)
	{
		if (sound->soundSource.floor == mapPosition.floor)
		{
			double distance = getDistance(mapPosition.x, mapPosition.y, sound->soundSource.x, sound->soundSource.y);

			if (distance > 0.0f)
			{
				//interestChange = (4.0f / (distance - 3.0f)); OLD VOLUME EQUATION
				interestChange = ((15.0f / (distance + 30.0f)) * (sound->worldVolume / 50.f));
			}
			else
			{
				interestChange = 0.0f;
			}

			if (interest + interestChange > 1)
			{
				interest = 1;
			}
			else
			{
				interest += interestChange;
				interestChange = 0;
			}
		}

		if (interest > .5f)
		{
			destX = sound->soundSource.x;
			destY = sound->soundSource.y;

			path.compute(mapPosition.x, mapPosition.y, destX, destY); //will compute multiple times?
		}
	}
}

void AICreature::update() //ai and behavior attributes update here
{
	renderPosition = Position3(offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset));

	if (health > 0)
	{
		if (INPUT->debug2Key->isSwitched) //debug pathfinding
		{	
			destX = INPUT->mouse.cx - 1 + WORLD->xOffset;
			destY = INPUT->mouse.cy - 3 + WORLD->yOffset;

			path.compute(mapPosition.x, mapPosition.y, destX, destY);
		}

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

		reactToSounds();

		if (WORLD->isInPlayerFov(mapPosition)) //should it check for all creatures if they are in its fov? if so, need to find out how to make individual fov maps
		{
			debugBGColor = TCODColor::yellow;
		}
		else debugBGColor = TCODColor::black;

		move();

		updateTools();
	}

	if (!(health > 0)) //if not "alive"
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

	if (health > 0)
	{
		selectedItem->renderTool(pane); //want to fix, selected item is still rendered when dead
	}
}


