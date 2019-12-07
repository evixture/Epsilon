#include "main.hpp"

//MagazineData::MagazineData(Weapon::AmmoType ammoType, int ammoCapacity, int availableAmmo, bool isValid)
//	:isValid(isValid), ammoType(ammoType), ammoCapacity(ammoCapacity), availableAmmo(availableAmmo)
//{
//}

//ITEM STRUCT
Item::Item(int size, std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool, Position position)
	: tile(tile), tool(tool), mapPosition(position), renderPosition(position), size(size), distToEnt(0)
{
}

std::shared_ptr<MagazineData> Item::getMagazineData()
{
	return std::make_shared<MagazineData>(MagazineData::AmmoType::NONE, 0, 0, false);
}

void Item::updateTool(int x, int y, int mx, int my, double angle, int level, std::shared_ptr<MagazineData> magData)
{
	tool->update(x, y, mx, my, angle, magData);

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
