#include "main.hpp"

Action::Action(std::string name, std::function<void()> action, Type actionType)
	:name(name), action(action), type(actionType)
{}

//----------------------------------------------------------------------------------------------------

ActionManager::ActionManager(std::vector<std::shared_ptr<Action>> actionList)
	:actionList(actionList), actionIndex(0), selectedAction(this->actionList[actionIndex])
{}

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
		actionIndex++;
		selectedAction = actionList[actionIndex];
	}
}

void ActionManager::doAction()
{
	selectedAction->action();
}

//----------------------------------------------------------------------------------------------------

void Item::createActionManager(Player* owner)
{
	if (type == ItemType::NODROP)
	{

	}
	else if (type == ItemType::NORMAL)
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
}

Item::Item(int size, std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool, Position position, Player* owner, ItemType type)
	:size(size), tile(tile), tool(tool), mapPosition(position), renderPosition(position), distToEnt(0), owner(owner), type(type)
{
	createActionManager(owner);
}

std::shared_ptr<MagazineData> Item::getMagazineData()
{
	return std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false);
}

void Item::updateTool(Position mapPosition, int mx, int my, double angle)
{
	tool->update(mapPosition, mx, my, angle);

	this->mapPosition = Position(tool->sourcePosition.x + WORLD->xOffset, tool->sourcePosition.y + WORLD->yOffset, mapPosition.level);
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
	tile->render(renderPosition.x, renderPosition.y, pane);

	if (distToEnt < 5 && WORLD->isInFov(mapPosition))
	{
		pane->console->setCharBackground(renderPosition.x, renderPosition.y, tile->backgroundColor + TCODColor::darkGrey);
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
