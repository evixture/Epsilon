#include "main.hpp"

//Gui Class
class Gui
{
public:

	//Main Windows
	std::shared_ptr<PlayerPane> playerPane;
	std::shared_ptr<StatusPane> statusPane;
	std::shared_ptr<InventoryPane> inventoryPane;
	std::shared_ptr<Window> proximityPane;
	std::shared_ptr<Window> actionsPane;
	std::shared_ptr<Window> eventLogPane;
	std::shared_ptr<MapPane> mapPane;

	std::shared_ptr<SplashPanel> startupSplash;

	Gui(int windowX, int windowY);

	void update();
	void render() const;

private:
	std::vector<std::shared_ptr<Window>> windowList;
};
