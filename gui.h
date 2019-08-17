#include "main.hpp"

class Gui
{
public:
	std::vector<std::shared_ptr<GuiWindow>> windowList;

	std::shared_ptr<GuiMap> guiMap;

	Gui(int windowX, int windowY);

	void render();
};
