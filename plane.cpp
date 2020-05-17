
#include "plane.h"

Mix_Chunk *fire=NULL;
int PLANE_SPEED=0;

void movePlane(Texture* ourPlane, const int& SCREEN_WIDTH)
{
    if(ourPlane->x<=0 && PLANE_SPEED==-10)
        ourPlane->x+=0;
    else if(ourPlane->x>=SCREEN_WIDTH-ourPlane->getWidth() && PLANE_SPEED==10)
        ourPlane->x+=0;
    else
        ourPlane->x+=PLANE_SPEED;
}

void turnLeft()
{
    PLANE_SPEED -= 10;
}

void turnRight()
{
    PLANE_SPEED += 10;
}

void handlePlane(SDL_Event& e, vector<Texture*>& amo, SDL_Renderer* renderer, Texture* ourPlane)
{
    fire = Mix_LoadWAV("fire.wav");
    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button== SDL_BUTTON_LEFT)
        {
            Mix_PlayChannel( -1, fire, 0 );
            Texture* a= new Texture();
            a->loadFromFile("bullet.png", renderer);
            a->x=ourPlane->x + 50;;
            a->y=ourPlane->y+50;;
            amo.push_back(a);
        }
    }
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

void moveBullet(Texture* p)
{
    p->y -= 20;
}
