#include "main.hpp"

//Gui Class
class Gui
{
public:
	std::vector<std::shared_ptr<GuiWindow>> windowList;

	//Main Windows
	std::shared_ptr<GuiWindow> playerWindow;
	std::shared_ptr<GuiWindow> statusWindow;
	std::shared_ptr<GuiWindow> inventoryWindow;
	std::shared_ptr<GuiWindow> proximityWindow;
	std::shared_ptr<GuiWindow> actionsWindow;
	std::shared_ptr<GuiWindow> eventLogWindow;
	std::shared_ptr<GuiMap> mapWindow;

	Gui(int windowX, int windowY);

	void render();
};
