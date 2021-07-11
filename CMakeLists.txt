set(NCPROJECT_NAME "ncInvaders")
set(NCPROJECT_EXE_NAME "ncinvaders")
set(NCPROJECT_VENDOR "Angelo Theodorou")
set(NCPROJECT_COPYRIGHT "Copyright ©2016-2021 ${NCPROJECT_VENDOR}")
set(NCPROJECT_DESCRIPTION "A simplified version of Space Invaders made with the nCine")
set(NCPROJECT_HOMEPAGE "https://ncine.github.io")
set(NCPROJECT_REVERSE_DNS "io.github.ncine.ncinvaders")

set(NCPROJECT_SOURCES
	src/invaders.h
	src/invaders.cpp
	src/Configuration.h
	src/SpritePool.h
	src/SpritePool.cpp
	src/Game.h
	src/Game.cpp
	src/ProjectilePool.h
	src/ProjectilePool.cpp
	src/EnemyPool.h
	src/EnemyPool.cpp
	src/Player.h
	src/Player.cpp
	src/CollisionManager.h
	src/CollisionManager.cpp
)

set(NCPROJECT_ANDROID_ASSETS
	data/DroidSans32_256.fnt
	android/DroidSans32_256.webp
	data/bomb.png
	data/enemy1.png
	data/enemy2.png
	data/player.png
	data/rocket.png
)

# Don't edit beyond this line
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
	return()
endif()

cmake_minimum_required(VERSION 3.13)
project(${NCPROJECT_NAME})

find_path(NCPROJECT_DIR cmake/project_main.cmake
	PATHS ${nCine_DIR} $ENV{NCINE_DIR}
	PATH_SUFFIXES project ../project ../../../project ../../../share/ncine/project
	NO_CMAKE_FIND_ROOT_PATH # For Emscripten
	DOC "Path to the nCine template project scripts")

if(IS_DIRECTORY ${NCPROJECT_DIR})
	include(${NCPROJECT_DIR}/cmake/project_main.cmake)
else()
	message(FATAL_ERROR "Directory \"${NCPROJECT_DIR}\" does not exist")
endif()
