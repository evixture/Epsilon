#include "main.hpp"

//Window Struct
struct Pane
{
	int consoleW;
	int consoleH;
	TCODConsole* console;

	TCODColor bgColor;
	TCODColor fgColor;

	Pane(int consoleW, int consoleH, TCODColor bgColor, TCODColor fgColor);
	~Pane();

	void render() const;
};

//Ribon Struct
struct Ribon
{
	const char* windowName;
	int windowW;

	std::shared_ptr<Pane> ribonWindow;

	Ribon(const char* windowName, int windowW);

	void updateRibonName(const char* ribonName);

	void render() const;
};

//GuiWindow Struct
struct Window
{
public:

	int w;
	int h;
	const char* panelName;
	Position renderpos;

	std::shared_ptr<Pane> drawWindow;

	Window(int w, int h, const char* panelName, int rx, int ry);

	virtual void update();
	virtual void render() const;

	void setRibonName(const char* ribonName);
	void clearWindow() const;
	void pushWindow() const;

private:
	std::shared_ptr<Ribon> ribon;
	std::shared_ptr<Pane> mainWindow;
};

//GuiMap Struct
