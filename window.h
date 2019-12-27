#include "main.hpp"

struct Pane //TCOD console with width, height, and colors
{
	int consoleWidth; //width of the console in tiles
	int consoleHeight; //height of console in tiles
	TCODConsole* console; //TCOD console from Libtcod

	TCODColor backgroundColor; //background color of console
	TCODColor foregroundColor; //foreground color of console

	Pane(int consoleWidth, int consoleHeight, TCODColor backgroundColor, TCODColor foregroundColor); //constructor that takes width, height, background color, and foreground color
	~Pane(); //destructor

	void render() const; //renders pane
};

struct Ribon //top ribon for windows that displays the window name
{
	int consoleWidth; //width of the ribon in tiles

	std::shared_ptr<Pane> ribonWindow; //ribon pane

	Ribon(std::string windowName, int consoleWidth); //constructor that takes string name and console width

	void render() const; //renders ribon

private:
	std::string windowName; //string name of ribon to display, should be accessed through window
};

struct Window //group of panes and consoles that is the base for gui panels
{
	int consoleWidth; //width of the window in tiles
	int consoleHeight; //height of the window in tiles
	std::string panelName; //string name of the window for use with ribon
	Position renderpos; //tile coordinates to render window on root console

	std::shared_ptr<Pane> drawWindow; //window that is availible to render to by other classes

	Window(int consoleWidth, int consoleHeight, std::string panelName, int rx, int ry); //constructor that takes width, height, string name, and render coords

	virtual void update(); //virtual updates window
	virtual void render() const; //virtual renders window

	void clearWindow() const; //clears window so that tiles can be drawn
	void pushWindow() const; //pushes all tiles to window so it can be propperly rendered

private:
	std::shared_ptr<Ribon> ribon; //ribon component to window
	std::shared_ptr<Pane> mainWindow; //main window that the ribon and draw window are blitted to
};
