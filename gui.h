#include "main.hpp"

//Gui Class
class Gui
{
public:
	enum ActiveWindow { NONE, STARTUPSPLASH, INVENTORYFULL} activeWindow;

	//Part-screen Windows
	std::shared_ptr<PlayerPane> playerPane;
	std::shared_ptr<StatusPane> statusPane;
	std::shared_ptr<InventoryPane> inventoryPane;
	std::shared_ptr<LogPane> eventLogPane;
	std::shared_ptr<Window> proximityPane;
	std::shared_ptr<Window> actionsPane;
	std::shared_ptr<MapPane> mapPane;

	//Full-screen Windows
	std::shared_ptr<SplashPane> startupSplash;

	//Half Screen Windows
	std::shared_ptr<InventoryFullPane> inventoryFullPane;

	Gui(int windowX, int windowY);

	void update();
	void render() const;

private:
	std::vector<std::shared_ptr<Window>> windowList;
};
