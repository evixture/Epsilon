#include "main.hpp"

//Gui Class
class Gui
{
public:
	
	//Main Windows
	std::shared_ptr<Panel> playerWindow;
	std::shared_ptr<Panel> statusWindow;
	std::shared_ptr<Panel> inventoryWindow;
	std::shared_ptr<Panel> proximityWindow;
	std::shared_ptr<Panel> actionsWindow;
	std::shared_ptr<Panel> eventLogWindow;
	std::shared_ptr<GuiMap> mapWindow;

	Gui(int windowX, int windowY);

	void update();
	void render();

private:

	std::vector<std::shared_ptr<Panel>> windowList;

};
