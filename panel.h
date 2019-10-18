#include "main.hpp"

struct StatusPane : public Window
{
	int displayHealth;
	int displayArmor;

	StatusPane(int windowW, int windowH, int rx, int ry);

	void update();

	void render() const;
};

struct MapPane : public Window
{
	std::shared_ptr<Pane> mapSidePanel;

	std::shared_ptr<World> world;

	MapPane(int windowW, int windowH, int rx, int ry);

	void update();

	void render() const;
};

struct PlayerPane : public Window
{
	const char* playerSpeed;
	const char* playerStance;

	PlayerPane(int windowW, int windowH, int rx, int ry);

	void update();

	void render() const;
};

struct InventoryPane : public Window
{
	std::vector<const char*> itemNameList;

	InventoryPane(int windowW, int windowH, int rx, int ry);

	void update();
	void render() const;
};