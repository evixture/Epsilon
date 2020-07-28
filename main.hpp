#pragma once

//STD
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
#include <utility>

//Libraries
#include <libtcod.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <E:/pugixml-1.10/src/pugixml.hpp>
#include <soloud.h>

#include <soloud_thread.h> //need later??
#include <soloud_speech.h>
#include <soloud_wav.h>
#include <soloud_audiosource.h>

struct Pane;
struct Creature;
struct Player;

//Class File Imports
#include "utility.h"

#include "audio.h"

#include "environment.h"

	#include "tile.h"
	#include "tool.h"
#include "item.h"

	#include "creature.h"
	#include "aicreature.h"
#include "map.h"

	#include "input.h"
#include "settings.h"

	#include "window.h"
	#include "panel.h"
#include "gui.h"

#include "engine.h"