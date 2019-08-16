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

	std::shared_ptr<Window> ribonWindow;

	Ribon(const char* windowName, int windowW);

	void render(std::shared_ptr<Window> window);
};

struct Message
{
	const char* text;
	TCODColor color;

	Message(const char* text, TCODColor color);
};

struct GuiWindow
{
	int w;
	int h;
	const char* guiWindowName;
	Position renderpos;

	std::shared_ptr<Window> mainWindow;
	std::shared_ptr<Ribon> ribon;
	std::shared_ptr<Window> drawWindow;

	GuiWindow(int w, int h, const char* guiWindowName, int rx, int ry);

	void render(std::shared_ptr<Window> window);
};