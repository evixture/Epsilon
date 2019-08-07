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

	void createCanvas();
};

//TODO : set up gui
//TODO : Effects and weapons