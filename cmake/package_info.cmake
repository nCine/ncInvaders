set(PACKAGE_NAME "ncInvaders")
set(PACKAGE_EXE_NAME "ncinvaders")
set(PACKAGE_ICON_NAME ${PACKAGE_NAME})
set(PACKAGE_DESCRIPTION "A simplified version of Space Invaders made with the nCine")
set(PACKAGE_AUTHOR_MAIL "encelo@gmail.com")
set(PACKAGE_DESKTOP_FILE "io.github.ncine.ncinvaders.desktop")
set(PACKAGE_JAVA_URL "io/github/ncine/ncinvaders")

set(PACKAGE_SOURCES
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

set(PACKAGE_ANDROID_ASSETS
	data/DroidSans32_256.fnt
	android/DroidSans32_256.webp
	data/bomb.png
	data/enemy1.png
	data/enemy2.png
	data/player.png
	data/rocket.png
)
