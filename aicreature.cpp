#include "main.hpp"

AICreature::AICreature(Creature creature, TCODMap* fovMap)
	:Creature(creature), path(TCODPath(fovMap, 0.0f))
{
	//selectedItem = std::make_shared<Item>(ITEM_SIP45(0, 0, 0, this));
}

void AICreature::move()
{

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
		if (INPUT->debug2Key->isSwitched)
		{
			path.compute(mapPosition.x, mapPosition.y, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3);
		}

		if (!path.isEmpty())
		{
			path.walk(&mapPosition.x, &mapPosition.y, false);
		}
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

	if (health > 0)
	{
		//selectedItem->renderTool(pane); //want to fix, selected item is still rendered when dead
	}
}
