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
	std::vector<std::shared_ptr<Container>> inventoryItemList;

	InventoryPane(int windowW, int windowH, int rx, int ry);

	void update();
	void render() const;
};

struct SplashPane : public Window
{
	int menuIndex;
	const char* menuSelection;
	std::vector<const char*> menuItemList;

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
	std::vector<const char*> messageList;

	LogPane(int windowW, int windowH, int rx, int ry);

	void pushMessage(const char* message);

	void update();
	void render() const;
};