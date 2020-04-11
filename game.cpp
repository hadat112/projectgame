#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>

#include "SDL_Utils.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 717;
const string WINDOW_TITLE = "An Implementation of Code.org Painter";
int step_delay=1000;
int stepX=0, stepY=0;
SDL_Renderer* renderer;

class LTexture
{
    public:
        LTexture();
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

LTexture gFooTexture;
LTexture gBackgroundTexture;
LTexture bullet1;
LTexture bullet2;
LTexture bullet3;

LTexture :: LTexture(){
    mTexture=NULL;
    mWidth=0;
    mHeight=0;
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

    if( !bullet1.loadFromFile( "foo.png") )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }

    if( !bullet2.loadFromFile( "foo.png") )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }


    if( !bullet3.loadFromFile( "foo.png") )
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
    if(x<=0 && stepX==-15)
        x+=0;
    else if(x>=SCREEN_WIDTH-gFooTexture.getWidth() && stepX==15)
        x+=0;
     else
        x+=stepX;
    y+=stepY;
    }

void turnLeft(){
    stepX = -15;
    stepY = 0;
}

void turnRight(){
    stepX = 15;
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

void moveBullet(int &bulletX, int& bulletY, int const& x, int const &y){
    if(bulletY<=0) { bulletX = x + 50; bulletY=y;}
    else bulletY-=20;
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
            int x=SCREEN_WIDTH/2, y=SCREEN_HEIGHT-1-gFooTexture.getHeight(), bulletX=-100, bulletY=200, bullet2X=15, bullet2Y=0, bullet3X=-100, bullet3Y=400;
			while(0 <= y && SCREEN_HEIGHT >= y+gFooTexture.getHeight())
			{
                moveBullet(bulletX, bulletY, x, y);
                moveBullet(bullet2X, bullet2Y, x, y);
                moveBullet(bullet3X, bullet3Y, x, y);

				move(x, y);
                cout << x << y << bullet2X << bullet2Y << bulletY << endl;
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );
				gBackgroundTexture.render( 0, 0);

				gFooTexture.render( x, y);

				bullet1.render(bulletX, bulletY);

                bullet2.render(bullet2X, bullet2Y);

                bullet3.render(bullet3X, bullet3Y);

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
    bullet1.free();
    bullet2.free();
    bullet3.free();
    gFooTexture.free();
    gBackgroundTexture.free();
    quitSDL(window, renderer);
    return 0;
}
