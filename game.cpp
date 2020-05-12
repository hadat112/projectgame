#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>
#include <SDL2/SDL_ttf.h>
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
int stepX=0, stepY=0;
int ENERMY_SPEED=7;
int scrollingOffset = 0;

Texture ourPlane(SCREEN_WIDTH/2-50, SCREEN_HEIGHT-ourPlane.getHeight()-100);
Texture backgroundGame;
Texture enermy1(-1000, 0);
Texture test( ( SCREEN_WIDTH - test.getWidth() ) / 2 - 148, ( SCREEN_HEIGHT - test.getHeight() ) / 2 + 150 );
Texture score(50, 50);
Texture gameOver((SCREEN_WIDTH)/2 - 130, SCREEN_HEIGHT/2-100);

void loadMedia(SDL_Renderer* renderer);
void movePlane();
void turnLeft();
void turnRight();
void moveEnermy(Texture s[], const int & diem);
void handlePlane(SDL_Event& e, vector<Texture*>& amo, SDL_Renderer* renderer);
bool isGameOver(Texture s[]);
bool banTrung(Texture s[], vector<Texture*> amo, int & checkEnermy, int & checkBullet);
void resetPos(Texture s[]);
void moveAmo(Texture* p);
void drawBackground(SDL_Renderer* renderer);
void drawMenu(SDL_Renderer* renderer,const int& lanchoi, bool& startGame, SDL_Event& e, Texture s[]);
void drawGamePlay(SDL_Renderer* renderer, Texture s[], vector<Texture*> & amo, const int& diem);

int main(int argc, char* argv[])
{
    srand(time(0));

    SDL_Window* window=NULL;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    loadMedia(renderer);

    bool startGame = true;
    int checkBullet, checkEnermy, diem=0, lanchoi=1;
    Texture s[5];
    vector<Texture *> amo;
    for (int i=0; i<4; i++)
        s[i]=enermy1;
    SDL_Event e;

    while(true)
    {
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( renderer );

        drawBackground(renderer);
        if(startGame==true)
        {
            drawMenu(renderer, lanchoi, startGame, e, s);
        }
        else
        {
            drawGamePlay(renderer, s, amo, diem);

            movePlane();
            moveEnermy(s, diem);
            if(banTrung(s, amo, checkEnermy, checkBullet))
            {
                amo[checkBullet]->x=-100;
                s[checkEnermy].x=10000;
                diem+=10;
            }
            if(isGameOver(s))
            {
                startGame = true;
                diem=0;
                lanchoi++;
            }
        }
        SDL_RenderPresent(renderer);
        if(SDL_PollEvent( &e ) == 0)
            continue;
        if( e.type == SDL_QUIT )
            break;
        handlePlane(e, amo, renderer);
        SDL_Delay(10);
    }

    test.free();
    score.free();
    ourPlane.free();
    enermy1.free();
    backgroundGame.free();
    quitSDL(window, renderer);
    return 0;
}

void loadMedia(SDL_Renderer* renderer)
{
    if( !ourPlane.loadFromFile( "plane.png", renderer) )
        cout << "Failed to load plane texture image!" << endl;

    if( !enermy1.loadFromFile( "enermy.png", renderer) )
        cout <<  "Failed to load enermy texture image!" << endl;

    if( !backgroundGame.loadFromFile( "backGround.png", renderer) )
        cout <<  "Failed to load background texture image!" << endl;

    if( !gameOver.loadFromFile( "gameover.png", renderer) )
        cout << "Failed to load gameover texture image!" << endl;

    SDL_Color textColor = { 255, 242, 0 };

    if( !test.loadFromRenderedText( "press SPACE to play", textColor, renderer ) )
        cout << "Failed to render text texture!" << endl;
}

void movePlane()
{
    if(ourPlane.x<=0 && stepX==-10)
        ourPlane.x+=0;
    else if(ourPlane.x>=SCREEN_WIDTH-ourPlane.getWidth() && stepX==10)
        ourPlane.x+=0;
    else
        ourPlane.x+=stepX;
}

void turnLeft()
{
    stepX -= 10;
    stepY = 0;
}

void turnRight()
{
    stepX += 10;
    stepY = 0;
}

void handlePlane(SDL_Event& e, vector<Texture*>& amo, SDL_Renderer* renderer)
{
    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button== SDL_BUTTON_LEFT)
        {
            Texture* a= new Texture();
            a->loadFromFile("bullet.png", renderer);
            a->x=ourPlane.x + 50;;
            a->y=ourPlane.y+50;;
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

void moveEnermy(Texture s[], const int& diem)
{
    ENERMY_SPEED = 7 + diem*1/100;
    for(int i=0; i<4; i++)
    {
        if(s[i].y>SCREEN_HEIGHT)
        {
            s[i].x=rand()%(SCREEN_WIDTH-s[i].getWidth());
            s[i].y=-100*i;
        }
        s[i].y += ENERMY_SPEED;
    }
}


bool isGameOver(Texture s[])
{
    for(int i=0; i<4; i++)
    {
        int enermyTop = s[i].y + ourPlane.getHeight() - 50;
        int enermyLeft = s[i].x+s[i].getWidth();
        int enermyRight = s[i].x-s[i].getWidth();
        if(ourPlane.y <= enermyTop && enermyLeft > ourPlane.x && enermyRight < ourPlane.x)
        {
            ourPlane.x = -200;
            return true;
        }
    }
    return false;
}

bool banTrung(Texture s[], vector<Texture*> amo, int & checkEnermy, int & checkBullet)
{
    for(int i=0; i<4; i++)
    {
        int enermyTop = s[i].y + s[i].getHeight();
        int enermyLeft = s[i].x - s[i].getWidth();
        int enermyRight = s[i].x+s[i].getWidth();
        for(int j=0; j<amo.size(); j++)
            if(amo[j]->y <= enermyTop && enermyLeft <= amo[j]->x && enermyRight >= amo[j]->x)
            {
                checkEnermy = i;
                checkBullet = j;
                return true;
            }
    }
    return false;
}

void resetPos(Texture s[])
{
    for (int i=0; i<4; i++)
    {
        s[i].x=-1000;
        s[i].y=0;
    }
}

void moveAmo(Texture* p)
{
    p->y -= 20;
}

void drawBackground(SDL_Renderer* renderer)
{
    scrollingOffset+=2;
    if( scrollingOffset > backgroundGame.getHeight() )
    {
        scrollingOffset = 0;
    }
    backgroundGame.renderBackground( 0, scrollingOffset, renderer);
    backgroundGame.renderBackground( 0, scrollingOffset - backgroundGame.getHeight(), renderer);
    ourPlane.render(renderer);
}

void drawMenu(SDL_Renderer* renderer,const int& lanchoi, bool& startGame, SDL_Event& e, Texture s[]){
    test.render(renderer);
    if(lanchoi>1)
        gameOver.render(renderer);
    if (  e.type == SDL_KEYDOWN )
        if(e.key.keysym.sym == SDLK_SPACE)
        {
            resetPos(s);
            ourPlane.x = SCREEN_WIDTH/2-50;
            startGame = false;
        }
}

void drawGamePlay(SDL_Renderer* renderer, Texture s[], vector<Texture*> & amo, const int& diem){
    SDL_Color textColor = { 255, 255, 255 };
    score.loadFromRenderedText( to_string(diem), textColor, renderer );
    score.render(renderer);
    for(int i=0; i<4; i++)
        s[i].render(renderer);
    for (int i=0; i<amo.size(); i++)
    {
        Texture* p = amo.at(i);
        if(p!=NULL)
        {
            moveAmo(p);
            p->render(renderer);
            if(amo[i]->y<0)
            {
                amo.erase(amo.begin()+i);
                delete p;
            }
        }
            }
}
