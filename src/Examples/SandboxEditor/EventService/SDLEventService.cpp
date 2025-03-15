#include "SDLEventService.h"

#include <SDL3/SDL_events.h>

void SDLEventService::InitEventLoop()
{
    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
            else
            {
            }
        }

        // Do game logic, present a frame, etc.
    }
}
