#include "main.hpp"

AICreature::AICreature(Creature creature, TCODMap* fovMap)
	:Creature(creature), path(TCODPath(fovMap)), moveSpeedMode(1), debugBGColor(TCODColor::black)
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
			path.walk(&mapPosition.x, &mapPosition.y, false);

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
		
		move();

		if (WORLD->isInPlayerFov(mapPosition)) //should it check for all creatures if they are in its fov? if so, need to find out how to make individual fov maps
		{
			debugBGColor = TCODColor::yellow;
		}
		else debugBGColor = TCODColor::black;
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
