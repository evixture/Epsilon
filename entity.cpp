#include "main.hpp"

//Entity Class
Entity::Entity(Position pos, int symbol, std::string name, TCODColor color)
	: mapPosition(pos), renderPosition(pos), symbol(symbol), color(color), name(name), height(3)
{}

void Entity::update()
{
	return;
}

void Entity::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(mapPosition.x, mapPosition.y, symbol);
	pane->console->setCharForeground(mapPosition.x, mapPosition.y, color);
}

//----------------------------------------------------------------------------------------------------

//Creature Class
Creature::Creature(Position pos, int symbol, std::string name, TCODColor color, int health, int armor)
	:Entity(Position(pos), symbol, name, color), angle(0), health(health), armor(armor), containerIndex(0), itemIndex(0), nullMagazine(std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false))
{
}

void Creature::update()
{
	angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx, engine->settings->input->mouse.cy);
}

void Creature::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(mapPosition.x, mapPosition.y, symbol);
	pane->console->setCharForeground(mapPosition.x, mapPosition.y, color);
}

//----------------------------------------------------------------------------------------------------

//Player Class
Player::Player(Position pos)
	:Creature(Position(pos), '@', "player", UICOLOR_Player_Color, 100, 0)
{
	//here
	inventory.push_back(CONTAINER_SmallBackpack(0, 0, 0));
	inventory[0]->addItem(ITEM_M1911(0, 0, 0));
	inventory[0]->addItem(MAGAZINE_45ACPMagazine7(0, 0, 0));
	inventory[0]->addItem(MAGAZINE_45ACPMagazine7(0, 0, 0));
	inventory.push_back(CONTAINER_SmallBackpack(0, 0, 0));

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
	//need to fix indexes

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
	//will pick up multiple items on single press

	for (int i = 0; i < WORLD->mapItemList.size(); i++)
	{
		if (WORLD->mapItemList[i] != nullptr && WORLD->mapItemList[i]->mapPosition.x == mapPosition.x && WORLD->mapItemList[i]->mapPosition.y == mapPosition.y && WORLD->mapItemList[i]->mapPosition.level == mapPosition.level)
		{
			if (containerIndex != -1)
			{
				if (inventory[containerIndex]->addItem(WORLD->mapItemList[i]))
				{
					GUI->eventLogPane->pushMessage("Picked up " + WORLD->mapItemList[i]->tool->name);

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
			GUI->eventLogPane->pushMessage("Picked up " + WORLD->mapItemList[i]->tool->name);

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
			GUI->eventLogPane->pushMessage("Dropped " + inventory[containerIndex]->itemList[itemIndex]->tool->name);

			WORLD->mapItemList.push_back(selectedItem);

			inventory[containerIndex]->itemList.erase(inventory[containerIndex]->itemList.begin() + itemIndex);
		}
		else if (itemIndex <= -1)
		{
			GUI->eventLogPane->pushMessage("Dropped " + inventory[containerIndex]->containerItem->tool->name);
			
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
			selectedItem = ITEM_Hands(0, 0, 0);
		}
	}

	else
	{
		selectedItem = ITEM_Hands(0, 0, 0);
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
						//need to be able to reload mag with less ammo if mags with more ammo do not exist
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

void Player::update()
{
	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	//MOUSE WHEEL ITEM SELECTION
	if (INPUT->mouse.wheel_up || INPUT->mouse.wheel_down)
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

	if (INPUT->e->isSwitched)
	{
		pickUpItem();
	}

	if (INPUT->q->isSwitched)
	{
		dropItem();
	}

	filterIndexes();

	angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3);

	if (INPUT->r->isSwitched)
	{
		reload();
	}
	selectedItem->updateTool(renderPosition.x, renderPosition.y, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3, angle, mapPosition.level);
	
	if (INPUT->space->isSwitched)
	{
		if (WORLD->getTile(mapPosition.x, mapPosition.y, mapPosition.level)->tag == Tile::Tag::STAIR)
		{
			WORLD->getTile(mapPosition.x, mapPosition.y, mapPosition.level)->interact();
		}
	}
}

//----------------------------------------------------------------------------------------------------

void Player::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(renderPosition.x, renderPosition.y, symbol);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);

	selectedItem->renderTool(pane);
}
