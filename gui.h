#include "main.hpp"

//Gui Class
class Gui
{
public:
	
	//Main Windows
	std::shared_ptr<Window> playerWindow;
	std::shared_ptr<Window> statusWindow;
	std::shared_ptr<Window> inventoryWindow;
	std::shared_ptr<Window> proximityWindow;
	std::shared_ptr<Window> actionsWindow;
	std::shared_ptr<Window> eventLogWindow;
	std::shared_ptr<MapPane> mapWindow;

	Gui(int windowX, int windowY);

	void update();
	void render();

private:

	std::vector<std::shared_ptr<Window>> windowList;

};
