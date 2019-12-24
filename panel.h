#include "main.hpp"

struct StatusPane : public Window
{
	int displayHealth;
	int displayArmor;

	StatusPane(int consoleWidth, int windowH, int rx, int ry);

	void update();

	void render() const;
};

struct MapPane : public Window
{
	std::shared_ptr<Pane> mapSidePanel;

	std::shared_ptr<World> world;

	MapPane(int consoleWidth, int windowH, int rx, int ry);

	void update();

	void render() const;
};

struct PlayerPane : public Window
{
	std::string playerSpeed;
	std::string playerStance;

	PlayerPane(int consoleWidth, int windowH, int rx, int ry);

	void update();

	void render() const;
};

struct InventoryPane : public Window
{
	std::vector<std::shared_ptr<Container>> inventoryItemList;

	InventoryPane(int consoleWidth, int windowH, int rx, int ry);

	void update();
	void render() const;
};

struct SplashPane : public Window
{
	int menuIndex;
	std::string menuSelection;
	std::vector<std::string> menuItemList;

	SplashPane(int consoleWidth, int windowH, int rx, int ry);

	void update();
	void render() const;

private:
	void renderLargeLogo() const;
	void renderMenuOptions() const;
};

struct InventoryFullPane : public Window
{
	std::vector<std::shared_ptr<Container>> inventoryItemList;

	InventoryFullPane(int consoleWidth, int windowH, int rx, int ry);

	void update();
	void render() const;
};

struct LogPane : public Window
{
	std::vector<std::string> messageList;

	LogPane(int consoleWidth, int windowH, int rx, int ry);

	void pushMessage(std::string message);

	void update();
	void render() const;
};

struct ProximityPane : public Window
{
	std::vector<std::shared_ptr<Container>> proximityContainerList;
	std::vector<std::shared_ptr<Item>> proximityItemList;

	ProximityPane(int consoleWidth, int windowH, int rx, int ry);

	void update();
	void render() const;
};

struct ActionPane : public Window
{
	std::shared_ptr<ActionManager> actionManager;

	ActionPane(int consoleWidth, int windowH, int rx, int ry);

	void update();
	void render() const;
};