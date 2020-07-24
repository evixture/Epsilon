#include "main.hpp"

Button::Button(const std::string name)
	: isDown(false), isSwitched(false), buttonSwitch(false), keyName(name)
{
}

void Button::clear()
{
	isDown = false;
	isSwitched = false;
}

//----------------------------------------------------------------------------------------------------

KeyboardButton::KeyboardButton(sf::Keyboard::Key key, const std::string name)
	: Button(name), button(key)
{
}

void KeyboardButton::update()
{
	isDown = sf::Keyboard::isKeyPressed(button);
	
	if (sf::Keyboard::isKeyPressed(button))
	{
		if (!buttonSwitch)
		{
			isSwitched = true;
			buttonSwitch = true;
		}
		else if (buttonSwitch)
		{
			isSwitched = false;
		}
	}
	else
	{
		isSwitched = false;
		buttonSwitch = false;
	}
}

//----------------------------------------------------------------------------------------------------

MouseButton::MouseButton(sf::Mouse::Button button, const std::string name)
	: Button(name), button(button)
{
}

void MouseButton::update()
{
	isDown = sf::Mouse::isButtonPressed(button);

	if (sf::Mouse::isButtonPressed(button))
	{
		if (!buttonSwitch)
		{
			isSwitched = true;
			buttonSwitch = true;
		}
		else if (buttonSwitch)
		{
			isSwitched = false;
		}
	}
	else
	{
		isSwitched = false;
		buttonSwitch = false;
	}
}

//----------------------------------------------------------------------------------------------------

Mouse::Mouse()
	:screenPosition(Position2(0, 0)), mapPosition(Position2(0, 0))
{
	leftMB = std::make_shared<MouseButton>(sf::Mouse::Left, "LeftMB");
	rightMB = std::make_shared<MouseButton>(sf::Mouse::Right, "RightMB");
	MouseWheel = std::make_shared<MouseButton>(sf::Mouse::Middle, "MouseWheel");
}

void Mouse::update(TCOD_mouse_t TCODmouse)
{
	if (TCODConsole::hasMouseFocus()) TCODMouse::showCursor(false); //change later to use setting option

	leftMB	  ->update();
	rightMB	  ->update();
	MouseWheel->update();

	screenPosition = Position2(TCODmouse.cx, TCODmouse.cy);
	mapPosition = Position2(screenPosition.x + WORLD->xOffset - 1, screenPosition.y + WORLD->yOffset - 3);
}

//----------------------------------------------------------------------------------------------------

Keyboard::Keyboard()
{
	keyboard.push_back(escape			= std::make_shared<KeyboardButton>(sf::Keyboard::Escape		, "Escape"));
	keyboard.push_back(f1				= std::make_shared<KeyboardButton>(sf::Keyboard::F1			, "F1"));
	keyboard.push_back(f2				= std::make_shared<KeyboardButton>(sf::Keyboard::F2			, "F2"));
	keyboard.push_back(f3				= std::make_shared<KeyboardButton>(sf::Keyboard::F3			, "F3"));
	keyboard.push_back(f4				= std::make_shared<KeyboardButton>(sf::Keyboard::F4			, "F4"));
	keyboard.push_back(f5				= std::make_shared<KeyboardButton>(sf::Keyboard::F5			, "F5"));
	keyboard.push_back(f6				= std::make_shared<KeyboardButton>(sf::Keyboard::F6			, "F6"));
	keyboard.push_back(f7				= std::make_shared<KeyboardButton>(sf::Keyboard::F7			, "F7"));
	keyboard.push_back(f8				= std::make_shared<KeyboardButton>(sf::Keyboard::F8			, "F8"));
	keyboard.push_back(f9				= std::make_shared<KeyboardButton>(sf::Keyboard::F9			, "F9"));
	keyboard.push_back(f10				= std::make_shared<KeyboardButton>(sf::Keyboard::F10		, "F10"));
	keyboard.push_back(f11				= std::make_shared<KeyboardButton>(sf::Keyboard::F11		, "F11"));
	keyboard.push_back(f12				= std::make_shared<KeyboardButton>(sf::Keyboard::F12		, "F12"));
	keyboard.push_back(pause			= std::make_shared<KeyboardButton>(sf::Keyboard::Pause		, "Pause"));
																		   							   			
	keyboard.push_back(tilde			= std::make_shared<KeyboardButton>(sf::Keyboard::Tilde		, "~"));
	keyboard.push_back(k1				= std::make_shared<KeyboardButton>(sf::Keyboard::Num1		, "1"));
	keyboard.push_back(k2				= std::make_shared<KeyboardButton>(sf::Keyboard::Num2		, "2"));
	keyboard.push_back(k3				= std::make_shared<KeyboardButton>(sf::Keyboard::Num3		, "3"));
	keyboard.push_back(k4				= std::make_shared<KeyboardButton>(sf::Keyboard::Num4		, "4"));
	keyboard.push_back(k5				= std::make_shared<KeyboardButton>(sf::Keyboard::Num5		, "5"));
	keyboard.push_back(k6				= std::make_shared<KeyboardButton>(sf::Keyboard::Num6		, "6"));
	keyboard.push_back(k7				= std::make_shared<KeyboardButton>(sf::Keyboard::Num7		, "7"));
	keyboard.push_back(k8				= std::make_shared<KeyboardButton>(sf::Keyboard::Num8		, "8"));
	keyboard.push_back(k9				= std::make_shared<KeyboardButton>(sf::Keyboard::Num9		, "9"));
	keyboard.push_back(k0				= std::make_shared<KeyboardButton>(sf::Keyboard::Num0		, "0"));
	keyboard.push_back(minus			= std::make_shared<KeyboardButton>(sf::Keyboard::Hyphen		, "-"));
	keyboard.push_back(equals			= std::make_shared<KeyboardButton>(sf::Keyboard::Equal		, "="));
	keyboard.push_back(backspace		= std::make_shared<KeyboardButton>(sf::Keyboard::BackSpace	, "BackSpace"));
	keyboard.push_back(insert			= std::make_shared<KeyboardButton>(sf::Keyboard::Insert		, "Insert"));
	keyboard.push_back(home				= std::make_shared<KeyboardButton>(sf::Keyboard::Home		, "Home"));
	keyboard.push_back(pageUp			= std::make_shared<KeyboardButton>(sf::Keyboard::PageUp		, "PageUp"));
	keyboard.push_back(numLock			= std::make_shared<KeyboardButton>(sf::Keyboard::PageDown	, "PageDown"));
	keyboard.push_back(kpDivide			= std::make_shared<KeyboardButton>(sf::Keyboard::Divide		, "k/"));
	keyboard.push_back(kpMultiply		= std::make_shared<KeyboardButton>(sf::Keyboard::Multiply	, "k*"));
	keyboard.push_back(kpMinus			= std::make_shared<KeyboardButton>(sf::Keyboard::Dash		, "k-"));
																		   							   			
	keyboard.push_back(tab				= std::make_shared<KeyboardButton>(sf::Keyboard::Tab		, "Tab"));
	keyboard.push_back(q				= std::make_shared<KeyboardButton>(sf::Keyboard::Q			, "Q"));
	keyboard.push_back(w				= std::make_shared<KeyboardButton>(sf::Keyboard::W			, "W"));
	keyboard.push_back(e				= std::make_shared<KeyboardButton>(sf::Keyboard::E			, "E"));
	keyboard.push_back(r				= std::make_shared<KeyboardButton>(sf::Keyboard::R			, "R"));
	keyboard.push_back(t				= std::make_shared<KeyboardButton>(sf::Keyboard::T			, "T"));
	keyboard.push_back(y				= std::make_shared<KeyboardButton>(sf::Keyboard::Y			, "Y"));
	keyboard.push_back(u				= std::make_shared<KeyboardButton>(sf::Keyboard::U			, "U"));
	keyboard.push_back(i				= std::make_shared<KeyboardButton>(sf::Keyboard::I			, "I"));
	keyboard.push_back(o				= std::make_shared<KeyboardButton>(sf::Keyboard::O			, "O"));
	keyboard.push_back(p				= std::make_shared<KeyboardButton>(sf::Keyboard::P			, "P"));
	keyboard.push_back(leftBrace		= std::make_shared<KeyboardButton>(sf::Keyboard::LBracket	, "["));
	keyboard.push_back(rightBrace		= std::make_shared<KeyboardButton>(sf::Keyboard::RBracket	, "]"));
	keyboard.push_back(backSlash		= std::make_shared<KeyboardButton>(sf::Keyboard::BackSlash	, "\\"));
	keyboard.push_back(del				= std::make_shared<KeyboardButton>(sf::Keyboard::Delete		, "Delete"));
	keyboard.push_back(end				= std::make_shared<KeyboardButton>(sf::Keyboard::End		, "End"));
	keyboard.push_back(pageDown			= std::make_shared<KeyboardButton>(sf::Keyboard::PageDown	, "PageDown"));
	keyboard.push_back(kp7				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad7	, "k7"));
	keyboard.push_back(kp8				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad8	, "k8"));
	keyboard.push_back(kp9				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad9	, "k9"));
	keyboard.push_back(kpAdd			= std::make_shared<KeyboardButton>(sf::Keyboard::Add		, "k+"));
																		   							   			
	keyboard.push_back(a				= std::make_shared<KeyboardButton>(sf::Keyboard::A			, "A"));
	keyboard.push_back(s				= std::make_shared<KeyboardButton>(sf::Keyboard::S			, "S"));
	keyboard.push_back(d				= std::make_shared<KeyboardButton>(sf::Keyboard::D			, "D"));
	keyboard.push_back(f				= std::make_shared<KeyboardButton>(sf::Keyboard::F			, "F"));
	keyboard.push_back(g				= std::make_shared<KeyboardButton>(sf::Keyboard::G			, "G"));
	keyboard.push_back(h				= std::make_shared<KeyboardButton>(sf::Keyboard::H			, "H"));
	keyboard.push_back(j				= std::make_shared<KeyboardButton>(sf::Keyboard::J			, "J"));
	keyboard.push_back(k				= std::make_shared<KeyboardButton>(sf::Keyboard::K			, "K"));
	keyboard.push_back(l				= std::make_shared<KeyboardButton>(sf::Keyboard::L			, "L"));
	keyboard.push_back(semiColon		= std::make_shared<KeyboardButton>(sf::Keyboard::SemiColon	, ";"));
	keyboard.push_back(quote			= std::make_shared<KeyboardButton>(sf::Keyboard::Quote		, "'"));
	keyboard.push_back(enter			= std::make_shared<KeyboardButton>(sf::Keyboard::Enter		, "Enter"));
	keyboard.push_back(kp4				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad4	, "k4"));
	keyboard.push_back(kp5				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad5	, "k5"));
	keyboard.push_back(kp6				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad6	, "k6"));
																		   							   			
	keyboard.push_back(leftShift		= std::make_shared<KeyboardButton>(sf::Keyboard::LShift		, "LShift"));
	keyboard.push_back(z				= std::make_shared<KeyboardButton>(sf::Keyboard::Z			, "Z"));
	keyboard.push_back(x				= std::make_shared<KeyboardButton>(sf::Keyboard::X			, "X"));
	keyboard.push_back(c				= std::make_shared<KeyboardButton>(sf::Keyboard::C			, "C"));
	keyboard.push_back(v				= std::make_shared<KeyboardButton>(sf::Keyboard::V			, "V"));
	keyboard.push_back(b				= std::make_shared<KeyboardButton>(sf::Keyboard::B			, "B"));
	keyboard.push_back(n				= std::make_shared<KeyboardButton>(sf::Keyboard::N			, "N"));
	keyboard.push_back(m				= std::make_shared<KeyboardButton>(sf::Keyboard::M			, "M"));
	keyboard.push_back(comma			= std::make_shared<KeyboardButton>(sf::Keyboard::Comma		, ","));
	keyboard.push_back(period			= std::make_shared<KeyboardButton>(sf::Keyboard::Period		, "."));
	keyboard.push_back(slash			= std::make_shared<KeyboardButton>(sf::Keyboard::Slash		, "/"));
	keyboard.push_back(rightShift		= std::make_shared<KeyboardButton>(sf::Keyboard::RShift		, "RShift"));
	keyboard.push_back(kpUp				= std::make_shared<KeyboardButton>(sf::Keyboard::Up			, "Up"));
	keyboard.push_back(kp1				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad1	, "k1"));
	keyboard.push_back(kp2				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad2	, "k2"));
	keyboard.push_back(kp3				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad3	, "k3"));
																		   							   			
	keyboard.push_back(leftControl		= std::make_shared<KeyboardButton>(sf::Keyboard::LControl	, "LControl"));
	keyboard.push_back(leftAlt			= std::make_shared<KeyboardButton>(sf::Keyboard::LAlt		, "LAlt"));
	keyboard.push_back(space			= std::make_shared<KeyboardButton>(sf::Keyboard::Space		, "Space"));
	keyboard.push_back(rightAlt			= std::make_shared<KeyboardButton>(sf::Keyboard::RAlt		, "RAlt"));
	keyboard.push_back(rightControl		= std::make_shared<KeyboardButton>(sf::Keyboard::LControl	, "LControl"));
	keyboard.push_back(kpLeft			= std::make_shared<KeyboardButton>(sf::Keyboard::Left		, "Left"));
	keyboard.push_back(kpDown			= std::make_shared<KeyboardButton>(sf::Keyboard::Down		, "Down"));
	keyboard.push_back(kpRight			= std::make_shared<KeyboardButton>(sf::Keyboard::Right		, "Right"));
	keyboard.push_back(kp0				= std::make_shared<KeyboardButton>(sf::Keyboard::Numpad0	, "k0"));
}

std::vector<std::shared_ptr<KeyboardButton>> Keyboard::getButtonsDown()
{
	std::vector<std::shared_ptr<KeyboardButton>> ret;

	for (int i = 0; i < keyboard.size(); i++)
	{
		if (keyboard[i]->isDown)
		{
			ret.push_back(keyboard[i]);
		}
	}

	return ret;
}

std::vector<std::shared_ptr<KeyboardButton>> Keyboard::getButtonsSwitched()
{
	std::vector<std::shared_ptr<KeyboardButton>> ret;

	for (int i = 0; i < keyboard.size(); i++)
	{
		if (keyboard[i]->isSwitched)
		{
			ret.push_back(keyboard[i]);
		}
	}

	return ret;
}

std::string Keyboard::getButtonsSwitchedText()
{
	std::vector<std::shared_ptr<KeyboardButton>> buttons = getButtonsSwitched();
	if (buttons.size() > 0)
	{
		if (buttons[0]->keyName.size() == 1)
		{
			return buttons[0]->keyName; //normal letter/symbol
		}
		else if (buttons[0]->keyName == "Space")
		{
			return " "; //space
		}
	}
	return "";
}

void Keyboard::update(TCOD_mouse_t mouse)
{
	for (auto& button : keyboard)
	{
		if		(TCODConsole::hasMouseFocus()) button->update();
		else	button->isDown = false;
	}
}

Bind::Bind(std::shared_ptr<Button> bind, const std::string name)
	:bind(bind), name(name)
{
}

std::shared_ptr<Button> Bind::getButton()
{
	return bind;
}

void Bind::clear()
{
	bind->clear();

	isDown = false;
	isSwitched = false;
}

void Bind::rebind(std::shared_ptr<KeyboardButton> kbButton)
{
	bind = kbButton;
}

void Bind::update(bool disabled)
{
	//bind->update(); binds are already updated in keyboard

	if (!disabled)
	{
		isDown = bind->isDown;
		isSwitched = bind->isSwitched;
	}
	else
	{
		isDown = false;
		isSwitched = false;
	}
}

Input::Input()
	:keyboard(std::make_shared<Keyboard>()), TCODmouse(), mouse(std::make_shared<Mouse>()), bindsDisabled(false)
{
	//bindList = std::make_shared<std::vector<std::shared_ptr<Bind>>>();

	bindList.push_back(moveUp			= std::make_shared<Bind>(keyboard->w			, "Move Up"		));
	bindList.push_back(moveDown			= std::make_shared<Bind>(keyboard->s			, "Move Down"	));
	bindList.push_back(moveLeft			= std::make_shared<Bind>(keyboard->a			, "Move Left"	));
	bindList.push_back(moveRight		= std::make_shared<Bind>(keyboard->d			, "Move Right"	));
									   							 								
	bindList.push_back(moveSlow			= std::make_shared<Bind>(keyboard->leftControl	, "Move Slow"	));
	bindList.push_back(moveFast			= std::make_shared<Bind>(keyboard->leftShift	, "Move Fast"	));
									   							 									
	bindList.push_back(stanceDown		= std::make_shared<Bind>(keyboard->z			, "Stance Down"	));
	bindList.push_back(stanceUp			= std::make_shared<Bind>(keyboard->x			, "Stance Up"	));
									  							 									
	bindList.push_back(pickUp			= std::make_shared<Bind>(keyboard->e			, "Pick Up"		));
	bindList.push_back(drop				= std::make_shared<Bind>(keyboard->q			, "Drop"		));
									   							 									
	bindList.push_back(reload			= std::make_shared<Bind>(keyboard->r			, "Reload"		));
									   							 									
	bindList.push_back(deepInteract		= std::make_shared<Bind>(keyboard->leftAlt		, "Deep Interact"));
	bindList.push_back(worldInteract	= std::make_shared<Bind>(keyboard->space		, "World Interact"));
									   						 									
	bindList.push_back(inventory		= std::make_shared<Bind>(keyboard->i			, "Inventory"	));
	bindList.push_back(fullscreen		= std::make_shared<Bind>(keyboard->f11			, "Fullscreen"	));
	bindList.push_back(info				= std::make_shared<Bind>(keyboard->n			, "Info"		));
	bindList.push_back(menu				= std::make_shared<Bind>(keyboard->escape		, "Menu"		));
									   							 								
	bindList.push_back(highlight		= std::make_shared<Bind>(keyboard->h			, "Highlight"	));
									  							 									
	bindList.push_back(debug1			= std::make_shared<Bind>(keyboard->k1			, "debug1"		));
	bindList.push_back(debug2			= std::make_shared<Bind>(keyboard->k2			, "debug2"		));
	bindList.push_back(debug3			= std::make_shared<Bind>(keyboard->k3			, "debug3"		));
	bindList.push_back(console			= std::make_shared<Bind>(keyboard->tilde		, "Toggle Console"));
									   							 									
	bindList.push_back(primaryUse		= std::make_shared<Bind>(mouse->leftMB			, "Primary Use"	));
	bindList.push_back(alternateUse		= std::make_shared<Bind>(mouse->rightMB			, "Alternate Use"));
}

//std::vector<std::shared_ptr<Bind>> Input::getBindList()
//{
//	return bindList;
//}

void Input::update()
{
	keyEvent = TCODSystem::checkForEvent(TCOD_EVENT_ANY, NULL, &TCODmouse);

	for (auto& bind : bindList)
	{
		if (bind != console)
		{
			bind->update(bindsDisabled);
		}
		else
		{
			bind->update(false); //dont block console command input
		}
	}

	keyboard->update(TCODmouse);
	mouse->update(TCODmouse);
}
