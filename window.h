#include "main.hpp"

//Message Struct
struct Message
{
	const char* text;
	TCODColor color;

	Message(const char* text, TCODColor color);
};

//Window Struct
struct Window
{
	int consoleW;
	int consoleH;
	TCODConsole* console;

	TCODColor bgColor;
	TCODColor fgColor;

	Window(int consoleW, int consoleH, TCODColor bgColor, TCODColor fgColor);
	~Window();

	void render();
};

//Ribon Struct
struct Ribon
{
	const char* windowName;
	int windowW;

	std::shared_ptr<Window> ribonWindow;

	Ribon(const char* windowName, int windowW);

	void render();
};

//GuiWindow Struct
struct GuiWindow
{
	int w;
	int h;
	const char* guiWindowName;
	Position renderpos;

	std::shared_ptr<Ribon> ribon;
	std::shared_ptr<Window> mainWindow;
	std::shared_ptr<Window> drawWindow;

	GuiWindow(int w, int h, const char* guiWindowName, int rx, int ry);

	void render();
};

//GuiMap Struct
struct GuiMap
{
	std::shared_ptr<Map> map;
	std::shared_ptr<TCODMap> tcodMap;
	std::shared_ptr<GuiWindow> mapWindow;

	GuiMap(int w, int h, int rx, int ry);

	void render();
};
