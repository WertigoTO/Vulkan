all: main.o
	g++ main.o -o vulkan.exe `sdl2-config --cflags --libs`

main.o:
	g++ -c main.cpp
