#include "main.hpp"

struct Gui //contains all of the window panels used for rendering everything that is important to the screen
{
	enum class ActiveWindow { NONE, STARTUPSPLASH, INVENTORYFULL} activeWindow; //the large windows that are active like the startup splash and big inventory

	//Part-screen Windows
	std::shared_ptr<PlayerWindow> playerWindow; //displays the character's health, armor, etc
	std::shared_ptr<StatusWindow> statusWindow; //displays the player's stance and movement speed
	std::shared_ptr<InventoryWindow> inventoryWindow; //shows what is in the player's inventory
	std::shared_ptr<LogWindow> logWindow; //shows the recent events pushed to the event log
	std::shared_ptr<ProximityWindow> proximityWindow; //shows what items are in the player's proximity
	std::shared_ptr<ActionWindow> actionWindow; //shows the availible actions for the player's selected item

	//Half Screen Windows
	std::shared_ptr<MapWindow> worldWindow; //the pane used for everything regarding the map
	std::shared_ptr<InventoryFullWindow> inventoryFullWindow; //shows what is in the player's inventory in a larger view

	//Full-screen Windows
	std::shared_ptr<SplashWindow> startupSplashWindow; //the startup main menu that shows initial menu options and the large logo

	Gui(int windowX, int windowY);

	void update();
	void render() const;

private:
	std::vector<std::shared_ptr<Window>> windowList; //private list for easy itteration over all of the panes
};
