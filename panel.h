#include "main.hpp"

struct StatusWindow : public Window
{
	StatusWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const;

private:
	Bar healthBar;
	Bar armorBar;
};

struct MapWindow : public Window
{
	std::shared_ptr<Pane> mapSidePanel;
	std::shared_ptr<World> world;

	MapWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const;
};

struct PlayerWindow : public Window
{
	PlayerWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const;

private:
	void renderSpeedAndStance() const;

	int playerSpeed;
	int playerStance;
};

struct InventoryWindow : public Window
{
	InventoryWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const;

private:
	std::vector<std::shared_ptr<Container>> inventoryItemList;
};

struct SplashWindow : public Window
{

	SplashWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const;

private:
	Menu menu;

	std::vector<char> slashList;

	const int numSlashIndexes;
	std::vector<int> slashIndexList;

	Clock slashClock;

	void renderLargeLogo() const;
	void renderMenuOptions() const;
};

struct InventoryFullWindow : public Window
{
	InventoryFullWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const;

private:
	std::vector<std::shared_ptr<Container>> inventoryItemList;
};

struct LogWindow : public Window
{
	struct Message
	{
		enum class MessageLevel {LOW, MEDIUM, HIGH} messageLevel;
		TCODColor color;
		std::string message;

		Message(std::string message, MessageLevel messageLevel);
	};

	LogWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void pushMessage(Message message);

	void update();
	void render() const;

private:
	std::vector<Message> messageList;
};

struct ProximityWindow : public Window
{
	ProximityWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const;

private:
	std::vector<std::shared_ptr<Container>> proximityContainerList;
	std::vector<std::shared_ptr<Item>> proximityItemList;
};

struct ActionWindow : public Window
{
	ActionWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const;

private:
	std::shared_ptr<ActionManager> actionManager;
};

struct PauseWindow : public Window
{
	PauseWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const;

private:
	Menu baseMenu;
	Menu settingsMenu;

	bool baseMenuActive;
	bool settingsMenuActive;
};

struct InfoWindow : public Window
{
	InfoWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	std::string tileDetail;
	std::string creatureDetail;
	std::string itemDetail;

	void update();
	void render() const;

private:
	void setTileDetails();
	void setCreatureDetails();
	void setItemDetails();
};