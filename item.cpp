#include "main.hpp"

Action::Action(std::string name, Type actionType)
	: name(name), type(actionType)
{}

void Action::update()
{
	if (type == Type::CHANGEFIREMODE)
	{
		if		(WORLD->debugmap->player->selectedItem->tool->fireMode == Tool::SAFE) name = "Change Fire Mode : SAFE";
		else if (WORLD->debugmap->player->selectedItem->tool->fireMode == Tool::SEMI) name = "Change Fire Mode : SEMI";
		else if (WORLD->debugmap->player->selectedItem->tool->fireMode == Tool::FULL) name = "Change Fire Mode : FULL";
	}
}

bool Action::operator==(const Action& compAction)
{
	return (this->name == compAction.name && this->type == compAction.type);
}

//----------------------------------------------------------------------------------------------------

ActionManager::ActionManager(std::vector<Action> actionList)
	: actionList(actionList), actionIndex(0), selectedAction(this->actionList[0])
{
}

void ActionManager::update()
{
	for (auto& action : actionList) action.update();
}

void ActionManager::moveSelectorUp()
{
	if (actionIndex > 0)
	{
		actionIndex--;
		selectedAction = actionList[actionIndex];

		AUDIO->playSound(Sound(("tick"), 0.0f, 5.0f));
	}
}

void ActionManager::moveSelectorDown()
{
	if (actionIndex + 1 < actionList.size())
	{
		++actionIndex;
		selectedAction = actionList[actionIndex];

		AUDIO->playSound(Sound(("tick"), 0.0f, 5.0f));
	}
}

void ActionManager::doAction(Creature* currentOwner)
{
	if (selectedAction.type == Action::Type::DROP)					std::bind(&Creature::dropItem, currentOwner)();
	else if (selectedAction.type == Action::Type::RELOAD)			std::bind(&Creature::reload, currentOwner)();
	else if (selectedAction.type == Action::Type::CHANGEFIREMODE)	std::bind(&Creature::changeFireMode, currentOwner)();
	else if (selectedAction.type == Action::Type::EQUIP)			std::bind(&Creature::equipArmor, currentOwner)();
	else if (selectedAction.type == Action::Type::MELEE)			std::bind(&Creature::useMelee, currentOwner)();
}

//----------------------------------------------------------------------------------------------------

Item::Item(const Creature* creature, int size, std::shared_ptr<Block> block, std::shared_ptr<Tool> tool, Position4 position, ItemType type)
	: owner(creature), size(size), block(block), tool(tool), mapPosition(position), tileRenderPosition(position), distToEnt(5), type(type), barColor(TCODColor::white), lastKnownMapPosition(position), inFov(false)
{
	createActionManager();

	if (type == ItemType::HAND) tool->effectiveRange = 5; //override effective range for hands
}

void Item::createActionManager()
{
	switch (type)
	{
	case ItemType::NORMAL:
	case ItemType::MAGAZINE:
		actionManager = std::make_shared<ActionManager>(std::vector<Action>{Action("Drop", Action::Type::DROP)});
		break;

	case ItemType::MELEE:
		actionManager = std::make_shared<ActionManager>(std::vector<Action>{Action("Drop", Action::Type::DROP), Action("Melee",Action::Type::MELEE)});
		break;

	case ItemType::FIREARM:
		actionManager = std::make_shared<ActionManager>
			(std::vector<Action>{Action("Reload",Action::Type::RELOAD), Action("Drop",Action::Type::DROP), Action("Change Fire Mode",Action::Type::CHANGEFIREMODE), Action("Melee",Action::Type::MELEE)});
		break;

	case ItemType::ARMOR:
		actionManager = std::make_shared<ActionManager>(std::vector<Action>{Action("Drop", Action::Type::DROP), Action("Equip", Action::Type::EQUIP)});
		break;

	case ItemType::HAND:
		actionManager = std::make_shared<ActionManager>(std::vector<Action>{Action("Melee", Action::Type::MELEE)});
		break;

	default:
		actionManager = std::make_shared<ActionManager>(std::vector<Action>{Action("ERROR", Action::Type::EQUIP)});
		break;
	}
}

std::pair<bool, MagazineData&> Item::getMagazineData()
{
	MagazineData nullMag = MagazineData(MagazineData::AmmoType::NONE, 0, 0);
	return std::pair<bool, MagazineData&>(false, nullMag);
}

void Item::changeBarColor()
{
	tool->changeBarColor(barColor);
}

void Item::updateTool(Position4& mapPosition, int xMouse, int yMouse, bool isHeld)
{
	inFov = WORLD->isInPlayerFov(mapPosition); //need in tool?

	tool->update(mapPosition, xMouse, yMouse, isHeld);
	actionManager->update();

	changeBarColor();

	this->mapPosition = Position4(tool->sourcePosition.x, tool->sourcePosition.y, mapPosition.h, mapPosition.z);
	tileRenderPosition = Position4(mapPosition.x - WORLD->xOffset, mapPosition.y - WORLD->yOffset, mapPosition.h, mapPosition.z); //replace with better way?
}

void Item::renderTool(const Pane& pane) const
{
	tool->render(pane);
}

void Item::updateTile()
{
	inFov = WORLD->isInPlayerFov(mapPosition);

	mapPosition.h = WORLD->debugmap->player->mapPosition.h;
	distToEnt = getDistance(WORLD->debugmap->player->mapPosition.x, WORLD->debugmap->player->mapPosition.y, mapPosition.x, mapPosition.y);

	if (inFov) lastKnownMapPosition = mapPosition; //need renderPosition?

	tileRenderPosition = Position4(mapPosition.x - WORLD->xOffset, mapPosition.y - WORLD->yOffset, mapPosition.h, mapPosition.z); //replace with better way?

}

void Item::renderTile(const Pane& pane) const
{
	block->render(Position4(tileRenderPosition.x, tileRenderPosition.y, WORLD->debugmap->player->mapPosition.h, tileRenderPosition.z), pane);

	if (distToEnt < 5 && inFov)
	{
		pane.console->setCharBackground(tileRenderPosition.x, tileRenderPosition.y, block->tileList[0].backgroundColor + TCODColor::darkGrey); //look into color later
	}

	if (!inFov)
	{
		pane.console->setChar(tileRenderPosition.x, tileRenderPosition.y, '?');
		pane.console->setCharForeground(tileRenderPosition.x, tileRenderPosition.y, TCODColor::darkestGrey); //look into color later
	}
}

//----------------------------------------------------------------------------------------------------

MagazineItem::MagazineItem(Item item, MagazineData magazineData)
	: Item(item), magazineData(magazineData)
{
}

std::pair<bool, MagazineData&> MagazineItem::getMagazineData()
{
	return std::pair<bool, MagazineData&>(true, magazineData);
}

void MagazineItem::changeBarColor()
{
	if (magazineData.ammoCapacity != 0) barColor = TCODColor::lerp(TCODColor::red, TCODColor::darkerGreen, (float(magazineData.availableAmmo) / float(magazineData.ammoCapacity)));
	else barColor = TCODColor::red;
}

//----------------------------------------------------------------------------------------------------

Container::Container(int itemCapacity, std::shared_ptr<Item> item)
	: itemCapacity(itemCapacity), item(item), currentSize(0)
{
}

Container::Container(int itemCapacity, std::shared_ptr<Item> item, std::vector<std::shared_ptr<Item>> itemList)
	: itemCapacity(itemCapacity), item(item), currentSize(0), itemList(itemList)
{
}

bool Container::addItem(std::shared_ptr<Item> item)
{
	int spaceUsed = 0;

	for (auto& i : itemList) spaceUsed += i->size;

	if (spaceUsed + item->size <= itemCapacity)
	{
		itemList.push_back(item);
		return true;
	}

	return false;
}