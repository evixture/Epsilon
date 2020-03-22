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
	:Entity(position, ch, name, color), health(health), equippedArmor(armor), angle(0), containerIndex(0), itemIndex(0),
	nullMagazine(std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false)), moveClock(0), moveSpeed(0), baseMoveTime(0.0f)
{
	inventory.push_back(std::make_shared<Container>(ep::container::hands(0, 0, 0, this)));
	selectedItem = inventory[0]->containerItem;

	selectedMagazine = std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false);
}

void Creature::move()
{
}

void Creature::changeStanceUp()
{
	if (mapPosition.height + 1 <= 3)
	{
		if (WORLD->getWalkability(Position4(mapPosition.x, mapPosition.y, mapPosition.height + 1, mapPosition.floor), false)) mapPosition.height += 1;
	}
}

void Creature::changeStanceDown()
{
	if (mapPosition.height - 1 >= 1)
	{
		if (WORLD->getWalkability(Position4(mapPosition.x, mapPosition.y, mapPosition.height - 1, mapPosition.floor), false)) mapPosition.height -= 1;
	}
}

void Creature::pickUpItem()
{
}

void Creature::dropItem()
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
	

void Creature::reload()
{
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
	angle = getAngle(renderPosition.x, renderPosition.y, INPUT->mouse.cx, INPUT->mouse.cy);

	renderPosition = Position3(offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset));

	if (health == 0) //if dead
	{
		ch = '$'; //set char to dead symbol
		color = TCODColor::red;
	}
}

void Creature::render(const std::shared_ptr<Pane>& pane) const
{
	if (WORLD->debugmap->player->mapPosition.floor == mapPosition.floor)
	{
		pane->console->setChar(renderPosition.x, renderPosition.y, ch);
		pane->console->setCharForeground(renderPosition.x, renderPosition.y, (WORLD->isInPlayerFov(mapPosition))? color : TCODColor::darkerGrey); //out of fov creatures rendered with one step above normal fov grey to be more noticible
	}
}

//----------------------------------------------------------------------------------------------------

Player::Player(Position4 position)
	:Creature(position, '@', "player", ep::color::player, 100, Armor("", TCODColor::pink, 0, 0)), xMoveDist(0), yMoveDist(0), backgroundColor(TCODColor::pink), hasSecondChance(true)
{
	hasSecondChance = true;

	inventory.push_back(	std::make_shared<Container>(ep::container::smallBackpack(0, 0, 0, this)));
	inventory[1]->addItem(	std::make_shared<Item>(ep::item::sip45(0, 0, 0, this)));
	inventory[1]->addItem(	std::make_shared<MagazineItem>(ep::magazine::cal45Magazine7(0, 0, 0, this)));
	inventory[1]->addItem(	std::make_shared<MagazineItem>(ep::magazine::cal45Magazine7(0, 0, 0, this)));
	inventory.push_back(	std::make_shared<Container>(ep::container::smallBackpack(0, 0, 0, this)));

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
}

void Player::move()
{
	if (GUI->activeWindow != Gui::ActiveWindow::STARTUPSPLASH)
	{
		if (INPUT->moveFastKey->isDown)			baseMoveTime = .25f;
		else if (INPUT->moveSlowKey->isDown)	baseMoveTime = 1.0f;
		else									baseMoveTime = .5f;

		xMoveDist = 0;
		yMoveDist = 0;

		if (INPUT->stanceDownKey->isSwitched)
		{
			changeStanceDown();
		}
		if (INPUT->stanceUpKey->isSwitched)
		{
			changeStanceUp();
		}

		moveSpeed = baseMoveTime / mapPosition.height;

		if (INPUT->moveUpKey->isDown && INPUT->moveDownKey->isDown) yMoveDist = 0;
		else if (INPUT->moveUpKey->isDown && !INPUT->moveDownKey->isDown) yMoveDist = -1;
		else if (INPUT->moveDownKey->isDown && !INPUT->moveUpKey->isDown) yMoveDist = 1;

		if (INPUT->moveLeftKey->isDown && INPUT->moveRightKey->isDown) xMoveDist = 0;
		else if (INPUT->moveLeftKey->isDown && !INPUT->moveRightKey->isDown) xMoveDist = -1;
		else if (INPUT->moveRightKey->isDown && !INPUT->moveLeftKey->isDown) xMoveDist = 1;

		if (xMoveDist != 0 || yMoveDist != 0)
		{
			moveClock.timeBetweenUpdates = moveSpeed;
			moveClock.tickUp();

			for (int i = 1; i <= moveClock.numCalls; moveClock.numCalls--)
			{
				WORLD->updateBlock(mapPosition, false); //clear old position's map properties

				if (WORLD->getWalkability(Position4(mapPosition.x + xMoveDist, mapPosition.y, mapPosition.height, mapPosition.floor), true))
				{
					mapPosition.x += xMoveDist;
					xMoveDist = 0;
				} 
				if (WORLD->getWalkability(Position4(mapPosition.x, mapPosition.y + yMoveDist, mapPosition.height, mapPosition.floor), true))
				{
					mapPosition.y += yMoveDist;
					yMoveDist = 0;
				}

				WORLD->updateBlock(mapPosition, true); //update new position property
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

void Player::moveSelectorDown()
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

void Player::pickUpItem()
{
	for (int i = 0; i < WORLD->debugmap->mapItemList.size(); ++i)
	{
		if (WORLD->debugmap->mapItemList[i] != nullptr && WORLD->debugmap->mapItemList[i]->mapPosition.x == mapPosition.x && WORLD->debugmap->mapItemList[i]->mapPosition.y == mapPosition.y && WORLD->debugmap->mapItemList[i]->mapPosition.floor == mapPosition.floor)
		{
			if (containerIndex != -1)
			{
				if (inventory[containerIndex]->addItem(WORLD->debugmap->mapItemList[i]))
				{
					GUI->logWindow->pushMessage(LogWindow::Message("Picked up " + WORLD->debugmap->mapItemList[i]->tool->name, LogWindow::Message::MessageLevel::MEDIUM));

					WORLD->debugmap->mapItemList.erase(WORLD->debugmap->mapItemList.begin() + i);
					return;
				}
			}
		}
	}

	for (int i = 0; i < WORLD->debugmap->mapContainerList.size(); ++i)
	{
		if (WORLD->debugmap->mapContainerList[i] != nullptr && WORLD->debugmap->mapContainerList[i]->containerItem->mapPosition.x == mapPosition.x && WORLD->debugmap->mapContainerList[i]->containerItem->mapPosition.y == mapPosition.y && WORLD->debugmap->mapContainerList[i]->containerItem->mapPosition.floor == mapPosition.floor)
		{
			GUI->logWindow->pushMessage(LogWindow::Message("Picked up " + WORLD->debugmap->mapContainerList[i]->containerItem->tool->name, LogWindow::Message::MessageLevel::MEDIUM));

			inventory.push_back(WORLD->debugmap->mapContainerList[i]);
			WORLD->debugmap->mapContainerList.erase(WORLD->debugmap->mapContainerList.begin() + i);
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

			WORLD->debugmap->mapItemList.push_back(selectedItem);

			inventory[containerIndex]->itemList.erase(inventory[containerIndex]->itemList.begin() + itemIndex);
		}
		else if (itemIndex <= -1)
		{
			if (selectedItem->type != Item::ItemType::HAND)
			{
				GUI->logWindow->pushMessage(LogWindow::Message("Dropped " + inventory[containerIndex]->containerItem->tool->name, LogWindow::Message::MessageLevel::LOW));
			
				WORLD->debugmap->mapContainerList.push_back(inventory[containerIndex]);

				inventory.erase(inventory.begin() + containerIndex);
			}
		}
	}
}

void Player::takeDamage(int damage)
{
	if (health != -1) //if player not currently in second chance
	{
		if (health - damage > 0) //take normal damage
		{
			health -= damage;
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
	}
}

void Player::reload()
{
	for (auto& container : inventory)
	{
		for (auto& item : container->itemList)
		{
			if (item->getMagazineData()->isValid == true) // if it is actually a magazine
			{
				if (item->getMagazineData()->ammoType == selectedItem->tool->ammoType) // if it has the same type of ammo as the current weapon
				{
					if (item->getMagazineData()->availableAmmo != 0) // if the magazine is not empty
					{
						if (item->getMagazineData()->availableAmmo > selectedMagazine->availableAmmo)
						{
							selectedMagazine = item->getMagazineData();
							selectedItem->tool->reload(selectedMagazine);
							return;
						}
						else
						{
							selectedMagazine = item->getMagazineData();
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

void Player::useMelee()
{
	selectedItem->tool->useMelee();
}

void Player::updateTools()
{
	if (inventory.size() > 0)
	{
		for (int i = 0; i < inventory.size(); i++)
		{
			if (itemIndex == -1 && containerIndex == i) //if container is the held item
			{
				//special update the held container
				inventory[i]->containerItem->updateTool(mapPosition, INPUT->mouse.cx - 1 + WORLD->xOffset, INPUT->mouse.cy - 3 + WORLD->yOffset, true); //uses the map position of the mouse
			}
			else
			{
				//normal update the container
				inventory[i]->containerItem->updateTool(mapPosition, INPUT->mouse.cx - 1 + WORLD->xOffset, INPUT->mouse.cy - 3 + WORLD->yOffset, false);
			}

			for (int j = 0; j < inventory[i]->itemList.size(); j++) //stops when i gets to empty container list
			{
				if (itemIndex == j && containerIndex == i)
				{
					//special update held item
					inventory[i]->itemList[j]->updateTool(mapPosition, INPUT->mouse.cx - 1 + WORLD->xOffset, INPUT->mouse.cy - 3 + WORLD->yOffset, true);
				}
				else
				{
					//normal update the item
					inventory[i]->itemList[j]->updateTool(mapPosition, INPUT->mouse.cx - 1 + WORLD->xOffset, INPUT->mouse.cy - 3 + WORLD->yOffset, false);
				}
			}
		}
	}
	else
	{
		selectedItem->updateTool(mapPosition, INPUT->mouse.cx - 1 + WORLD->xOffset, INPUT->mouse.cy - 3, true);
	}
}

void Player::update()
{
	renderPosition = Position3(offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset));

	if (health != 0) //if player is alive
	{
		if (INPUT->highlightKey->isDown)
		{
			backgroundColor = TCODColor::yellow;
		}
		else backgroundColor = TCODColor::pink;

		//items
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
		
		if (INPUT->reloadKey->isSwitched)
		{
			reload();
		}

		if (INPUT->primaryUseButton->isDown)
		{
			selectedItem->tool->use(INPUT->primaryUseButton->isDown, INPUT->primaryUseButton->isSwitched);
		}

		if (INPUT->alternateUseButton->isSwitched)
		{
			selectedItem->actionManager->doAction(this);
		}
		
		filterIndexes();
		
		angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3);
		
		updateTools();
		
		//world
		if (INPUT->worldInteractKey->isSwitched)
		{
			if (WORLD->debugmap->getBlock(mapPosition)->tag == Block::Tag::STAIR)
			{
				WORLD->debugmap->getBlock(mapPosition)->interact();
			}
		}

		move();
	}
	else //if dead
	{
		ch = '&';
		color = TCODColor::red;
	}
}

void Player::render(const std::shared_ptr<Pane>& pane) const
{
	if (backgroundColor != TCODColor::pink)
	{
		pane->console->setCharBackground(renderPosition.x, renderPosition.y, backgroundColor);
	}

	if (health != 0)
	{
		selectedItem->renderTool(pane);
	}

	pane->console->setChar(renderPosition.x, renderPosition.y, ch);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);
}

