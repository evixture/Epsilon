#include "main.hpp"

struct Gui
{
	enum class ActiveWindow { NONE, STARTUPSPLASH, INVENTORYFULL, PAUSE} activeWindow;
	enum class ActiveLogWindow { LOG, INFO } activeLogWindow;

	//Part-screen Windows
	std::shared_ptr<PlayerWindow> playerWindow;
	std::shared_ptr<StatusWindow> statusWindow;
	std::shared_ptr<InventoryWindow> inventoryWindow;
	std::shared_ptr<LogWindow> logWindow;
	std::shared_ptr<ProximityWindow> proximityWindow;
	std::shared_ptr<ActionWindow> actionWindow;
	std::shared_ptr<InfoWindow> infoWindow;

	//Half Screen Windows
	std::shared_ptr<MapWindow> worldWindow;
	std::shared_ptr<InventoryFullWindow> inventoryFullWindow;

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
