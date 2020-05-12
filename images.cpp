#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_Utils.h"
#include "images.h"

TTF_Font *gFont = NULL;

using namespace std;
Texture :: Texture(int _x, int _y){
    x=_x;
    y=_y;
}

Texture :: Texture(){
    newTexture=NULL;
    width=0;
    height=0;
    y=0;
    x=0;
}

Texture ::~ Texture(){
    free();
}

bool Texture :: loadFromFile(string path, SDL_Renderer* renderer){
    free();
    SDL_Texture* texture=NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface );
        if( texture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            width = loadedSurface->w;
            height = loadedSurface->h;
        }
        SDL_FreeSurface( loadedSurface );
    }
    newTexture = texture;
    return newTexture != NULL;
}

bool Texture::loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* renderer )
{
    free();
    gFont = TTF_OpenFont( "lazy.ttf", 28 );
    if(gFont== NULL) cout << "cant load font";

    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( newTexture  == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            width = textSurface->w;
            height = textSurface->h;
        }
        SDL_FreeSurface( textSurface );
    }

    return newTexture != NULL;
}

void Texture::free()
{
    if( newTexture != NULL )
    {
        SDL_DestroyTexture( newTexture );
        newTexture = NULL;
        width = 0;
        height = 0;
        x=0;
        y=0;
    }
}

void Texture::render(SDL_Renderer* renderer)
{
    SDL_Rect renderQuad = { x, y, width, height };
    SDL_RenderCopy( renderer, newTexture, NULL, &renderQuad );
}

void Texture :: renderBackground(int a, int b, SDL_Renderer* renderer)
{
    SDL_Rect renderQuad = { a, b, width, height };
    SDL_RenderCopy( renderer, newTexture, NULL, &renderQuad );
}

int Texture::getWidth()
{
    return width;
}

int Texture::getHeight()
{
    return height;
}
