#include "main.hpp"

struct Item
{
	int size;
	double distToEnt;

	Position mapPosition;
	Position renderPosition;

	std::shared_ptr<Tile> tile;
	std::shared_ptr<Tool> tool;

	Item(int size, std::shared_ptr<Tile> tile, std::shared_ptr<Tool> tool, Position position);

	virtual std::shared_ptr<MagazineData> getMagazineData();

	void updateTool(int x, int y, int mx, int my, double angle, int level);
	void renderTool(const std::shared_ptr<Pane>& pane) const;

	void updateTile();
	void renderTile(const std::shared_ptr<Pane>& pane) const;
};

struct MagazineItem : public Item
{
	std::shared_ptr<MagazineData> magazineData;

	MagazineItem(Item item, std::shared_ptr<MagazineData> magazineData);

	std::shared_ptr<MagazineData> getMagazineData();
};

struct Container
{
	int itemCapacity;
	int currentSize;

	std::shared_ptr<Item> containerItem;

	std::vector<std::shared_ptr<Item>> itemList;

	Container(int itemCapacity, std::shared_ptr<Item> containerItem);

	bool addItem(std::shared_ptr<Item> item);
};

