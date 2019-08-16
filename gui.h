#include "main.hpp"

class Gui
{
	std::vector<std::shared_ptr<GuiWindow>> windowList;

	Gui(int windowX, int windowY);

	void render();
};
