#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>

#include "SDL_Utils.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 717;
const string WINDOW_TITLE = "An Implementation of Code.org Painter";
int step_delay=1000;
int stepX=0, stepY=0;
SDL_Renderer* renderer;

class LTexture
{
    public:
        LTexture();
        LTexture(int initWidth, int initHeight);
        ~LTexture();
        bool loadFromFile( std::string path);
        void free();
        void render( int x, int y);
        int getWidth();
        int getHeight();

    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};

LTexture gFooTexture(100, 100);
LTexture gBackgroundTexture;
LTexture bullet;

LTexture :: LTexture(){
    mTexture=NULL;
    mWidth=0;
    mHeight=0;
}

LTexture :: LTexture(int initWidth, int initHeight){
    mWidth=initWidth;
    mHeight=initHeight;
}

LTexture ::~ LTexture(){
    free();
}

bool LTexture :: loadFromFile(string path){
    free();
    SDL_Texture* newTexture=NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render( int x, int y)
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy( renderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool loadMedia()
{
    bool success = true;

    if( !gFooTexture.loadFromFile( "plane.png") )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }

    if( !bullet.loadFromFile( "foo.png") )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }

    //Load background texture
    if( !gBackgroundTexture.loadFromFile( "planet.png") )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }

    return success;
}

void move(int &x, int &y){
        x+=stepX;
        y+=stepY;
    }

void turnLeft(){
    stepX = -5;
    stepY = 0;
}

void turnRight(){
    stepX = 5;
    stepY = 0;
}

void turnUp(){
    stepX = 0;
    stepY = -5;
}

void turnDown(){
    stepX = 0;
    stepY = 5;
    }

void moveBullet(int& x, int& y){
    if(y==0) { x = 240; y=360;}
    else y-=5;
}

int main(int argc, char* argv[])
{
    SDL_Window* window=NULL;

    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    SDL_Event e;
        if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			SDL_Event e;
            int x=0, y=0, bulletX=240, bulletY=360;
			while(0 <= x && 0 <= y && SCREEN_WIDTH >= x + gFooTexture.getWidth() && SCREEN_HEIGHT >= y+gFooTexture.getHeight())
			{
			    moveBullet(bulletX, bulletY);
				move(x, y);
                cout << x << y;
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );
				gBackgroundTexture.render( 0, 0);

                bullet.render(bulletX, bulletY);

				gFooTexture.render( x, y);

				SDL_RenderPresent( renderer );
				if(SDL_PollEvent( &e ) == 0) continue;
                if( e.type == SDL_QUIT ) break;
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE: break;
                        case SDLK_LEFT: turnLeft();
                            break;
                        case SDLK_RIGHT: turnRight();
                            break;
                        case SDLK_DOWN: turnDown();
                            break;
                        case SDLK_UP: turnUp();
                            break;
                    default: break;
                    }
				 }
			}
		}
    bullet.free();
    gFooTexture.free();
    gBackgroundTexture.free();
    quitSDL(window, renderer);
    return 0;
}
