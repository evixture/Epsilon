#include "main.hpp"

//MagazineData::MagazineData(Weapon::AmmoType ammoType, int ammoCapacity, int availableAmmo, bool isValid)
//	:isValid(isValid), ammoType(ammoType), ammoCapacity(ammoCapacity), availableAmmo(availableAmmo)
//{
//}

Action::Action(std::string name, std::function<void()> action, Type actionType) // function, Action::Type::RELOAD
	:name(name), type(actionType), action(action)
{
	//action();
	//this->action = action;
}

ActionManager::ActionManager(std::vector<std::shared_ptr<Action>> actionList)
	:actionList(actionList), actionIndex(0), selectedAction(this->actionList[actionIndex])
{
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
		actionIndex++;
		selectedAction = actionList[actionIndex];
	}
}

void ActionManager::doAction()
{
	selectedAction->action();
}

//ITEM STRUCT
Item::Item(int size, std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool, Position position, Player* owner)
	: tile(tile), tool(tool), mapPosition(position), renderPosition(position), size(size), distToEnt(0)
{
	//test = Test();

	//local allows build
	//std::shared_ptr<Player> player;

	//gives error : 
	//WORLD->entityList[0];
	//WORLD->player;

	//cant access player stats
	//err here

	//std::function<void()> func = std::bind(&Test::testFunction);

	//std::function<void()> function = std::bind(&Test::testFunction, test);
	//std::shared_ptr<Action> testAction = std::make_shared<Action>("reload", std::bind(&Test::testFunction, test), Action::Type::RELOAD);

	//std::vector<std::shared_ptr<Action>> testActionManager; //std::vector<std::shared_ptr<Action>>{ testAction };
	//testActionManager.push_back(testAction);

	actionManager = std::make_shared<ActionManager>(std::vector<std::shared_ptr<Action>> {std::shared_ptr<Action>(std::make_shared<Action>("Reload", std::bind(&Player::reload, owner), Action::Type::RELOAD)),
		std::shared_ptr<Action>(std::make_shared<Action>("Drop", std::bind(&Player::dropItem, owner), Action::Type::DROP))});
}

std::shared_ptr<MagazineData> Item::getMagazineData()
{
	return std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false);
}

void Item::updateTool(int x, int y, int mx, int my, double angle, int level)
{
	tool->update(x, y, mx, my, angle);

	mapPosition = Position(tool->sourcex + WORLD->xOffset, tool->sourcey + WORLD->yOffset, level);

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Item::renderTool(const std::shared_ptr<Pane>& pane) const
{
	tool->render(pane);
}

void Item::updateTile()
{
	distToEnt = getDistance(WORLD->player->mapPosition.x, WORLD->player->mapPosition.y, mapPosition.x, mapPosition.y);
	//mapPosition = Position(tool->toolx, tool->tooly, 0);

	renderPosition = offsetPosition(mapPosition, WORLD->xOffset, WORLD->yOffset);
}

void Item::renderTile(const std::shared_ptr<Pane>& pane) const
{
	tile->render(renderPosition.x, renderPosition.y, pane);

	if (distToEnt < 5)
	{
		pane->console->setCharBackground(renderPosition.x, renderPosition.y, tile->backgroundColor + TCODColor::darkGrey);
	}
}

//----------------------------------------------------------------------------------------------------

//Container Struct
Container::Container(int itemCapacity, std::shared_ptr<Item> containerItem)
	:itemCapacity(itemCapacity), currentSize(0), containerItem(containerItem)
{
}

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
}

std::shared_ptr<MagazineData> MagazineItem::getMagazineData()
{
	return magazineData;
}

