#include "main.hpp"

struct Key
{
	sf::Keyboard::Key sfKey;
	bool isDown;
	bool isPressed;
	bool isSwitched;

	Key(sf::Keyboard::Key sfKey);

	void update();

private:
	bool keySwitch;
};

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

	void getKeyDown();
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
};
