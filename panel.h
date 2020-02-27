#include "main.hpp"

struct StatusWindow : public Window //player status pane derived window that renders player attributes
{
	StatusWindow(int consoleWidth, int consoleHeight, int rx, int ry); //window constructor that takes console width and heightm and render coordinates

	void update(); //updates the window
	void render() const; //renders the window

private:
	unsigned short int displayHealth; //the health that is displayed
	unsigned short int displayArmor; //the armor that is displayed
};

struct MapWindow : public Window //map pane derived window that handles the world
{
	std::shared_ptr<Pane> mapSidePanel; //the side bar on the map window
	std::shared_ptr<World> world; //the world

	MapWindow(int consoleWidth, int consoleHeight, int rx, int ry); //window constructor that takes console width and heightm and render coordinates

	void update(); //updates the window
	void render() const; //renders the window
};

struct PlayerWindow : public Window //player pane that renders player speed and stance
{
	PlayerWindow(int consoleWidth, int consoleHeight, int rx, int ry); //window constructor that takes console width and heightm and render coordinates

	void update(); //updates the window
	void render() const; //renders the window

private:
	void renderSpeedAndStance() const;

	int playerSpeed; //string of player's speed
	int playerStance; //string of player's stance
};

struct InventoryWindow : public Window //inventory pane that renders what is in the player's inventory
{
	InventoryWindow(int consoleWidth, int consoleHeight, int rx, int ry); //window constructor that takes console width and heightm and render coordinates

	void update(); //updates the window
	void render() const; //renders the window

private:
	std::vector<std::shared_ptr<Container>> inventoryItemList; //inventory
};

struct SplashWindow : public Window //the startup window
{
	SplashWindow(int consoleWidth, int consoleHeight, int rx, int ry); //window constructor that takes console width and heightm and render coordinates

	void update(); //updates the window
	void render() const; //renders the window

private:
	Menu menu;

	void renderLargeLogo() const;
	void renderMenuOptions() const;
};

struct InventoryFullWindow : public Window //full size inventory window
{
	InventoryFullWindow(int consoleWidth, int consoleHeight, int rx, int ry); //window constructor that takes console width and height and render coordinates

	void update(); //updates the window
	void render() const; //renders the window

private:
	std::vector<std::shared_ptr<Container>> inventoryItemList; //inventory
};

struct LogWindow : public Window //log event window
{
	struct Message
	{
		enum class MessageLevel {LOW, MEDIUM, HIGH} messageLevel;
		TCODColor color;
		std::string message;

		Message(std::string message, MessageLevel messageLevel);
	};

	LogWindow(int consoleWidth, int consoleHeight, int rx, int ry); //window constructor that takes console width and heightm and render coordinates

	void pushMessage(Message message); //pushes a string message to the log window

	void update(); //updates the window
	void render() const; //renders the window

private:
	std::vector<Message> messageList; //list of string messages to be displayed
};

struct ProximityWindow : public Window //shows the items in the player's proximity
{
	ProximityWindow(int consoleWidth, int consoleHeight, int rx, int ry); //window constructor that takes console width and heightm and render coordinates

	void update(); //updates the window
	void render() const; //renders the window

private:
	std::vector<std::shared_ptr<Container>> proximityContainerList; //containers in the player's proximity
	std::vector<std::shared_ptr<Item>> proximityItemList; //items in the player's proximity
};

struct ActionWindow : public Window //shows the availible actions for the player's selected tool
{
	ActionWindow(int consoleWidth, int consoleHeight, int rx, int ry); //window constructor that takes console width and heightm and render coordinates

	void update(); //updates the window
	void render() const; //renders the window

private:
	std::shared_ptr<ActionManager> actionManager; //the list of availible actions
};

struct PauseWindow : public Window
{
	PauseWindow(int consoleWidth, int consoleHeight, int rx, int ry);

	void update();
	void render() const; //renders the window

private:
	Menu baseMenu;
	Menu settingsMenu;

	bool baseMenuActive;
	bool settingsMenuActive;
};