#include "main.hpp"

Font::Font(const char* filePath, int format)
	:filePath(filePath), format(format), charW(0), charH(0)
{
}

Font::Font(const char* filePath, int format, int charW, int charH)
	: filePath(filePath), format(format), charW(charW), charH(charH)
{
}

void Font::setfont(std::shared_ptr<Font> font)
{
	if (font->charH == 0 && font->charW == 0)
	{
		TCODConsole::setCustomFont(font->filePath, font->format);
	}
	else
	{
		TCODConsole::setCustomFont(font->filePath, font->format, font->charW, font->charH);
	}
}

Input::Input()
	:keyboard(TCODConsole::checkForKeypress(TCOD_KEY_PRESSED))
{
}

void Input::getKeyInput(std::shared_ptr<Entity> entity)
{
	//check for mortality

	if (engine.gamestate == Engine::MAIN)
	{
		int ix = 0;
		int iy = 0;

		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &keyboard, NULL);

		switch (keyboard.c)
		{
		case 'w':
			iy++;
			break;
		case 's':
			iy--;
			break;
		case 'a':
			ix--;
			break;
		case 'd':
			ix++;
			break;
		default:
			break;
		}
		switch (keyboard.vk)
		{
		case TCODK_UP:
			iy++;
			break;
		case TCODK_DOWN:
			iy--;
			break;
		case TCODK_LEFT:
			ix--;
			break;
		case TCODK_RIGHT:
			ix++;
			break;
		default:
			break;
		}

		if (ix != 0 || iy != 0)
		{
			entity->position.x += ix;
			entity->position.y += iy;
			std::cout << entity->position.x << " : " << entity->position.y << std::endl;
		}
	}
}

Settings::Settings(int w, int h)
	: windowX(w), windowY(h), windowTitle("Epsilon v. Alpha"), fullscreen(false), maxFps(60), renderer(TCOD_RENDERER_OPENGL2)
{
	terminalfont = std::make_shared<Font>("data/fonts/terminal16x16_gs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GRAYSCALE);
	terminalfont->setfont(terminalfont);

	input = std::make_shared<Input>();

	TCODConsole::initRoot(windowX, windowY, windowTitle, fullscreen, renderer);
	TCODConsole::root->setDefaultBackground(TCODColor::black);
	TCODSystem::setFps(maxFps);
}

void Settings::printLogo()
{
	TCODConsole::root->printf(1, 0, "Epsilon");
}

void Settings::update(std::shared_ptr<Entity> entity)
{
	input->getKeyInput(entity);
}