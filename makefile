all: clean main.o 
	g++ main.o -o vulkan.exe `sdl2-config --cflags --libs` -lvulkan-1

main.o:
	g++ -c main.cpp

clean:
	rm *.o
