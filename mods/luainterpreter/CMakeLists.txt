cmake_minimum_required(VERSION 2.8.4)
project(cpp-mods.luainterpreter)

add_subdirectory(lua-5.2.3)

set(SOURCE_FILES luainterpreter.cpp)

include_directories(lua-5.2.3/src)
add_library(luainterpreter ${SOURCE_FILES})
target_link_libraries(luainterpreter lua)
