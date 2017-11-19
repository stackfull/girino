

.PHONY: clion all build clean

all: clean build

build:
	platformio run

clean:
	platformio run --target clean

tags:
	ctags -R src

clion: .idea CMakeListsPrivate.txt CMakeLists.txt

.idea CMakeLists.txt CMakeListsPrivate.txt: platformio.ini src/*
	platformio init --ide clion

