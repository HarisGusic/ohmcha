
.PHONY: all

all:
	mkdir -p _build/release
	cd _build/release; qmake CONFIG+=release ../../; make
