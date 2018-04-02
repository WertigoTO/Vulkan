#include <cstdio>
#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>

class SdlScreen
{
public:

	SDL_Window* window;
	SdlScreen(const char* name, int height, int width)
	{
		//init SDL
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
		{
			throw "Failed to init SDL.";
		}

		//Create window
		window = SDL_CreateWindow
		(
			name,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			0
		);
		if(!window)
		{
			SDL_Quit();
			throw "Failed to create SDL window.";
		}
	}
private:
};

int SDL_main(int argc, char** argv)
{
	try
	{
		SdlScreen("Vulkan", 800, 600);
	}
	catch(const char* e)
	{
		printf("%s\n", e);
	}

	return 0;
}