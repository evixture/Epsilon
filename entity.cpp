#include "main.hpp"

Entity::Entity(Position pos, int ch, std::string name, TCODColor color)
	: mapPosition(pos), renderPosition(pos), ch(ch), color(color), name(name), height(3)
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

Creature::Creature(Position pos, int ch, std::string name, TCODColor color, int health, int armor)
	:Entity(Position(pos), ch, name, color), health(health), armor(armor), angle(0), containerIndex(0), itemIndex(0), nullMagazine(std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false))
{
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

Player::Player(Position pos)
	:Creature(Position(pos), '@', "player", UICOLOR_Player_Color, 100, 0), baseMoveTime(0.0f), moveXSpeed(0), moveYSpeed(0), movementClock(Clock(1.0f, 0.0f))
{
	//inventory.push_back(	CONTAINER_SmallBackpack(0, 0, 0, this));
	//inventory[0]->addItem(	ITEM_SIP45(0, 0, 0, this));
	//inventory[0]->addItem(	MAGAZINE_45Magazine7(0, 0, 0, this));
	//inventory[0]->addItem(	MAGAZINE_45Magazine7(0, 0, 0, this));
	//inventory.push_back(	CONTAINER_SmallBackpack(0, 0, 0, this));

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
		if (INPUT->lshift->isDown) baseMoveTime = .25f;
		else if (INPUT->lctrl->isDown)	baseMoveTime = 1.0f;
		else baseMoveTime = .5f;

		moveXSpeed = 0;
		moveYSpeed = 0;

		if (INPUT->z->isSwitched)
		{
			int tempHeight = height;
			for (int i = 0; i < int(abs(tempHeight - 1)); i++)
			{
				if (tempHeight - 1 > 0)
				{
					changeStanceDown();
				}
				else if (tempHeight - 1 < 0)
				{
					changeStanceUp();
				}
			}
		}
		if (INPUT->x->isSwitched)
		{
			int tempHeight = height;
			for (int i = 0; i < int(abs(tempHeight - 2)); i++)
			{
				if (tempHeight - 2 > 0)
				{
					changeStanceDown();
				}
				else if (tempHeight - 2 < 0)
				{
					changeStanceUp();
				}
			}
		}
		if (INPUT->c->isSwitched)
		{
			int tempHeight = height;
			for (int i = 0; i < int(abs(tempHeight - 3)); i++)
			{
				if (tempHeight - 3 > 0)
				{
					changeStanceDown();
				}
				else if (tempHeight - 3 < 0)
				{
					changeStanceUp();
				}
			}
		}
		//(int)((baseMoveTime / height) * SETTINGS->fpsCount)
		movementClock.capacity = float(baseMoveTime / height);

		if (INPUT->w->isDown && INPUT->s->isDown) moveYSpeed = 0;
		else if (INPUT->w->isDown && !INPUT->s->isDown) moveYSpeed = -1;
		else if (INPUT->s->isDown && !INPUT->w->isDown) moveYSpeed = 1;

		if (INPUT->a->isDown && INPUT->d->isDown) moveXSpeed = 0;
		else if (INPUT->a->isDown && !INPUT->d->isDown) moveXSpeed = -1;
		else if (INPUT->d->isDown && !INPUT->a->isDown) moveXSpeed = 1;

		if (moveXSpeed != 0 || moveYSpeed != 0)
		{
			if (movementClock.isAtZero())
			{
				if (GUI->worldWindow->world->getWalkability(Position(mapPosition.x + moveXSpeed, mapPosition.y, mapPosition.level), height))
				{
					mapPosition.x += moveXSpeed;
					moveXSpeed = 0;
				}
				if (GUI->worldWindow->world->getWalkability(Position(mapPosition.x, mapPosition.y + moveYSpeed, mapPosition.level), height))
				{
					mapPosition.y += moveYSpeed;
					moveYSpeed = 0;
				}
				movementClock.update(false, true, false);
			}
		}
			movementClock.update(true, false, false);
	}
}

void Player::changeStanceUp()
{
	if (WORLD->getWalkability(mapPosition, height + 1)) height += 1;
}

void Player::changeStanceDown()
{
	if (WORLD->getWalkability(mapPosition, height + 1)) height -= 1;
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
			itemIndex++;
		}
	}
}

void Player::moveSelectorDown()
{
	if (itemIndex + 1 < inventory[containerIndex]->itemList.size())
	{
		itemIndex++;
	}
	else if (itemIndex + 1 >= inventory[containerIndex]->itemList.size())
	{
		if (containerIndex < inventory.size() - 1)
		{
			containerIndex++;
			itemIndex = -1;
		}
		else if (itemIndex == -1)
		{
			if (itemIndex + 2 <= inventory[containerIndex]->itemList.size())
			{
				itemIndex++;
			}
		}
	}
}

void Player::pickUpItem()
{
	for (int i = 0; i < WORLD->mapItemList.size(); i++)
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

	for (int i = 0; i < WORLD->mapContainerList.size(); i++)
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
			//selectedItem = ITEM_Hands(0, 0, 0, this);
		}
	}
	else
	{
		//selectedItem = ITEM_Hands(0, 0, 0, this);
	}
}

void Player::reload()
{
	for (int i = 0; i < inventory.size(); i++)
	{
		for (int j = 0; j < inventory[i]->itemList.size(); j++)
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

void Player::update()
{
	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	if (health > 0) //if player is alive
	{
		move();
		
		if (INPUT->mouse.wheel_up || INPUT->mouse.wheel_down)
		{
			if (!INPUT->lalt->isDown)
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
			else if (INPUT->lalt->isDown)
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
		
		if (INPUT->e->isSwitched)
		{
			pickUpItem();
		}
		
		if (INPUT->q->isSwitched)
		{
			dropItem();
		}
		
		if (INPUT->rightMouseButton->isSwitched)
		{
			selectedItem->actionManager->doAction();
		}
		
		filterIndexes();
		
		angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3);
		
		selectedItem->updateTool(mapPosition, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3, angle);
		
		if (INPUT->r->isSwitched)
		{
			reload();
		}
		
		if (INPUT->space->isSwitched)
		{
			if (WORLD->getTile(mapPosition)->tag == Tile::Tag::STAIR)
			{
				WORLD->getTile(mapPosition)->interact();
			}
		}
	}

	if (INPUT->num0->isSwitched)
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
