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

//Player Class
Player::Player(Position pos)
	:Creature(Position(pos), '@', "player", TCODColor::azure, 100, 0)
{
	inventory.push_back(std::make_shared<Container>("Test Container Cap 5", 5));
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

void Player::update()
{
	renderPosition = offSetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);

	if (INPUT->num0->isSwitched)
	{
		inventory[containerIndex]->addItem(ITEM_Test2_Size2(0, 0, 0));
	}
	if (INPUT->num9->isSwitched)
	{
		inventory.push_back(std::make_shared<Container>("Test Container Cap 7", 7));
		inventory[1]->addItem(ITEM_Test_Size2(0, 0, 0));
	}

	//MOUSE WHEEL ITEM SELECTION
	if (INPUT->mouse.wheel_up || INPUT->mouse.wheel_down)
	{
		if (INPUT->mouse.wheel_up)
		{
			if (itemIndex > 0)
			{
				itemIndex--;
			}
			
			else if (itemIndex == 0)
			{
				if (containerIndex > 0)
				{
					containerIndex--;
					itemIndex = inventory[containerIndex]->itemList.size() - 1;
				}				
			}

			else if (itemIndex == -1)
			{
				if (itemIndex + 2 <= itemIndex < inventory[containerIndex]->itemList.size())
				{
					itemIndex++;
				}
			}
		}

		else if (INPUT->mouse.wheel_down)
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
					if (itemIndex + 2 <= itemIndex < inventory[containerIndex]->itemList.size())
					{
						itemIndex++;
					}
				}
			}
		}
	}

	//INDEX FILTERING
	if (containerIndex > (inventory.size() - 1))
	{
		containerIndex = inventory.size() - 1;
	}

	if (containerIndex != -1)
	{
		//int temp = inventory[containerIndex]->itemList.size() - 1;
		if (itemIndex + 1 > inventory[containerIndex]->itemList.size())
		{
			//not going through after item delete
			//fixed
			itemIndex = inventory[containerIndex]->itemList.size() - 1;
		}

		if (itemIndex != -1)
		{
			//error when fail to filter index
			//fixed
			selectedItem = inventory[containerIndex]->itemList[itemIndex];
		}
		else
		{
			selectedItem = ITEM_Hands(0, 0, 0);
		}
	}

	angle = getAngle(renderPosition.x, renderPosition.y, engine->settings->input->mouse.cx - 1, engine->settings->input->mouse.cy - 3);

	if (itemIndex != -1)
	{
		selectedItem->updateTool(renderPosition.x, renderPosition.y, INPUT->mouse.cx - 1, INPUT->mouse.cy - 3, angle);
	}

	if (engine->settings->input->space->isSwitched == true)
	{
		if (WORLD->getTile(mapPosition.x, mapPosition.y, mapPosition.level)->tag == "stair")
		{
			WORLD->getTile(mapPosition.x, mapPosition.y, mapPosition.level)->interact();
		}
	}

	if (INPUT->e->isSwitched)
	{
		for (auto& item : WORLD->mapItemList)
		{
			if (item->mapPosition.x == mapPosition.x && item->mapPosition.y == mapPosition.y)
			{
				//error if picking up 2 items with same coords
				if (inventory[containerIndex]->addItem(item))
				{
					WORLD->mapItemList.erase(std::remove(WORLD->mapItemList.begin(), WORLD->mapItemList.end(), item), WORLD->mapItemList.end());
				}
			}
		}
	}

	if (INPUT->q->isSwitched)
	{	
		if (containerIndex != -1 && itemIndex != -1)
		{
			WORLD->mapItemList.push_back(selectedItem);
		
			inventory[containerIndex]->itemList.erase(inventory[containerIndex]->itemList.begin() + itemIndex);
		}
	}
}

void Player::render(const std::shared_ptr<Pane>& pane) const
{
	pane->console->setChar(renderPosition.x, renderPosition.y, symbol);
	pane->console->setCharForeground(renderPosition.x, renderPosition.y, color);

	selectedItem->renderTool(pane);
}
