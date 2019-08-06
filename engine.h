#pragma once

//for main tcod functions and call tree
class Engine
{
public:

	int windowx;
	int windowy;

	Engine(int winx, int winy);

	~Engine();

	//start of update functions
	void update();

	//start of render functions
	void render();

	void createCanvas(int x, int y);

private:

};

extern Engine engine;