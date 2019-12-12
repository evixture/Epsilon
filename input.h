#include "main.hpp"

struct Key
{
	sf::Keyboard::Key sfKey;
	bool isDown;
	//bool isPressed;
	bool isSwitched;

	Key(sf::Keyboard::Key sfKey);
	Key();

	void update();

private:
	bool keySwitch;
};

struct MouseButton
{
	sf::Mouse::Button sfMButton;
	bool isDown;
	bool isSwitched;

	MouseButton(sf::Mouse::Button MButton);
	MouseButton();

	void update();

private:
	bool buttonSwitch;
};

//Input Struct
struct Input
{
	TCOD_event_t keyEvent;
	TCOD_key_t keyboard;
	TCOD_mouse_t mouse;

	std::shared_ptr<Key> w;
	std::shared_ptr<Key> a;
	std::shared_ptr<Key> s;
	std::shared_ptr<Key> d;

	std::shared_ptr<Key> lctrl;
	std::shared_ptr<Key> lshift;

	std::shared_ptr<Key> z;
	std::shared_ptr<Key> x;
	std::shared_ptr<Key> c;

	std::shared_ptr<Key> e;
	std::shared_ptr<Key> q;
	std::shared_ptr<Key> r;

	std::shared_ptr<Key> i;

	std::shared_ptr<Key> num0;
	std::shared_ptr<Key> num9;

	std::shared_ptr<Key> space;
	std::shared_ptr<Key> f11;
	std::shared_ptr<Key> escape;
	std::vector<std::shared_ptr<Key>> keyList;

	std::shared_ptr<MouseButton> leftMouseButton;
	std::vector<std::shared_ptr<MouseButton>> mouseList;

	float baseMoveWait;

	//bool leftMouseClick;

	Input();

	//TODO : Move key checks to funcs with buttton params for config support

	void update(std::shared_ptr<Player> player);

	void getKeyDown();
	void getKeyInput(std::shared_ptr<Player> player);

	void getMouseInput();

private:
	Clock movementClock;
	
	int moveXSpeed;
	int moveYSpeed;
};
