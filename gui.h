#include "main.hpp"

//Gui Class
class Gui
{
public:

	//Main Windows
	std::shared_ptr<Window> playerPane;
	std::shared_ptr<Window> statusPane;
	std::shared_ptr<Window> inventoryPane;
	std::shared_ptr<Window> proximityPane;
	std::shared_ptr<Window> actionsPane;
	std::shared_ptr<Window> eventLogPane;
	std::shared_ptr<MapPane> mapPane;

	Gui(int windowX, int windowY);

	void update();
	void render();

private:

	std::vector<std::shared_ptr<Window>> windowList;
};
