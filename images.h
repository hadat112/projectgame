#ifndef SDL_IMAGES__H
#define SDL_IMAGES__H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_Utils.h"

using namespace std;

class Texture
{
public:
    Texture(int _x, int _y);
    Texture();
    ~Texture();
    bool loadFromFile( std::string path, SDL_Renderer* renderer);
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* renderer );
    void free();
    void render(SDL_Renderer* renderer);
    void renderBackground(int a, int b, SDL_Renderer* renderer);
    void renderClip(int a, int b, SDL_Rect* currentClip, SDL_Renderer* renderer);
    int getWidth();
    int getHeight();
    int x;
    int y;

private:
    SDL_Texture* newTexture;
    int width;
    int height;

};
#endif
