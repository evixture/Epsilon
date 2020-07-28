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
	: owner(creature), size(size), block(block), tool(tool), mapPosition(position), tileRenderPosition(position), distToEnt(5), type(type), barColor(TCODColor::white),
	lastKnownMapPosition(position), inFov(false), discovered(false), inAir(false)
{
	createActionManager();

	onMap = (owner == nullptr) ? true : false;

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

bool Item::reload(MagazineData& magazine)
{
	if (!onMap)
	{
		if (type == ItemType::FIREARM)
		{
			return tool->reload(magazine);
		}
	}
	return false;
}

bool Item::changeFireMode()
{
	if (!onMap)
	{
		if (type == ItemType::FIREARM)
		{
			tool->changeFireMode();
			return true;
		}
	}
	return false;
}

bool Item::useMelee()
{
	if (!onMap)
	{
		if (type == ItemType::MELEE || type == ItemType::FIREARM)
		{
			tool->useMelee();
			return true;
		}
	}
	return false;
}

bool Item::equip(Armor& armor)
{
	if (!onMap)
	{
		if (type == ItemType::ARMOR)
		{
			tool->equip(armor);
			return true;
		}
	}
	return false;
}

bool Item::use(bool hold, bool swtch)
{
	if (!onMap)
	{
		tool->use(hold, swtch);
		return true;
	}
	return false;
}

bool Item::pickUp(Creature* owner)
{
	if (onMap)
	{
		if (owner->containerIndex != -1)
		{
			if (owner->inventory[owner->containerIndex]->addItem(std::make_shared<Item>(*this)))
			{
				//WORLD->debugmap->mapItemList.erase(WORLD->debugmap->mapItemList.begin() + i); //how to delete on map vec?

				for (int i = 0; i < WORLD->debugmap->mapItemList.size(); i++)
				{
					if (this == WORLD->debugmap->mapItemList[i].get())
					{
						WORLD->debugmap->mapItemList.erase(WORLD->debugmap->mapItemList.begin() + i);
					}
				}

				this->owner = owner;
				onMap = false;
				
				return true;
			}
		}
	}
	return false;
}

void Item::drop(Creature* owner)
{
	if (!onMap)
	{
		WORLD->debugmap->mapItemList.push_back(std::make_shared<Item>(*this));
		owner->inventory[owner->containerIndex]->itemList.erase(owner->inventory[owner->containerIndex]->itemList.begin() + owner->itemIndex);

		onMap = true;
		this->owner = nullptr;
	}
}

void Item::throwItem()
{
	if (!onMap)
	{
		WORLD->debugmap->mapItemList.push_back(std::make_shared<Item>(*this));
		owner->inventory[owner->containerIndex]->itemList.erase(owner->inventory[owner->containerIndex]->itemList.begin() + owner->itemIndex);
		owner = nullptr;

		//Projectile(const Creature* owner, int ch, std::string name, TCODColor color, const Position4 startPosition, Position2 targetPosition, int velocity, int mass);
		projectile = std::make_shared<Projectile>(owner, tool->ch, tool->name, tool->color, mapPosition, owner->targetPosition, 80, 240);

		inAir = true;
	}
}

void Item::updateTool(Position4& mapPosition, int xMouse, int yMouse, bool isHeld)
{
	inFov = WORLD->isInPlayerFov(mapPosition);
	if (inFov) discovered = true;

	tool->update(mapPosition, xMouse, yMouse, isHeld);
	actionManager->update();

	changeBarColor();

	this->mapPosition = Position4(tool->sourcePosition.x, tool->sourcePosition.y, mapPosition.h, mapPosition.z);
	tileRenderPosition = Position4(mapPosition.x - WORLD->xOffset, mapPosition.y - WORLD->yOffset, mapPosition.h, mapPosition.z); //replace with better way?
}

void Item::updateTile()
{
	inFov = WORLD->isInPlayerFov(mapPosition);
	if (inFov)
	{
		discovered = true;
		lastKnownMapPosition = mapPosition;
	}

	mapPosition.h = WORLD->debugmap->player->mapPosition.h;
	distToEnt = getDistance(WORLD->debugmap->player->mapPosition.x, WORLD->debugmap->player->mapPosition.y, mapPosition.x, mapPosition.y);

	if (inAir)
	{
		projectile->update();
		mapPosition = projectile->mapPosition;

		if (projectile->onGround)
		{
			inAir = false;
			onMap = true;
		}
	}

	tileRenderPosition = Position4(mapPosition.x - WORLD->xOffset, mapPosition.y - WORLD->yOffset, mapPosition.h, mapPosition.z); //replace with better way?
}

void Item::render(const Pane& pane) const
{
	if (discovered)
	{
		if (onMap)
		{
			block->render(Position4(tileRenderPosition.x, tileRenderPosition.y, WORLD->debugmap->player->mapPosition.h, tileRenderPosition.z), pane);

			if (distToEnt < 5 && inFov)
			{
				pane.console->setCharBackground(tileRenderPosition.x, tileRenderPosition.y, block->tileList[0].backgroundColor + TCODColor::darkGrey); //look into color later
			}

			if (!inFov)
			{
				pane.console->setChar(tileRenderPosition.x, tileRenderPosition.y, '?');
				pane.console->setCharForeground(tileRenderPosition.x, tileRenderPosition.y, TCODColor::darkerGrey); //look into color later
			}
		}
		else
		{
			tool->render(pane);
		}
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

bool Container::pickUp(Creature* owner)
{
	if (item->onMap)
	{
		owner->inventory.push_back(std::make_shared<Container>(*this));
		//WORLD->debugmap->mapContainerList.erase(WORLD->debugmap->mapContainerList.begin() + i);
		for (int i = 0; i < WORLD->debugmap->mapContainerList.size(); i++)
		{
			if (this == WORLD->debugmap->mapContainerList[i].get()) WORLD->debugmap->mapContainerList.erase(WORLD->debugmap->mapContainerList.begin() + i);
		}

		this->item->owner = owner;
		item->onMap = false;

		return true;
	}

	return false;
}

void Container::drop(Creature* owner)
{
	if (!item->onMap)
	{
		WORLD->debugmap->mapContainerList.push_back(std::make_shared<Container>(*this));
		owner->inventory.erase(owner->inventory.begin() + owner->containerIndex);

		item->onMap = true;
		this->item->owner = nullptr;
	}
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