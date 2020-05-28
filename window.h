#include "main.hpp"

/*
	Window
	|	Pane
	|	Ribbon
*/

struct Pane //TCOD console with width, height, and colors
{
	const unsigned short int consoleWidth; //width of the console in tiles
	const unsigned short int consoleHeight; //height of console in tiles
	TCODConsole* console; //TCOD console from Libtcod

	TCODColor backgroundColor; //background color of console
	TCODColor foregroundColor; //foreground color of console

	Pane(int consoleWidth, int consoleHeight, TCODColor backgroundColor, TCODColor foregroundColor); //constructor that takes width, height, background color, and foreground color
	~Pane(); //destructor

	void render() const; //renders pane
};

struct Ribbon //top ribbon for windows that displays the window name
{
	const unsigned short int consoleWidth; //width of the ribbon in tiles
	Pane ribonWindow; //ribbon pane

	Ribbon(std::string windowName, int consoleWidth); //constructor that takes string name and console width

	void render() const; //renders ribbon

private:
	std::string windowName; //string name of ribbon to display, should be accessed through window
};

struct Window //group of panes and consoles that is the base for gui panels
{
	const unsigned short int consoleWidth; //width of the window in tiles
	const unsigned short int consoleHeight; //height of the window in tiles
	Position3 renderpos; //tile coordinates to render window on root console

	Pane drawPane; //window that is availible to render to by other classes

	Window(int consoleWidth, int consoleHeight, std::string panelName, int rx, int ry); //constructor that takes width, height, string name, and render coords

	virtual void update() = 0; //virtual updates window
	virtual void render() const = 0; //virtual renders window

	void clearWindow() const; //clears window so that tiles can be drawn
	void pushWindow() const; //pushes all tiles to window so it can be propperly rendered

private:
	Ribbon ribbon; //ribbon component to window
	Pane mainPane; //main window that the ribbon and draw window are blitted to
};
