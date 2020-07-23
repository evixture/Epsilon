#include "main.hpp"

struct Gui
{
	enum class ActiveWindow { NONE, STARTUPSPLASH, INVENTORYFULL, PAUSE, COMMAND} activeWindow;
	enum class ActiveLowWindow { LOG, INFO } activeLowWindow;

	//Part-screen Windows
		//Top Windows
	std::shared_ptr<PlayerWindow> playerWindow;
	std::shared_ptr<StatusWindow> statusWindow;
		//Mid Windows
	std::shared_ptr<InventoryWindow> inventoryWindow;
	std::shared_ptr<ProximityWindow> proximityWindow;
	std::shared_ptr<ActionWindow> actionWindow;
		//Low Windows
	std::shared_ptr<LogWindow> logWindow;
	std::shared_ptr<InfoWindow> infoWindow;

	//Half Screen Windows
	std::shared_ptr<MapWindow> worldWindow;
	std::shared_ptr<InventoryFullWindow> inventoryFullWindow;
	std::shared_ptr<CommandWindow> commandWindow;

	//Full-screen Windows
	std::shared_ptr<PauseWindow> pauseWindow;
	std::shared_ptr<SplashWindow> startupSplashWindow;

	Gui(int windowX, int windowY);

	void renderMouse() const;

	void update();
	void render() const;

private:
	std::vector<std::shared_ptr<Window>> windowList;
};
