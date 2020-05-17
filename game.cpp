#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "SDL_Utils.h"
#include "images.h"
#include "plane.h"
#include "enermy.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 717;
const string WINDOW_TITLE = "FIGHTER AIRCRAFT";
int scrollingOffset = 0;

Texture backgroundGame;
Texture enermy1(-1000, 0);
Texture test( ( SCREEN_WIDTH - test.getWidth() ) / 2 - 140, ( SCREEN_HEIGHT - test.getHeight() ) / 2 + 150 );
Texture score(50, 50);
Texture gameOver((SCREEN_WIDTH)/2 - 110, SCREEN_HEIGHT/2-100);
Texture huongDan(0, SCREEN_HEIGHT-300);
Texture title(-5, 100);

void loadMedia(SDL_Renderer* renderer, Texture* ourPlane);

void resetPos(Texture enermy[], Texture enermyBullet[]);
bool isGameOver(Texture enermy[], Texture enermyBullet[], Texture* ourPlane);
bool banTrung(Texture enermy[], vector<Texture*> bullet, int & checkEnermy, int & checkBullet);

void drawBackground(SDL_Renderer* renderer, Texture* ourPlane);
void drawMenu(SDL_Renderer* renderer,const int& lanchoi, bool& startGame, SDL_Event& e, Texture enermy[], Texture enermyBullet[]);
void drawGamePlay(SDL_Renderer* renderer, Texture enermy[], vector<Texture*> & bullet, const int& diem, Texture enermyBullet[]);

void close( Texture enermy[], Texture enermyBullet[], Texture* ourPlane);

int main(int argc, char* argv[])
{
    srand(time(0));

    SDL_Window* window=NULL;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    bool startGame = false;
    int checkBullet, checkEnermy, diem=0, lanchoi=1;


    vector<Texture *> bullet;

    Texture* ourPlane=new Texture;
    ourPlane->x = SCREEN_WIDTH/2-50;
    ourPlane->y = SCREEN_HEIGHT-ourPlane->getHeight()-100;
    SDL_Event e;

    loadMedia(renderer, ourPlane);
    Texture enermy[5];
    for (int i=0; i<4; i++)
        enermy[i]=enermy1;
    Texture enermyBullet[5];
    for(int i=0;i<4;i++){
        enermyBullet[i].loadFromFile("enermyBullet.png", renderer);
        enermyBullet[i].x = enermy[i].x+20;
        enermyBullet[i].y = enermy[i].y;
    }
    while(true)
    {
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( renderer );
        drawBackground(renderer, ourPlane);
        if(startGame==false)
        {
            drawMenu(renderer, lanchoi, startGame, e, enermy, enermyBullet);
        }
        else
        {
            drawGamePlay(renderer, enermy, bullet, diem, enermyBullet);

            movePlane(ourPlane, SCREEN_WIDTH);
            moveEnermy(enermy, diem, SCREEN_WIDTH, SCREEN_HEIGHT);

            if(diem>100)
                moveEnermyBullet(enermy, enermyBullet, diem, SCREEN_WIDTH, SCREEN_HEIGHT);

            if(banTrung(enermy, bullet, checkEnermy, checkBullet))
            {
                bullet[checkBullet]->x=-100;
                enermy[checkEnermy].x=10000;
                diem+=10;
            }
            if(isGameOver(enermy, enermyBullet, ourPlane))
            {
                ourPlane->x=SCREEN_WIDTH/2-20;
                startGame = false;
                diem=0;
                lanchoi++;
            }
        }
        SDL_RenderPresent(renderer);
        if(SDL_PollEvent( &e ) == 0)
            continue;
        if( e.type == SDL_QUIT )
            break;
        handlePlane(e, bullet, renderer, ourPlane);
        SDL_Delay(10);
    }
    cout << "end";
    close(enermy, enermyBullet, ourPlane);
    quitSDL(window, renderer);
    return 0;
}

void loadMedia(SDL_Renderer* renderer, Texture* ourPlane)
{
    if( !huongDan.loadFromFile( "huongdan.png", renderer) )
        cout << "Failed to load plane texture image!" << endl;

    if( !title.loadFromFile( "title.png", renderer) )
        cout << "Failed to load plane texture image!" << endl;

    if( !ourPlane->loadFromFile( "plane.png", renderer) )
        cout << "Failed to load plane texture image!" << endl;
    cout << "run";
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

bool isGameOver(Texture enermy[], Texture enermyBullet[], Texture* ourPlane)
{
    for(int i=0; i<4; i++)
    {
        int enermyTop = enermy[i].y + ourPlane->getHeight() - 50;
        int enermyLeft = enermy[i].x+enermy[i].getWidth();
        int enermyRight = enermy[i].x-enermy[i].getWidth();
        if(ourPlane->y <= enermyTop && enermyLeft > ourPlane->x && enermyRight < ourPlane->x)
        {
            ourPlane->x = -200;
            return true;
        }
        int enermyBulletTop = enermyBullet[i].y + ourPlane->getHeight() - 50;
        int enermyBulletLeft = enermyBullet[i].x+enermyBullet[i].getWidth();
        int enermyBulletRight = enermyBullet[i].x-enermyBullet[i].getWidth();
        if(ourPlane->y <= enermyBulletTop && enermyBulletLeft > ourPlane->x && enermyBulletRight < ourPlane->x)
        {
            ourPlane->x = -200;
            return true;
        }
    }
    return false;
}

bool banTrung(Texture enermy[], vector<Texture*> bullet, int & checkEnermy, int & checkBullet)
{
    for(int i=0; i<4; i++)
    {
        int enermyTop = enermy[i].y + enermy[i].getHeight();
        int enermyLeft = enermy[i].x - enermy[i].getWidth();
        int enermyRight = enermy[i].x+enermy[i].getWidth();
        for(unsigned int j=0; j<bullet.size(); j++)
            if(bullet[j]->y <= enermyTop && enermyLeft <= bullet[j]->x && enermyRight >= bullet[j]->x)
            {
                checkEnermy = i;
                checkBullet = j;
                return true;
            }
    }
    return false;
}

void resetPos(Texture enermy[], Texture enermyBullet[])
{
    for (int i=0; i<4; i++)
    {
        enermy[i].x=-1000;
        enermy[i].y=0;
        enermyBullet[i].x=-1000;
        enermyBullet[i].y=0;
    }
}

void drawBackground(SDL_Renderer* renderer, Texture* ourPlane)
{
    scrollingOffset+=2;
    if( scrollingOffset > backgroundGame.getHeight() )
    {
        scrollingOffset = 0;
    }
    backgroundGame.renderBackground( 0, scrollingOffset, renderer);
    backgroundGame.renderBackground( 0, scrollingOffset - backgroundGame.getHeight(), renderer);
    ourPlane->render(renderer);
}

void drawMenu(SDL_Renderer* renderer,const int& lanchoi, bool& startGame, SDL_Event& e, Texture enermy[], Texture enermyBullet[]){
    test.render(renderer);
    huongDan.render(renderer);
    title.render(renderer);
    if(lanchoi>1)
        gameOver.render(renderer);
    if (  e.type == SDL_KEYDOWN )
        if(e.key.keysym.sym == SDLK_SPACE)
        {
            resetPos(enermy, enermyBullet);
            startGame = true;
        }
}

void drawGamePlay(SDL_Renderer* renderer, Texture enermy[], vector<Texture*> & bullet, const int& diem, Texture enermyBullet[]){
    SDL_Color textColor = { 255, 255, 255 };
    score.loadFromRenderedText( to_string(diem), textColor, renderer );
    score.render(renderer);
    for(int i=0; i<4; i++){
        enermy[i].render(renderer);
        enermyBullet[i].render(renderer);
    }
    for (unsigned int i=0; i<bullet.size(); i++)
    {
        Texture* p = bullet.at(i);
        if(p!=NULL)
        {
            moveBullet(p);
            p->render(renderer);
            if(bullet[i]->y<0)
            {
                delete p;
                bullet.erase(bullet.begin()+i);
            }
        }
            }
}

void close( Texture enermy[], Texture enermyBullet[], Texture* ourPlane){
    for (int i=0; i<4; i++){
        enermy[i].free();
        enermyBullet[i].free();
    }
    huongDan.free();
    title.free();
    gameOver.free();
    test.free();
    score.free();
    delete ourPlane;
    ourPlane->free();
    enermy1.free();
    backgroundGame.free();
}
