#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "SDL_Utils.h"
#include "images.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 717;
const string WINDOW_TITLE = "An Implementation of Code.org Painter";
int step_delay=1000;
int stepX=0, stepY=0;

Texture ourPlane(SCREEN_WIDTH/2, SCREEN_HEIGHT-ourPlane.getHeight()-100);
Texture backgroundGame;
Texture bullet(-100, 200);
Texture enermy1(-100, 0);

bool loadMedia(SDL_Renderer* renderer);

void movePlane();

void turnLeft();

void turnRight();

void moveBullet();

void moveEnermy(Texture s[]);

void handlePlane(SDL_Event& e);

bool gameOver(Texture s[]) {
    for(int i=0;i<5;i++) {
        int enermyTop = s[i].y + ourPlane.getHeight() - 50;
        int enermyLeft = s[i].x+s[i].getWidth();
        int enermyRight = s[i].x-s[i].getWidth();
        if(ourPlane.y <= enermyTop && enermyLeft > ourPlane.x && enermyRight < ourPlane.x) return true;
    }
    return false;
}

bool banTrung(Texture s[], int & check) {
    for(int i=0;i<5;i++) {
        int enermyTop = s[i].y + s[i].getHeight();
        int enermyLeft = s[i].x - bullet.getWidth();
        int enermyRight = s[i].x+s[i].getWidth();
        if(bullet.y <= enermyTop && enermyLeft <= bullet.x && enermyRight >= bullet.x)
        {
            check = i;
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[])
{
    srand(time(0));
    SDL_Window* window=NULL;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    loadMedia(renderer);
    Texture s[5];
    int check;
    for (int i=0; i<5; i++) {
        s[i]=enermy1;
    }
    int scrollingOffset = 0;
    SDL_Event e;

    while(!gameOver(s))
    {
        moveBullet();
        movePlane();
        moveEnermy(s);

        if(banTrung(s, check)) {
            bullet.x=-100;
            s[check].x=10000;
        }

        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( renderer );

        scrollingOffset+=2;
        if( scrollingOffset > backgroundGame.getHeight() )
        {
            scrollingOffset = 0;
        }
        backgroundGame.renderBackground( 0, scrollingOffset, renderer);
        backgroundGame.renderBackground( 0, scrollingOffset - backgroundGame.getHeight(), renderer);
        for(int i=0; i<5; i++)
        {
            s[i].render(renderer);
        }
        bullet.render(renderer);
        ourPlane.render(renderer);
        SDL_Delay(5);

        SDL_RenderPresent(renderer);

        if(SDL_PollEvent( &e ) == 0) continue;
        if( e.type == SDL_QUIT ) break;
        handlePlane(e);
    }

    bullet.free();
    ourPlane.free();
    enermy1.free();
    backgroundGame.free();
    quitSDL(window, renderer);
    return 0;
}

bool loadMedia(SDL_Renderer* renderer)
{
    bool success = true;

    if( !ourPlane.loadFromFile( "plane.png", renderer) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }

    if( !enermy1.loadFromFile( "enermy.png", renderer) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }

    if( !bullet.loadFromFile( "bullet.png", renderer) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }

    if( !backgroundGame.loadFromFile( "backGround.png", renderer) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }

    return success;
}

void movePlane()
{
    if(ourPlane.x<=0 && stepX==-15)
        ourPlane.x+=0;
    else if(ourPlane.x>=SCREEN_WIDTH-ourPlane.getWidth() && stepX==15)
        ourPlane.x+=0;
    else
        ourPlane.x+=stepX;
}

void turnLeft()
{
    stepX -= 15;
    stepY = 0;
}

void turnRight()
{
    stepX += 15;
    stepY = 0;
}

void handlePlane(SDL_Event& e) {
        if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
        {
            switch( e.key.keysym.sym )
            {
            case SDLK_LEFT:
                turnLeft();
                break;
            case SDLK_RIGHT:
                turnRight();
                break;
            }
        }
        else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
        {
            switch( e.key.keysym.sym )
            {
            case SDLK_LEFT:
                turnRight();
                break;
            case SDLK_RIGHT:
                turnLeft();
                break;
            }
        }
}

void moveBullet()
{
    if(bullet.y<=0)
    {
        bullet.x = ourPlane.x + 50;
        bullet.y=ourPlane.y+50;
    }
    else
        bullet.y-=20;
}

void moveEnermy(Texture s[])
{
    for(int i=0; i<5; i++)
    {
        if(s[i].y>SCREEN_HEIGHT)
        {
            s[i].x=rand()%(SCREEN_WIDTH-s[i].getWidth());
            s[i].y=-100*i;
        }
        s[i].y +=7;
    }
}
