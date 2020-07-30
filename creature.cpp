#include "main.hpp"

Creature::Creature(Position4 pos, int ch, std::string name, Position3 stance, int health, Armor armor)
	:Entity(pos, ch, name, TCODColor(stance.x, stance.y, stance.z)), health(health), equippedArmor(armor), angle(0), containerIndex(0), itemIndex(0),
	moveClock(0), moveSpeed(0), baseMoveTime(0.0f), stance(stance)
{
	inventory.push_back(std::make_shared<Container>(ep::container::hands(this, 0, 0, 0)));
	selectedItem = inventory[0]->item;
}

void Creature::move()
{
}

void Creature::changeStanceUp()
{
	if (mapPosition.h + 1 <= 3)
	{
		if (WORLD->debugmap->getWalkability(Position4(mapPosition.x, mapPosition.y, mapPosition.h + 1, mapPosition.z), false)) mapPosition.h += 1;
	}
}

void Creature::changeStanceDown()
{
	if (mapPosition.h - 1 >= 1)
	{
		if (WORLD->debugmap->getWalkability(Position4(mapPosition.x, mapPosition.y, mapPosition.h - 1, mapPosition.z), false)) mapPosition.h -= 1;
	}
}

bool Creature::pickUpItem()
{
	return false;
}

void Creature::dropItem()
{
}

void Creature::throwItem()
{
}

void Creature::takeDamage(int damage)
{
	if (health - damage > 0) //take normal damage
	{
		health -= damage;
	}
	else //if damage taken would have resulted in death
	{
		health = 0;
	}
}
	

bool Creature::reload()
{
	return false;
}

void Creature::changeFireMode()
{
}

void Creature::equipArmor()
{
}

void Creature::useMelee()
{
}

void Creature::updateTools()
{
}

void Creature::update()
{
	angle = getAngle(renderPosition.x, renderPosition.y, INPUT->mouse->screenPosition.x, INPUT->mouse->screenPosition.y);

	renderPosition = getRenderPosition(mapPosition);

	if (health == 0) //if dead
	{
		ch = '$'; //set char to dead symbol
		color = TCODColor::red;
	}
}

void Creature::render(const Pane& pane) const
{
	if (WORLD->debugmap->player->mapPosition.z == mapPosition.z)
	{
		pane.console->setChar(renderPosition.x, renderPosition.y, ch);
		pane.console->setCharForeground(renderPosition.x, renderPosition.y, (WORLD->isInPlayerFov(mapPosition))? color : TCODColor::darkerGrey); //out of fov creatures rendered with one step above normal fov grey to be more noticible
	}
}

//----------------------------------------------------------------------------------------------------

Player::Player(Position4 position)
	:Creature(position, '@', "player", Position3(255, 0, 255), 100, Armor(this, "", TCODColor::pink, 0, 0)), backgroundColor(TCODColor::pink), hasSecondChance(true)
{
	hasSecondChance = true;

	inventory.push_back(	std::make_shared<Container>(ep::container::smallBackpack(this, 0, 0, 0)));
	inventory[1]->addItem(	std::make_shared<Item>(ep::item::sip45(this, 0, 0, 0)));
	inventory[1]->addItem(	std::make_shared<MagazineItem>(ep::magazineItem::cal45Magazine7(this, 0, 0, 0)));
	inventory[1]->addItem(	std::make_shared<MagazineItem>(ep::magazineItem::cal45Magazine7(this, 0, 0, 0)));
	inventory.push_back(	std::make_shared<Container>(ep::container::smallBackpack(this, 0, 0, 0)));

	if (inventory.size() > 0)
	{
		if (inventory[0]->itemList.size() > 0) selectedItem = inventory[containerIndex]->itemList[itemIndex];
	}
	else
	{
		selectedItem = nullptr;
	}
}

void Player::move()
{
	static int stepSound;
	static int stepSpeed;
	static bool moved = false;

	if (GUI->activeWindow != Gui::ActiveWindow::STARTUPSPLASH)
	{
		if (INPUT->moveFast->isDown)			baseMoveTime = .25f;
		else if (INPUT->moveSlow->isDown)	baseMoveTime = 1.0f;
		else									baseMoveTime = .5f;

		int xMoveDist = 0;
		int yMoveDist = 0;

		if (INPUT->stanceDown->isSwitched) changeStanceDown();
		if (INPUT->stanceUp->isSwitched) changeStanceUp();

		moveSpeed = baseMoveTime / mapPosition.h;

		if (INPUT->moveUp->isDown && INPUT->moveDown->isDown) yMoveDist = 0;
		else if (INPUT->moveUp->isDown && !INPUT->moveDown->isDown) yMoveDist = -1;
		else if (INPUT->moveDown->isDown && !INPUT->moveUp->isDown) yMoveDist = 1;

		if (INPUT->moveLeft->isDown && INPUT->moveRight->isDown) xMoveDist = 0;
		else if (INPUT->moveLeft->isDown && !INPUT->moveRight->isDown) xMoveDist = -1;
		else if (INPUT->moveRight->isDown && !INPUT->moveLeft->isDown) xMoveDist = 1;

		if (xMoveDist != 0 || yMoveDist != 0)
		{
			moveClock.timeBetweenUpdates = moveSpeed;
			moveClock.tickUp();

			for (int i = 1; i <= moveClock.numCalls; moveClock.numCalls--)
			{
				WORLD->updateBlock(mapPosition, false); //clear old position's map properties

				if (WORLD->debugmap->getWalkability(Position4(mapPosition.x + xMoveDist, mapPosition.y, mapPosition.h, mapPosition.z), true))
				{
					mapPosition.x += xMoveDist;
					xMoveDist = 0;
					moved = true;
				} 
				if (WORLD->debugmap->getWalkability(Position4(mapPosition.x, mapPosition.y + yMoveDist, mapPosition.h, mapPosition.z), true))
				{
					mapPosition.y += yMoveDist;
					yMoveDist = 0;
					moved = true;
				}
				WORLD->updateBlock(mapPosition, true); //update new position property

				//play footstep sound
				if (moved)
				{
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
	}
}

void Player::moveSelectorUp()
{
	if (!(itemIndex == -1 && containerIndex == -1)) // if not hands
	{
		if (itemIndex > -1)
		{
			itemIndex--;

			AUDIO->playSound(Sound(("tick"), 0.0f, 5.0f));
		}
		else if (itemIndex == -1)
		{
			if (containerIndex > 0)
			{
				containerIndex--;
				itemIndex = (int)(inventory[containerIndex]->itemList.size() - 1);

				AUDIO->playSound(Sound(("tick"), 0.0f, 5.0f));
			}
		}
		else if (itemIndex == -2)
		{
			if (itemIndex + 3 <= inventory[containerIndex]->itemList.size())
			{
				++itemIndex;

				AUDIO->playSound(Sound(("tick"), 0.0f, 5.0f));
			}
		}
	}
}

void Player::moveSelectorDown()
{
	if (!(itemIndex == -1 && containerIndex == -1)) //if not hands
	{
		if (itemIndex + 1 < inventory[containerIndex]->itemList.size())
		{
			++itemIndex;

			AUDIO->playSound(Sound(("tick"), 0.0f, 10.0f));
		}
		else if (itemIndex + 1 >= inventory[containerIndex]->itemList.size())
		{
			if (containerIndex < inventory.size() - 1)
			{
				++containerIndex;
				itemIndex = -1;

				AUDIO->playSound(Sound(("tick"), 0.0f, 5.0f));
			}
			else if (itemIndex == -1)
			{
				if (itemIndex + 2 <= inventory[containerIndex]->itemList.size())
				{
					++itemIndex;

					AUDIO->playSound(Sound(("tick"), 0.0f, 5.0f));
				}
			}
		}
	}
}

bool Player::pickUpItem()
{
	for (auto& item : WORLD->debugmap->mapItemList)
	{
		if (item->onMap && !item->inAir && item->mapPosition.x == mapPosition.x && item->mapPosition.y == mapPosition.y && item->mapPosition.z == mapPosition.z)
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
		if (container->item->onMap && !container->item->inAir && container->item->mapPosition.x == mapPosition.x && container->item->mapPosition.y == mapPosition.y && container->item->mapPosition.z == mapPosition.z)
		{
			container->pickUp(this);

			GUI->logWindow->pushMessage(Message("Picked up " + container->item->tool->name, Message::MessageLevel::MEDIUM));
			AUDIO->playSound(PositionalTrackedSound(("pick up"), &mapPosition, 60.0f, 30.0f));

			return true;
		}
	}

	return false;
}

void Player::dropItem()
{
	if (containerIndex != -1)
	{
		if (itemIndex >= 0)
		{
			GUI->logWindow->pushMessage(Message("Dropped " + inventory[containerIndex]->itemList[itemIndex]->tool->name, Message::MessageLevel::MEDIUM));
			AUDIO->playSound(PositionalTrackedSound(("drop"), &mapPosition, 65.0f, 30.0f));
			
			selectedItem->drop();
		}
		else if (itemIndex <= -1)
		{
			if (selectedItem->type != Item::ItemType::HAND)
			{
				GUI->logWindow->pushMessage(Message("Dropped " + inventory[containerIndex]->item->tool->name, Message::MessageLevel::LOW));
				AUDIO->playSound(PositionalTrackedSound(("drop"), &mapPosition, 65.0f, 30.0f));
				
				inventory[containerIndex]->drop(this);
			}
		}
	}
}

void Player::throwItem()
{
	//ITEM INDEXES
	//	-2 : no item selected
	//	-1 : container selected
	//	0+ : item in vector

	if (itemIndex == -1) // container selected
	{
		if (selectedItem->type != Item::ItemType::HAND)
		{
			inventory[containerIndex]->throwContainer(this);
		}
	}
	else
	{
		selectedItem->throwItem();
	}
}

void Player::takeDamage(int damage)
{
	if (health != -1) //if player not currently in second chance
	{
		if (health - damage > 0) //take normal damage
		{
			health -= damage;

			AUDIO->playSound(PositionalTrackedSound(("hoop"), &mapPosition, 75.0f, 40.0f));
		}
		else //if damage taken would have resulted in death
		{
			if (hasSecondChance) //will be saved from instant death
			{
				hasSecondChance = false; //should be reset after heal
				health = -1;
			}
			else
			{
				health = 0;
			}
		}
	}
	else
	{
		health = 0;
	}
}

void Player::filterIndexes()
{
	//INDEX FILTERING
	//
	//ITEM INDEXES
	//	-2 : no item selected
	//	-1 : container selected
	//	0+ : item in vector

	if (containerIndex + 1 > (inventory.size())) containerIndex = (int)(inventory.size() - 1);
	else if (containerIndex == -1 && inventory.size() > 0) containerIndex = 0;

	if (containerIndex != -1) //selected an item, not a container
	{
		if (itemIndex != -2) //an item is selected
		{
			if (itemIndex + 1 > inventory[containerIndex]->itemList.size()) //selected item is out of range
			{
				itemIndex = (int)(inventory[containerIndex]->itemList.size() - 1);
			}

			if (itemIndex != -1) //item index is in range
			{
				selectedItem = inventory[containerIndex]->itemList[itemIndex];
			}
			else if (itemIndex == -1) //no items in the container
			{
				selectedItem = inventory[containerIndex]->item;
			}
		}
	}
}

bool Player::reload()
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
						return selectedItem->reload(item->getMagazineData().second);
					}
				}
			}
		}
	}
	return false;
}

void Player::changeFireMode()
{
	selectedItem->changeFireMode();
}

void Player::equipArmor()
{
	selectedItem->equip(equippedArmor);
}

void Player::useMelee()
{
	selectedItem->useMelee();
}

void Player::updateTools()
{
	if (inventory.size() > 0)
	{
		for (int i = 0; i < inventory.size(); i++)
		{
			if (inventory[i]->item->onMap) inventory[i]->item->onMap = false; //should not be called

			if (itemIndex == -1 && containerIndex == i) inventory[i]->item->updateTool(mapPosition, INPUT->mouse->mapPosition.x, INPUT->mouse->mapPosition.y, true); //uses the map position of the mouse		
			else inventory[i]->item->updateTool(mapPosition, INPUT->mouse->mapPosition.x, INPUT->mouse->mapPosition.y, false);

			for (int j = 0; j < inventory[i]->itemList.size(); j++) //stops when i gets to empty container list
			{
				if (inventory[i]->itemList[j]->onMap) inventory[i]->itemList[j]->onMap = false;

				if (itemIndex == j && containerIndex == i) inventory[i]->itemList[j]->updateTool(mapPosition, INPUT->mouse->mapPosition.x, INPUT->mouse->mapPosition.y, true);
				else inventory[i]->itemList[j]->updateTool(mapPosition, INPUT->mouse->mapPosition.x, INPUT->mouse->mapPosition.y, false);
			}
		}
	}
	else selectedItem->updateTool(mapPosition, INPUT->mouse->mapPosition.x, INPUT->mouse->mapPosition.y, true);
}

void Player::update()
{
	renderPosition = getRenderPosition(mapPosition);
	targetPosition = INPUT->mouse->mapPosition;

	if (health != 0) //if player is alive
	{
		color = TCODColor(stance.x, stance.y, stance.z);

		if (INPUT->highlight->isDown) backgroundColor = TCODColor::yellow;
		else backgroundColor = TCODColor::pink;

		//items
		if (INPUT->TCODmouse.wheel_up || INPUT->TCODmouse.wheel_down)
		{
			if (!INPUT->deepInteract->isDown)
			{
				if (INPUT->TCODmouse.wheel_up) moveSelectorUp();
				else if (INPUT->TCODmouse.wheel_down) moveSelectorDown();
			}
			else if (INPUT->deepInteract->isDown)
			{
				if (INPUT->TCODmouse.wheel_up) selectedItem->actionManager->moveSelectorUp();
				else if (INPUT->TCODmouse.wheel_down) selectedItem->actionManager->moveSelectorDown();
			}
		}
		
		if (INPUT->debug1->isSwitched)			throwItem();
		if (INPUT->pickUp->isSwitched)			pickUpItem();
		if (INPUT->drop->isSwitched)			dropItem();
		if (INPUT->reload->isSwitched)			reload();
		if (INPUT->primaryUse->isDown)			selectedItem->use(INPUT->primaryUse->isDown, INPUT->primaryUse->isSwitched);
		if (INPUT->alternateUse->isSwitched)	selectedItem->actionManager->doAction(this);
		
		filterIndexes();
		
		angle = getAngle(renderPosition.x, renderPosition.y, INPUT->mouse->screenPosition.x - 1, INPUT->mouse->screenPosition.y - 3); //change to use mouse map position
		
		updateTools();
		
		//world
		if (INPUT->worldInteract->isSwitched)
		{
			if (WORLD->debugmap->getBlock(mapPosition)->tag == Block::Tag::STAIR) WORLD->debugmap->getBlock(mapPosition)->interact();
		}

		move();
	}
	else //if dead
	{
		ch = '&';
		color = TCODColor::red;
	}
}

void Player::render(const Pane& pane) const
{
	if (backgroundColor != TCODColor::pink) pane.console->setCharBackground(renderPosition.x, renderPosition.y, backgroundColor);

	if (health != 0)
	{
		for (auto& container : inventory)
		{
			container->item->render(pane);

			for (auto& tool : container->itemList) tool->render(pane);
		}
	}

	pane.console->setChar(renderPosition.x, renderPosition.y, ch);
	pane.console->setCharForeground(renderPosition.x, renderPosition.y, color);
}

