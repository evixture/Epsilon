#include "main.hpp"

Action::Action(std::string name, std::function<void()> action, Type actionType)
	:name(name), action(action), type(actionType)
{}

void Action::update()
{
	if (type == Type::CHANGEFIREMODE)
	{
		if (WORLD->player->selectedItem->tool->fireMode == Tool::SAFE)
		{
			name = "Change Fire Mode : SAFE";
		}
		else if (WORLD->player->selectedItem->tool->fireMode == Tool::SEMI)
		{
			name = "Change Fire Mode : SEMI";
		}
		else if (WORLD->player->selectedItem->tool->fireMode == Tool::FULL)
		{
			name = "Change Fire Mode : FULL";
		}
	}
}

//----------------------------------------------------------------------------------------------------

ActionManager::ActionManager(std::vector<std::shared_ptr<Action>> actionList)
	:actionList(actionList), actionIndex(0), selectedAction(this->actionList[actionIndex])
{}

void ActionManager::update()
{
	for (auto& action : actionList)
	{
		action->update();
	}
}

void ActionManager::moveSelectorUp()
{
	if (actionIndex > 0)
	{
		actionIndex--;
		selectedAction = actionList[actionIndex];
	}
}

void ActionManager::moveSelectorDown()
{
	if (actionIndex + 1 < actionList.size())
	{
		++actionIndex;
		selectedAction = actionList[actionIndex];
	}
}

void ActionManager::doAction()
{
	selectedAction->action();
}

//----------------------------------------------------------------------------------------------------

Item::Item(int size, std::shared_ptr<Block> tile, std::shared_ptr<Tool> tool, Position4 position, Player* owner, ItemType type)
	:size(size), tile(tile), tool(tool), mapPosition(position), renderPosition(position), distToEnt(5), owner(owner), type(type), barColor(TCODColor::white)
{
	createActionManager(owner);
}

void Item::createActionManager(Player* owner)
{
	if (type == ItemType::NORMAL)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Player::dropItem, owner), Action::Type::DROP))
		});
	}
	else if (type == ItemType::MAGAZINE)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Player::dropItem, owner), Action::Type::DROP))
		});
	}
	else if (type == ItemType::FIREARM)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Reload", std::bind(&Player::reload, owner), Action::Type::RELOAD)),
			std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Player::dropItem, owner), Action::Type::DROP)),
			std::shared_ptr<Action>(std::make_shared<Action>("Change Fire Mode", std::bind(&Player::changeFireMode, owner), Action::Type::CHANGEFIREMODE))
		});
	}
	else if (type == ItemType::ARMOR)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Player::dropItem, owner), Action::Type::DROP)),
			std::shared_ptr<Action>(std::make_shared<Action>("Equip", std::bind(&Player::equipArmor, owner), Action::Type::EQUIP))
		});
	}
	else// (type == ItemType::NODROP)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("", std::bind(&Player::filterIndexes, owner), Action::Type::DROP))
		});
	}
}

std::shared_ptr<MagazineData> Item::getMagazineData()
{
	return std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false);
}

void Item::changeBarColor()
{
	tool->changeBarColor(barColor);
}

void Item::updateTool(Position4 mapPosition, int mx, int my, double angle, bool isHeld)
{
	tool->update(mapPosition, mx, my, angle, isHeld);
	actionManager->update();

	changeBarColor();

	this->mapPosition = Position4(tool->sourcePosition.x + WORLD->xOffset, tool->sourcePosition.y + WORLD->yOffset, mapPosition.height, mapPosition.level);
	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Item::renderTool(const std::shared_ptr<Pane>& pane) const
{
	tool->render(pane);
}

void Item::updateTile()
{
	distToEnt = getDistance(WORLD->player->mapPosition.x, WORLD->player->mapPosition.y, mapPosition.x, mapPosition.y);

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Item::renderTile(const std::shared_ptr<Pane>& pane) const
{
	tile->render(Position4(renderPosition.x, renderPosition.y, WORLD->player->mapPosition.height, renderPosition.level), pane);

	if (distToEnt < 5 && WORLD->isInFov(mapPosition))
	{
		pane->console->setCharBackground(renderPosition.x, renderPosition.y, tile->tileList[0]->backgroundColor + TCODColor::darkGrey);
	}
}

//----------------------------------------------------------------------------------------------------

Container::Container(int itemCapacity, std::shared_ptr<Item> containerItem)
	:itemCapacity(itemCapacity), containerItem(containerItem), currentSize(0)
{}

bool Container::addItem(std::shared_ptr<Item> item)
{
	int spaceUsed = 0;

	for (auto& i : itemList)
	{
		spaceUsed += i->size;
	}

	if (spaceUsed + item->size <= itemCapacity)
	{
		itemList.push_back(item);
		return true;
	}

	return false;
}

MagazineItem::MagazineItem(Item item, std::shared_ptr<MagazineData> magazineData)
	:Item(item), magazineData(magazineData)
{
	createActionManager(item.owner);
}

std::shared_ptr<MagazineData> MagazineItem::getMagazineData()
{
	return magazineData;
}

void MagazineItem::changeBarColor()
{
	barColor = TCODColor::lerp(TCODColor::red, TCODColor::darkerGreen, (float(magazineData->availableAmmo) / float(magazineData->ammoCapacity)));
}
