#pragma once

//STD Library Imports
#include <fstream>
#include <memory>
#include <vector>
#include <libtcod.hpp>

struct Window;

//Class File Imports
#include "entity.h"
#include "tile.h"
#include "map.h"
#include "input.h"
#include "settings.h"
#include "window.h"
#include "gui.h"
#include "engine.h"

//TODO : optimize and test access modifiers
//TODO : to create and push to vector use vector.push(create ent here)
//TODO : tcod consoles start at 0, 0
//TODO : Figure out what fov algo to use
//TODO : fov is computed every update, try to optimize later

//!NOTE : blitting consoles requires a background flag
//!NOTE : Documentation is done in declaration
//!NOTE : create generic items for tiles and then push them 

//need to change "length" vars to be more easily typed
//need to clean up mapfile code