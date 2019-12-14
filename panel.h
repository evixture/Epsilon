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
	std::string playerSpeed;
	std::string playerStance;

	PlayerPane(int windowW, int windowH, int rx, int ry);

	void update();

	void render() const;
};

struct InventoryPane : public Window
{
	std::vector<std::shared_ptr<Container>> inventoryItemList;

	InventoryPane(int windowW, int windowH, int rx, int ry);

	void update();
	void render() const;
};

struct SplashPane : public Window
{
	int menuIndex;
	std::string menuSelection;
	std::vector<std::string> menuItemList;

	SplashPane(int windowW, int windowH, int rx, int ry);

	void update();
	void render() const;

private:
	void renderLargeLogo() const;
	void renderMenuOptions() const;
};

struct InventoryFullPane : public Window
{
	std::vector<std::shared_ptr<Container>> inventoryItemList;

	InventoryFullPane(int windowW, int windowH, int rx, int ry);

	void update();
	void render() const;
};

struct LogPane : public Window
{
	std::vector<std::string> messageList;

	LogPane(int windowW, int windowH, int rx, int ry);

	void pushMessage(std::string message);

	void update();
	void render() const;
};