#include "main.hpp"

struct Window
{
	int consoleW;
	int consoleH;
	TCODConsole* console;

	Window(int consoleW, int consoleH);
	~Window();
};

struct Ribon
{
	const char* windowName;
	int windowW;

	Window ribonWindow;

	Ribon(const char* windowName, int windowW);

	void render(std::shared_ptr<Window> window);
};

struct GuiWindow
{
	int w;
	int h;
	const char* guiWindowName;

	Window mainWindow;
	Ribon ribon;
	Window drawWindow;

	GuiWindow(int w, int h, const char* guiWindowName);
};