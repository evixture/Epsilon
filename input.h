#include "main.hpp"

//Input Struct
struct Input
{
	TCOD_event_t keyEvent;
	TCOD_key_t keyboard;
	TCOD_mouse_t mouse;

	float baseMoveWait;

	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool moveRight;

	bool reload;
	bool changeFloor;
	bool leftMouseClick;

	Input();

	//TODO : Move key checks to funcs with buttton params for config support

	void update(std::shared_ptr<Player> player);

	bool getKeyDown(sf::Keyboard::Key key);
	bool getKeyToggle(sf::Keyboard::Key key);
	bool getKeyPressed(sf::Keyboard::Key key);

	void getKeyActivity();
	void getKeyInput(std::shared_ptr<Player> player);

	void getMouseInput();

private:
	int moveXSpeed;
	int moveYSpeed;

	int moveTimer;
	int moveWait;

	bool reloadToggle;
	bool changeFloorToggle;
	bool f11Toggle;
	bool toggle;
};
