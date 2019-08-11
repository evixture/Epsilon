#include "main.hpp"

class Window
{
public:

	int windowx;
	int windowy;

	const char* windowTitle;
	bool windowFullscreen;
	int maxFps;

	TCODConsole* guiWin;

	Window(int windowx, int windowy);
	~Window();

	//creates initial canvas
	void createCanvas();
};

class Message
{
	const char* message;
};

class Gui
{
};
//TODO : set up gui
//TODO : Effects and weapons