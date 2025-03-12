#include "Window.h"
#include "stdio.h"

int Window::InitWindow(const char* title, int width, int height)
{
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL3

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        title,                  // window title
        width,                               // width, in pixels
        height,                               // height, in pixels
        SDL_WINDOW_VULKAN                  // flags - see below
    );
    
    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

}

void Window::InitEventLoop()
{
    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;

            }
        }

        // Do game logic, present a frame, etc.
    }
}

void* Window::getHandle()
{
    void* hwnd = NULL;

#if defined(SDL_PLATFORM_WIN32)
    hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
#elif defined(SDL_PLATFORM_MACOS)
    hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
        hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
    }
    else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
        hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
    }
#elif defined(SDL_PLATFORM_IOS)
    hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER, NULL);
#endif

    return hwnd;
}

int Window::Destroy()
{
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
