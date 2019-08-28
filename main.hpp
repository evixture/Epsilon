#pragma once

//Std Library Imports
#include <fstream>
#include <memory>
#include <vector>
#include <libtcod.hpp>

//Class Declarations
struct Window;
//struct GuiWindow;
//class Entity;

//Class File Imports
#include "entity.h"
#include "map.h"
#include "settings.h"
#include "window.h"
#include "gui.h"
#include "engine.h"

//TODO : optimize and test access modifiers
//TODO : Remember to clear vectors in destructors
//TODO : to create and push to vector use vector.push(create ent here)
//TODO : tcod consoles start at 0, 0

//!NOTE : blitting consoles requires a background flag
//!NOTE : Documentation is done in declaration
