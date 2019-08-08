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

	//creates initial canvas
	void createCanvas();

	void update();

	void render();
};

//TODO : set up gui
//TODO : Effects and weapons