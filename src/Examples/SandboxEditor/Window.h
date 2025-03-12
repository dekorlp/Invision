#include <SDL3/SDL.h>

#ifndef WINDOW_H
#define WINDOW_H

class Window
{
private:
	SDL_Window* window = nullptr;
	bool done = false;

public:
	

	int InitWindow(const char* title, int width, int height);
	void InitEventLoop();
	void* getHandle();
	int Destroy();
};

#endif // WINDOW_H