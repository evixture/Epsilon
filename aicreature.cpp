#include "main.hpp"

AICreature::AICreature(Creature creature, TCODMap* fovMap)
	:Creature(creature), path(TCODPath(fovMap)), moveSpeedMode(1), debugBGColor(TCODColor::black), interest(0.0f), interestDecay(.025f), interestDecayClock(1.0f)
{
	//selectedItem = std::make_shared<Item>(ITEM_SIP45(0, 0, 0, this));
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
			path.walk(&mapPosition.x, &mapPosition.y, true);

			//call on move
			WORLD->updateBlock(mapPosition);		
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
}

void AICreature::update() //ai and behavior attributes update here
{
	angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx, engine->settings->input->mouse.cy);

	renderPosition = Position3(offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset));

	if (health > 0)
	{
		if (INPUT->debug2Key->isSwitched)																									//debug pathfinding
		{																																	//
			path.compute(mapPosition.x, mapPosition.y, INPUT->mouse.cx - 1 + WORLD->xOffset, INPUT->mouse.cy - 3 + WORLD->yOffset);			//
		}

		interestDecayClock.tickUp();
		for (int i = 1; i <= interestDecayClock.numCalls; interestDecayClock.numCalls--)
		{
			GUI->logWindow->pushMessage(LogWindow::Message(std::to_string(interest), LogWindow::Message::MessageLevel::LOW));

			if (interest - interestDecay < 0)
			{
				interest = 0;
			}
			else
			{
				interest -= interestDecay;
			}
		}

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
				path.compute(mapPosition.x, mapPosition.y, sound->soundSource.x, sound->soundSource.y); //will compute multiple times?
			}
		}

		if (WORLD->isInPlayerFov(mapPosition)) //should it check for all creatures if they are in its fov? if so, need to find out how to make individual fov maps
		{
			debugBGColor = TCODColor::yellow;
		}
		else debugBGColor = TCODColor::black;

		move();
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
		//selectedItem->renderTool(pane); //want to fix, selected item is still rendered when dead
	}
}
