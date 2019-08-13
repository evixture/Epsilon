#include "main.hpp"

class Gui
{
public:

	int windowx;
	int windowy;
	const char* windowTitle;
	bool windowFullscreen;
	int maxFps;

	Gui(int windowx, int windowy);
	~Gui();
};

class Message
{
	const char* message;
};

//TODO : set up gui
//TODO : Effects and weapons