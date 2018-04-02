#include <cstdio>
#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

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

class Vulkan
{
public:

	Vulkan(SDL_Window* window)
	{
		this->window = window;
		createInstance();
	}



private:
	VkInstance instance;
	SDL_Window* window;

	bool createInstance()
	{
		//optional information 
		VkApplicationInfo appInfo;
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		//Obligatory information for the instance
		const char** extensionNames;
		uint32_t extensionCount;
		SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
		if(extensionCount == 0)
		{
			throw "No vulkan extension support.";
		}
		extensionNames = new const char*[extensionCount + 1];
		extensionNames[0] = VK_KHR_SURFACE_EXTENSION_NAME;
		SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, &extensionNames[1]);
		VkInstanceCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = extensionCount;
		createInfo.ppEnabledExtensionNames = extensionNames;
		createInfo.enabledLayerCount = 0;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		if(result != VK_SUCCESS)
		{
			throw "Failed to create instance.";
		}
	}
};

int SDL_main(int argc, char** argv)
{
	try
	{
		SdlScreen screen("Vulkan", 800, 600);
		Vulkan vulkan(screen.window);
	}
	catch(const char* e)
	{
		printf("%s\n", e);
	}

	return 0;
}