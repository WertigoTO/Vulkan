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
			SDL_WINDOW_VULKAN
		);
		if(!window)
		{
			SDL_Quit();
			throw "Failed to create SDL window.";
		}
	}
    
    ~SdlScreen()
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
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
        listExtensions();
	}
    
    ~Vulkan()
    {
        vkDestroyInstance(instance, nullptr);
    }



private:
	VkInstance instance;
	SDL_Window* window;

	void createInstance()
	{
		//optional information 
		VkApplicationInfo appInfo;
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = nullptr;
		appInfo.pApplicationName = "Vulkan";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

        //required extensions
		const char** extensionNames;
		uint32_t extensionCount;
        //number and space of required extensions
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
		if(extensionCount == 0)
		{
			throw "No vulkan extension support.";
		}
		extensionNames = new const char*[extensionCount];
        //retrive the actual extension names
		SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames);
        
        printf("Required extensions:\n");
        for(int i = 0; i < extensionCount; i++)
        {
            printf("%s\n", extensionNames[i]);
        }
        
        //required information for creating an instance
        VkInstanceCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
		createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = 0;
		createInfo.enabledExtensionCount = extensionCount;
		createInfo.ppEnabledExtensionNames = extensionNames;

        //create the actual instance
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		if(result != VK_SUCCESS)
		{
			throw "Failed to create instance.";
		}
        
        delete(extensionNames);
	}
    
    void listExtensions()
    {
        uint32_t propertyCount;
        VkExtensionProperties* properties;
        
        vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
        if(propertyCount == 0)
        {
            throw "No extensions found";
        }
        properties = new VkExtensionProperties[propertyCount];
        
        VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties);
        if(result != VK_SUCCESS)
        {
            throw "No extensions found";
        }
        
        printf("Supported extensions:\n");
        for(int i = 0; i < propertyCount; i++)
        {
            printf("%s\n", properties[i].extensionName);
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