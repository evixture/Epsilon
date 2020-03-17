#include "main.hpp"

Action::Action(std::string name, std::function<void()> action, Type actionType)
	:name(name), action(action), type(actionType)
{}

void Action::update()
{
	if (type == Type::CHANGEFIREMODE)
	{
		if (WORLD->debugmap->player->selectedItem->tool->fireMode == Tool::SAFE)
		{
			name = "Change Fire Mode : SAFE";
		}
		else if (WORLD->debugmap->player->selectedItem->tool->fireMode == Tool::SEMI)
		{
			name = "Change Fire Mode : SEMI";
		}
		else if (WORLD->debugmap->player->selectedItem->tool->fireMode == Tool::FULL)
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

Item::Item(int size, std::shared_ptr<Block> tile, std::shared_ptr<Tool> tool, Position4 position, Creature* owner, ItemType type)
	:size(size), tile(tile), tool(tool), mapPosition(position), tileRenderPosition(position), distToEnt(5), owner(owner), type(type), barColor(TCODColor::white)
{
	createActionManager(owner);
}

void Item::createActionManager(Creature* owner)
{
	if (type == ItemType::NORMAL)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Creature::dropItem, owner), Action::Type::DROP))
		});
	}
	else if (type == ItemType::MAGAZINE)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Creature::dropItem, owner), Action::Type::DROP))
		});
	}
	else if (type == ItemType::MELEE)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Creature::dropItem, owner), Action::Type::DROP)),
			std::shared_ptr<Action>(std::make_shared<Action>("Melee",std::bind(&Creature::useMelee, owner), Action::Type::MELEE))
		});
	}
	else if (type == ItemType::FIREARM)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Reload", std::bind(&Creature::reload, owner), Action::Type::RELOAD)),
			std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Creature::dropItem, owner), Action::Type::DROP)),
			std::shared_ptr<Action>(std::make_shared<Action>("Change Fire Mode", std::bind(&Creature::changeFireMode, owner), Action::Type::CHANGEFIREMODE)),
			std::shared_ptr<Action>(std::make_shared<Action>("Melee", std::bind(&Creature::useMelee, owner), Action::Type::MELEE))
		});
	}
	else if (type == ItemType::ARMOR)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Creature::dropItem, owner), Action::Type::DROP)),
			std::shared_ptr<Action>(std::make_shared<Action>("Equip", std::bind(&Creature::equipArmor, owner), Action::Type::EQUIP))
		});
	}
	else if (type == ItemType::HAND)
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("Melee", std::bind(&Creature::useMelee, owner), Action::Type::MELEE))
		});
	}
	else
	{
		actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {
			std::shared_ptr<Action>(std::make_shared<Action>("ERROR", std::bind(&Creature::equipArmor, owner), Action::Type::EQUIP))
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

void Item::updateTool(Position4& mapPosition, int mx, int my, bool isHeld)
{
	tool->update(mapPosition, mx, my, isHeld);
	actionManager->update();

	changeBarColor();

	this->mapPosition = Position4(tool->sourcePosition.x, tool->sourcePosition.y, mapPosition.height, mapPosition.floor);
	tileRenderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Item::renderTool(const std::shared_ptr<Pane>& pane) const
{
	tool->render(pane);
}

void Item::updateTile()
{
	distToEnt = getDistance(WORLD->debugmap->player->mapPosition.x, WORLD->debugmap->player->mapPosition.y, mapPosition.x, mapPosition.y);

	tileRenderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Item::renderTile(const std::shared_ptr<Pane>& pane) const
{
	tile->render(Position4(tileRenderPosition.x, tileRenderPosition.y, WORLD->debugmap->player->mapPosition.height, tileRenderPosition.floor), pane);

	if (distToEnt < 5 && WORLD->isInPlayerFov(mapPosition))
	{
		pane->console->setCharBackground(tileRenderPosition.x, tileRenderPosition.y, tile->tileList[0].backgroundColor + TCODColor::darkGrey);
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
