#ifndef SDL_UTILS__H
#define SDL_UTILS__H

#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT, const string& WINDOW_TITLE);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilKeyPressed();

#endif
