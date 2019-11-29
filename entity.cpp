#include "main.hpp"

//Entity Class
Entity::Entity(Position pos, int symbol, const char* name, TCODColor color)
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
Creature::Creature(Position pos, int symbol, const char* name, TCODColor color, int health, int armor)
	:Entity(Position(pos), symbol, name, color), angle(0), health(health), armor(armor), containerIndex(0), itemIndex(0)
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
	:Creature(Position(pos), '@', "player", COLOR_Player_Color, 100, 0)
{
	inventory.push_back(CONTAINER_Default_Container(0, 0, 0, 5));
	inventory[0]->addItem(ITEM_Test_Size2(0, 0, 0));

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

//----------------------------------------------------------------------------------------------------

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
	if (itemIndex < inventory[containerIndex]->itemList.size() - 1)
	{
		itemIndex++;
	}

	else if (itemIndex >= inventory[containerIndex]->itemList.size() - 1)
	{
		if (containerIndex < inventory.size() - 1)
		{
			containerIndex++;
			itemIndex = 0;
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
	for (auto& item : WORLD->mapItemList)
	{
		if (item != nullptr && item->mapPosition.x == mapPosition.x && item->mapPosition.y == mapPosition.y && item->mapPosition.level == mapPosition.level)
		{
			std::vector<std::shared_ptr<Item>>::iterator itr = std::find(WORLD->mapItemList.begin(), WORLD->mapItemList.end(), item);

			if (itr != WORLD->mapItemList.cend())
			{
				__int64 index = std::distance(WORLD->mapItemList.begin(), itr);

				if (containerIndex != -1)
				{
					inventory[containerIndex]->addItem(item);
					WORLD->mapItemList.erase(WORLD->mapItemList.begin() + index);
				}
			}
		}
	}
	for (auto& container : WORLD->mapContainerList)
	{
		if (container != nullptr && container->containerItem->mapPosition.x == mapPosition.x && container->containerItem->mapPosition.y == mapPosition.y && container->containerItem->mapPosition.level == mapPosition.level)
		{
			inventory.push_back(container);

			std::vector<std::shared_ptr<Container>>::iterator itr = std::find(WORLD->mapContainerList.begin(), WORLD->mapContainerList.end(), container);

			if (itr != WORLD->mapContainerList.cend())
			{
				__int64 index = std::distance(WORLD->mapContainerList.begin(), itr);
				WORLD->mapContainerList.erase(WORLD->mapContainerList.begin() + index);
			}
		}
	}
}

void Player::dropItem()
{
	if (containerIndex != -1)
	{
		if (itemIndex >= 0)
		{
			WORLD->mapItemList.push_back(selectedItem);

			inventory[containerIndex]->itemList.erase(inventory[containerIndex]->itemList.begin() + itemIndex);
		}
		else if (itemIndex <= -1)
		{
			WORLD->mapContainerList.push_back(inventory[containerIndex]);

			//inventory[containerIndex]->itemList.erase(inventory[containerIndex]->itemList.begin() + itemIndex);
			inventory.erase(inventory.begin() + containerIndex);
		}
	}
}

void Player::filterIndexes()
{
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

void Player::update()
{
	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	if (INPUT->num0->isSwitched)
	{
		inventory[containerIndex]->addItem(ITEM_Test2_Size2(0, 0, 0));
	}
	if (INPUT->num9->isSwitched)
	{
		inventory.push_back(CONTAINER_Default_Container(0, 0, 0, 7));
		inventory[containerIndex]->addItem(ITEM_Test_Size2(0, 0, 0));
	}

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

	//INDEX FILTERING
	//
	//ITEM INDEXES
	//	-2 : no item selected
	//	-1 : container selected
	//	0+ : item in vector
	
	filterIndexes();

	angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3);

	selectedItem->updateTool(renderPosition.x, renderPosition.y, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3, angle, mapPosition.level);
	
	if (engine->settings->input->space->isSwitched == true)
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
