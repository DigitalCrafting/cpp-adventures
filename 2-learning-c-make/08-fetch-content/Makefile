all: prepare build run

prepare:
	rm -Rf build
	mkdir build
	cd build && cmake ..

build:
	cmake --build ./build

run: 
	./build/app/Executable

dependency:
	cd build && cmake .. --graphviz=graph.dot && dot -Tpng graph.dot -o graph.png


.PHONY: all prepare build dependency
