#include "main.hpp"

Entity::Entity(Position4 pos, int ch, std::string name, TCODColor color)
	:mapPosition(pos), renderPosition(pos), ch(ch), color(color), name(name)
{}

void Entity::update()
{
	return;
}

void Entity::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(mapPosition.x, mapPosition.y, ch);
	pane->console->setCharForeground(mapPosition.x, mapPosition.y, color);
}

//----------------------------------------------------------------------------------------------------

Creature::Creature(Position4 position, int ch, std::string name, TCODColor color, int health, Armor armor)
	:Entity(position, ch, name, color), health(health), equippedArmor(armor), angle(0), containerIndex(0), itemIndex(0), nullMagazine(std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false))
{
}

void Creature::takeDamage(Bullet* bullet)
{
	if (equippedArmor.durability > 0) //if the armor durability is high enough
	{
		if (bullet->currentVelocity - equippedArmor.defense > 0) //if bullet is fast enough to pass through armor
		{
			equippedArmor.durability -= bullet->currentVelocity; //should happen before taking damage to prevent high damage
			bullet->currentVelocity -= equippedArmor.defense;

			health -= int(float(bullet->currentVelocity / (bullet->baseVelocity * 2.0f)) * bullet->mass);

			bullet->currentVelocity -= 100; //slowdown after going through body
		}
		else if (bullet->currentVelocity - equippedArmor.defense <= 0) //if the bullet is stopped by the armor
		{
			equippedArmor.durability -= bullet->currentVelocity;
			bullet->currentVelocity = 0;
		}

		if (equippedArmor.durability < 0)
		{
			equippedArmor.durability = 0;
		}
	}
	else
	{
		int damage = int(float(bullet->currentVelocity / (bullet->baseVelocity * 2.0f)) * bullet->mass);
		health -= damage;

		bullet->currentVelocity -= 100; //slowdown after going through body
	}

	if (health < 0)
	{
		health = 0;
	}
}

void Creature::update()
{
	angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx, engine->settings->input->mouse.cy);

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	if (!(health > 0)) //if not "alive"
	{
		health = 0; //prevent from taking further damage
		ch = '$'; //set char to dead symbol
		color = TCODColor::red;
	}
}

void Creature::render(const std::shared_ptr<Pane>& pane) const
{
	if (WORLD->player->mapPosition.level == mapPosition.level)
	{
		pane->console->setChar(renderPosition.x, renderPosition.y, ch);
		pane->console->setCharForeground(renderPosition.x, renderPosition.y, (WORLD->isInFov(mapPosition))? color : TCODColor::darkerGrey); //out of fov creatures rendered with one step above normal fov grey to be more noticible
	}
}

//----------------------------------------------------------------------------------------------------

Player::Player(Position4 position)
	:Creature(position, '@', "player", UICOLOR_Player_Color, 100, Armor("", TCODColor::pink, 0, 0)), baseMoveTime(0.0f), moveXSpeed(0), moveYSpeed(0), moveClock(0), moveSpeed(0)
{
	inventory.push_back(	CONTAINER_SmallBackpack(0, 0, 0, this));
	inventory[0]->addItem(	ITEM_SIP45(0, 0, 0, this));
	inventory[0]->addItem(	MAGAZINE_45Magazine7(0, 0, 0, this));
	inventory[0]->addItem(	MAGAZINE_45Magazine7(0, 0, 0, this));
	inventory.push_back(	CONTAINER_SmallBackpack(0, 0, 0, this));

	if (inventory.size() > 0)
	{
		if (inventory[0]->itemList.size() > 0)
		{
			selectedItem = inventory[containerIndex]->itemList[itemIndex];
		}
	}
	else
	{
		selectedItem = nullptr;
	}

	selectedMagazine = std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false);
}

void Player::move()
{
	if (GUI->activeWindow != Gui::ActiveWindow::STARTUPSPLASH)
	{
		if (INPUT->moveFastKey->isDown) baseMoveTime = .25f;
		else if (INPUT->moveSlowKey->isDown)	baseMoveTime = 1.0f;
		else baseMoveTime = .5f;

		moveXSpeed = 0;
		moveYSpeed = 0;

		if (INPUT->proneKey->isSwitched)
			{
				int testHeight = mapPosition.height;
				for (int i = 0; i < int(abs(testHeight - 1)); ++i)
				{
					if (testHeight - 1 > 0)
					{
						changeStanceDown();
					}
					else if (testHeight - 1 < 0)
					{
						changeStanceUp();
					}
				}
			}
		if (INPUT->crouchKey->isSwitched)
			{
				int testHeight = mapPosition.height;
				for (int i = 0; i < int(abs(testHeight - 2)); ++i)
				{
					if (testHeight - 2 > 0)
					{
						changeStanceDown();
					}
					else if (testHeight - 2 < 0)
					{
						changeStanceUp();
					}
				}
			}
		if (INPUT->standKey->isSwitched)
			{
				int testHeight = mapPosition.height;
				for (int i = 0; i < int(abs(testHeight - 3)); ++i)
				{
					if (testHeight - 3 > 0)
					{
						changeStanceDown();
					}
					else if (testHeight - 3 < 0)
					{
						changeStanceUp();
					}
				}
			}

		moveSpeed = baseMoveTime / mapPosition.height;

		if (INPUT->moveUpKey->isDown && INPUT->moveDownKey->isDown) moveYSpeed = 0;
		else if (INPUT->moveUpKey->isDown && !INPUT->moveDownKey->isDown) moveYSpeed = -1;
		else if (INPUT->moveDownKey->isDown && !INPUT->moveUpKey->isDown) moveYSpeed = 1;

		if (INPUT->moveLeftKey->isDown && INPUT->moveRightKey->isDown) moveXSpeed = 0;
		else if (INPUT->moveLeftKey->isDown && !INPUT->moveRightKey->isDown) moveXSpeed = -1;
		else if (INPUT->moveRightKey->isDown && !INPUT->moveLeftKey->isDown) moveXSpeed = 1;

		if (moveXSpeed != 0 || moveYSpeed != 0)
		{
			moveClock.timeBetweenUpdates = moveSpeed;
			moveClock.tickUp();

			for (int i = 1; i <= moveClock.numCalls; moveClock.numCalls--)
			{
				if (WORLD->getWalkability(Position4(mapPosition.x + moveXSpeed, mapPosition.y, mapPosition.height, mapPosition.level)))
				{
					mapPosition.x += moveXSpeed;
					moveXSpeed = 0;
				}
				if (WORLD->getWalkability(Position4(mapPosition.x, mapPosition.y + moveYSpeed, mapPosition.height, mapPosition.level)))
				{
					mapPosition.y += moveYSpeed;
					moveYSpeed = 0;
				}
			}
		}
	}
}

void Player::changeStanceUp()
{
	if (WORLD->getWalkability(Position4(mapPosition.x, mapPosition.y, mapPosition.height + 1, mapPosition.level))) mapPosition.height += 1;
}

void Player::changeStanceDown()
{
	if (WORLD->getWalkability(Position4(mapPosition.x, mapPosition.y, mapPosition.height - 1, mapPosition.level))) mapPosition.height -= 1;
}

void Player::moveSelectorUp()
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

void Player::moveSelectorDown()
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

void Player::pickUpItem()
{
	for (int i = 0; i < WORLD->mapItemList.size(); ++i)
	{
		if (WORLD->mapItemList[i] != nullptr && WORLD->mapItemList[i]->mapPosition.x == mapPosition.x && WORLD->mapItemList[i]->mapPosition.y == mapPosition.y && WORLD->mapItemList[i]->mapPosition.level == mapPosition.level)
		{
			if (containerIndex != -1)
			{
				if (inventory[containerIndex]->addItem(WORLD->mapItemList[i]))
				{
					GUI->logWindow->pushMessage(LogWindow::Message("Picked up " + WORLD->mapItemList[i]->tool->name, LogWindow::Message::MessageLevel::MEDIUM));

					WORLD->mapItemList.erase(WORLD->mapItemList.begin() + i);
					return;
				}
			}
		}
	}

	for (int i = 0; i < WORLD->mapContainerList.size(); ++i)
	{
		if (WORLD->mapContainerList[i] != nullptr && WORLD->mapContainerList[i]->containerItem->mapPosition.x == mapPosition.x && WORLD->mapContainerList[i]->containerItem->mapPosition.y == mapPosition.y && WORLD->mapContainerList[i]->containerItem->mapPosition.level == mapPosition.level)
		{
			GUI->logWindow->pushMessage(LogWindow::Message("Picked up " + WORLD->mapContainerList[i]->containerItem->tool->name, LogWindow::Message::MessageLevel::MEDIUM));

			inventory.push_back(WORLD->mapContainerList[i]);
			WORLD->mapContainerList.erase(WORLD->mapContainerList.begin() + i);
			return;
		}
	}
}

void Player::dropItem()
{
	if (containerIndex != -1)
	{
		if (itemIndex >= 0)
		{
			if (inventory[containerIndex]->itemList[itemIndex]->getMagazineData()->isValid)
			{
				for (auto& container : inventory)
				{
					for (auto& item : container->itemList)
					{
						if (item->tool->getMagData() == inventory[containerIndex]->itemList[itemIndex]->getMagazineData())
						{
							item->tool->reload(nullMagazine);
						}
					}
				}
			}
			GUI->logWindow->pushMessage(LogWindow::Message("Dropped " + inventory[containerIndex]->itemList[itemIndex]->tool->name, LogWindow::Message::MessageLevel::MEDIUM));

			WORLD->mapItemList.push_back(selectedItem);

			inventory[containerIndex]->itemList.erase(inventory[containerIndex]->itemList.begin() + itemIndex);
		}
		else if (itemIndex <= -1)
		{
			GUI->logWindow->pushMessage(LogWindow::Message("Dropped " + inventory[containerIndex]->containerItem->tool->name, LogWindow::Message::MessageLevel::LOW));
			
			WORLD->mapContainerList.push_back(inventory[containerIndex]);

			inventory.erase(inventory.begin() + containerIndex);
		}
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
				selectedItem = inventory[containerIndex]->containerItem;
			}
		}
		else
		{
			selectedItem = ITEM_Hands(0, 0, 0, this);
		}
	}
	else
	{
		selectedItem = ITEM_Hands(0, 0, 0, this);
	}
}

void Player::reload()
{
	for (int i = 0; i < inventory.size(); ++i)
	{
		for (int j = 0; j < inventory[i]->itemList.size(); ++j)
		{
			if (inventory[i]->itemList[j]->getMagazineData()->isValid == true) // if it is actually a magazine
			{
				if (inventory[i]->itemList[j]->getMagazineData()->ammoType == selectedItem->tool->ammoType) // if it has the same type of ammo as the current weapon
				{
					if (inventory[i]->itemList[j]->getMagazineData()->availableAmmo != 0) // if the magazine is not empty
					{
						if (inventory[i]->itemList[j]->getMagazineData()->availableAmmo > selectedMagazine->availableAmmo)
						{
							selectedMagazine = inventory[i]->itemList[j]->getMagazineData();
							selectedItem->tool->reload(selectedMagazine);
							return;
						}
						else
						{
							selectedMagazine = inventory[i]->itemList[j]->getMagazineData();
							selectedItem->tool->reload(selectedMagazine);
						}
					}
				}
			}
			else if (selectedMagazine->isValid == false)
			{
				selectedMagazine = std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false);
			}
		}
	}
}

void Player::changeFireMode()
{
	selectedItem->tool->changeFireMode();
}

void Player::equipArmor()
{
	selectedItem->tool->equip(equippedArmor);
}

void Player::update()
{
	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	if (health > 0) //if player is alive
	{
		move();
		
		if (INPUT->mouse.wheel_up || INPUT->mouse.wheel_down)
		{
			if (!INPUT->deepInteractKey->isDown)
			{
				if (INPUT->mouse.wheel_up)
				{
					moveSelectorUp();
				}
				else if (INPUT->mouse.wheel_down)
				{
					moveSelectorDown();
				}
			}
			else if (INPUT->deepInteractKey->isDown)
			{
				if (INPUT->mouse.wheel_up)
				{
					selectedItem->actionManager->moveSelectorUp();
				}
				else if (INPUT->mouse.wheel_down)
				{
					selectedItem->actionManager->moveSelectorDown();
				}
			}
		}
		
		if (INPUT->pickUpKey->isSwitched)
		{
			pickUpItem();
		}
		
		if (INPUT->dropKey->isSwitched)
		{
			dropItem();
		}
		
		if (INPUT->alternateUseButton->isSwitched)
		{
			selectedItem->actionManager->doAction();
		}
		
		filterIndexes();
		
		angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3);
		
		/*
		for containers in inventory
			if held item is the container item
			//

			else
				for items in container inventory
					if i and j is same as indexes


					if (containerIndex != -1) //if a container is selected
					{
						if (containerIndex == i && itemIndex == -1) //if the container item is selected
						{
							//update selected container item
							GUI->logWindow->pushMessage(LogWindow::Message("update selected container item", LogWindow::Message::MessageLevel::LOW));
						}
						else if (i == containerIndex && j == itemIndex)
						{
							//update selected item
							GUI->logWindow->pushMessage(LogWindow::Message("update selected item", LogWindow::Message::MessageLevel::LOW));
						}

						//update other items
					}
		*/

		for (int i = 0; i < inventory.size(); i++)
		{
			if (itemIndex == -1 && containerIndex == i) //if container is the held item
			{
				//spec update the held container
				//GUI->logWindow->pushMessage(LogWindow::Message("update selected container item", LogWindow::Message::MessageLevel::LOW));
				inventory[i]->containerItem->updateTool(mapPosition, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3, angle, true);
			}
			else
			{
				//normal update the container
				inventory[i]->containerItem->updateTool(mapPosition, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3, angle, false);
			}
			
			for (int j = 0; j < inventory[i]->itemList.size(); j++) //stops when i gets to empty container list
			{
				if (itemIndex != -1)
				{
					if (itemIndex == j && containerIndex == i)
					{
						inventory[i]->itemList[j]->updateTool(mapPosition, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3, angle, true);
						//spec update held item
						//GUI->logWindow->pushMessage(LogWindow::Message("update selected item", LogWindow::Message::MessageLevel::LOW));
					}
					else
					{
						//normal update the item
						inventory[i]->itemList[j]->updateTool(mapPosition, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3, angle, false);
					}
				}
			}
			
		}

		//selectedItem->updateTool(mapPosition, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3, angle);
		
		if (INPUT->reloadKey->isSwitched)
		{
			reload();
		}
		
		if (INPUT->worldInteractKey->isSwitched)
		{
			if (WORLD->getTile(mapPosition)->tag == Block::Tag::STAIR)
			{
				WORLD->getTile(mapPosition)->interact();
			}
		}
	}

	if (INPUT->debug2Key->isSwitched)
	{
		health -= 25;
	}

	if (!(health > 0)) //if not "alive"
	{
		health = 0; //prevent from taking further damage
		ch = '&'; //set char to dead symbol
		color = TCODColor::red;
	}
}

void Player::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(renderPosition.x, renderPosition.y, ch);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);

	selectedItem->renderTool(pane); //want to fix, selected item is still rendered when dead
}
