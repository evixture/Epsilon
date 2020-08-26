set(PROJECT_NAME Epsilon)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "data/maps/debugmap.emp"
)
source_group("" FILES ${no_group_source_files})

set(Libraries
    "../../../../pugixml-1.10/src/pugixml.cpp"
)
source_group("Libraries" FILES ${Libraries})

set(Resources
    "resource.h"
)
source_group("Resources" FILES ${Resources})

set(Source__AICreature
    "aicreature.cpp"
    "aicreature.h"
)
source_group("Source\\AICreature" FILES ${Source__AICreature})

set(Source__Audio
    "audio.cpp"
    "audio.h"
)
source_group("Source\\Audio" FILES ${Source__Audio})

set(Source__Engine
    "engine.cpp"
    "engine.h"
)
source_group("Source\\Engine" FILES ${Source__Engine})

set(Source__Entity
    "creature.cpp"
    "creature.h"
)
source_group("Source\\Entity" FILES ${Source__Entity})

set(Source__Environment
    "environment.cpp"
    "environment.h"
)
source_group("Source\\Environment" FILES ${Source__Environment})

set(Source__Gui
    "gui.cpp"
    "gui.h"
)
source_group("Source\\Gui" FILES ${Source__Gui})

set(Source__Input
    "input.cpp"
    "input.h"
)
source_group("Source\\Input" FILES ${Source__Input})

set(Source__Item
    "item.cpp"
    "item.h"
)
source_group("Source\\Item" FILES ${Source__Item})

set(Source__Main
    "main.cpp"
    "main.hpp"
)
source_group("Source\\Main" FILES ${Source__Main})

set(Source__Map
    "map.cpp"
    "map.h"
)
source_group("Source\\Map" FILES ${Source__Map})

set(Source__Panel
    "panel.cpp"
    "panel.h"
)
source_group("Source\\Panel" FILES ${Source__Panel})

set(Source__Settings
    "settings.cpp"
    "settings.h"
)
source_group("Source\\Settings" FILES ${Source__Settings})

set(Source__Tile
    "tile.cpp"
    "tile.h"
)
source_group("Source\\Tile" FILES ${Source__Tile})

set(Source__Tool
    "tool.cpp"
    "tool.h"
)
source_group("Source\\Tool" FILES ${Source__Tool})

set(Source__Utility
    "utility.cpp"
    "utility.h"
)
source_group("Source\\Utility" FILES ${Source__Utility})

set(Source__Window
    "window.cpp"
    "window.h"
)
source_group("Source\\Window" FILES ${Source__Window})

set(ALL_FILES
    ${no_group_source_files}
    ${Libraries}
    ${Resources}
    ${Source__AICreature}
    ${Source__Audio}
    ${Source__Engine}
    ${Source__Entity}
    ${Source__Environment}
    ${Source__Gui}
    ${Source__Input}
    ${Source__Item}
    ${Source__Main}
    ${Source__Map}
    ${Source__Panel}
    ${Source__Settings}
    ${Source__Tile}
    ${Source__Tool}
    ${Source__Utility}
    ${Source__Window}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE SWATGame)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libtcod/custom-libtcod-master/src;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../SFML/SFML-2.5.1/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../SoLoud/soloud-RELEASE_20200207/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libtcod/custom-libtcod-master/src;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../SFML/SFML-2.5.1/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../SoLoud/soloud-RELEASE_20200207/include"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /ZI;
                /fp:precise
            >
            $<$<CONFIG:Release>:
                /Gm-;
                /Ox;
                /Gy;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            >
            /permissive-;
            /std:c++17;
            /Oi;
            /sdl;
            /W3;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /fp:precise
            >
            $<$<CONFIG:Release>:
                /Gm-;
                /Ox;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /Oi;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL;
                /VERBOSE:LIB
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:WINDOWS
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:WINDOWS
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "sfml-window;"
        "sfml-system;"
        "kernel32;"
        "user32;"
        "gdi32;"
        "winspool;"
        "comdlg32;"
        "advapi32;"
        "shell32;"
        "ole32;"
        "oleaut32;"
        "uuid;"
        "odbc32;"
        "odbccp32;"
        "libtcod;"
        "soloud_static64"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "sfml-window;"
        "sfml-system;"
        "kernel32;"
        "user32;"
        "gdi32;"
        "winspool;"
        "comdlg32;"
        "advapi32;"
        "shell32;"
        "ole32;"
        "oleaut32;"
        "uuid;"
        "odbc32;"
        "odbccp32;"
        "libtcod;"
        "soloud"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "E:/SFML/SFML-2.5.1/lib;"
        "E:/Libtcod/custom-libtcod-master/build/msvs/libtcod/x64/Release;"
        "E:/SoLoud/soloud-RELEASE_20200207/build/vs2019/x64/Release"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "E:/SoLoud/soloud-RELEASE_20200207/lib;"
        "E:/SFML/SFML-2.5.1/lib;"
        "E:/Libtcod/custom-libtcod-master/build/msvs/libtcod/x64/Release"
    )
endif()

