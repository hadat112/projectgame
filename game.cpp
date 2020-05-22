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
const int EXPLOSION_FRAMES = 4;
int frame=0;
SDL_Rect spriteClips[ EXPLOSION_FRAMES ];

Texture backgroundGame;
Texture enermy1(-1000, 0);
Texture start( ( SCREEN_WIDTH - start.getWidth() ) / 2 - 140, ( SCREEN_HEIGHT - start.getHeight() ) / 2 + 150 );
Texture score(50, 50);
Texture gameOver((SCREEN_WIDTH)/2 - 110, SCREEN_HEIGHT/2-100);
Texture huongDan(0, SCREEN_HEIGHT-300);
Texture title(-5, 100);

void loadMedia(SDL_Renderer* renderer, Texture* ourPlane, Texture* explosion);

void resetPos(Texture enermy[], Texture enermyBullet[]);
bool isGameOver(Texture enermy[], Texture enermyBullet[], Texture* ourPlane);
bool banTrung(Texture enermy[], vector<Texture*> bullet, int & checkEnermy, int & checkBullet);

void drawExplosion(Texture *ourPlane, Texture *explosion, SDL_Renderer* renderer);
void drawBackground(SDL_Renderer* renderer, Texture* ourPlane);
void drawMenu(SDL_Renderer* renderer,const int& lanchoi, bool& startGame, SDL_Event& e, Texture enermy[], Texture enermyBullet[]);
void drawGamePlay(SDL_Renderer* renderer, Texture enermy[], vector<Texture*> & bullet, const int& diem, Texture enermyBullet[]);

void close(Texture enermy[], Texture enermyBullet[], Texture* ourPlane, Texture* explosion);

int main(int argc, char* argv[])
{
    srand(time(0));

    SDL_Window* window=NULL;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    bool startGame = false;
    int checkBullet, checkEnermy, diem=0, lanchoi=1;

    Texture* explosion;
    explosion->x = SCREEN_WIDTH/2-50;
    explosion->y = SCREEN_HEIGHT/2;
    vector<Texture *> bullet;
    Texture* ourPlane=new Texture;
    ourPlane->x = SCREEN_WIDTH/2-50;
    ourPlane->y = SCREEN_HEIGHT-ourPlane->getHeight()-100;
    SDL_Event e;

    loadMedia(renderer, ourPlane, explosion);
    Texture enermy[5];
    for (int i=0; i<4; i++)
        enermy[i]=enermy1;
    Texture enermyBullet[5];
    for(int i=0; i<4; i++)
    {
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

            if(banTrung(enermy, bullet, checkEnermy, checkBullet))
            {
                bullet[checkBullet]->x=-100;
                enermy[checkEnermy].x=10000;
                diem+=10;
            }
            if(isGameOver(enermy, enermyBullet, ourPlane))
            {
                drawExplosion(ourPlane, explosion, renderer);
                if(frame/4 >=EXPLOSION_FRAMES )
                {
                    startGame = false;
                    ourPlane->x=SCREEN_WIDTH/2-20;
                    diem=0;
                    lanchoi++;
                }
            } else
                {
                    movePlane(ourPlane, SCREEN_WIDTH);
                    moveEnermy(enermy, diem, SCREEN_WIDTH, SCREEN_HEIGHT);
                    if(diem>100)
                        moveEnermyBullet(enermy, enermyBullet, diem, SCREEN_WIDTH, SCREEN_HEIGHT);
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
        close(enermy, enermyBullet, ourPlane, explosion);
        quitSDL(window, renderer);
        return 0;
    }

    void loadMedia(SDL_Renderer* renderer, Texture* ourPlane, Texture* explosion)
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

        if( !start.loadFromRenderedText( "press SPACE to play", textColor, renderer ) )
            cout << "Failed to render text texture!" << endl;

        if( !explosion->loadFromFile( "explosion.png", renderer ) )
        {
            printf( "Failed to load explosion texture!\n" );
        }
        else
        {
            spriteClips[ 0 ].x =   0;
            spriteClips[ 0 ].y =   0;
            spriteClips[ 0 ].w =  64;
            spriteClips[ 0 ].h = 205;

            spriteClips[ 1 ].x =  64;
            spriteClips[ 1 ].y =   0;
            spriteClips[ 1 ].w =  64;
            spriteClips[ 1 ].h = 205;

            spriteClips[ 2 ].x = 128;
            spriteClips[ 2 ].y =   0;
            spriteClips[ 2 ].w =  64;
            spriteClips[ 2 ].h = 205;

            spriteClips[ 3 ].x = 196;
            spriteClips[ 3 ].y =   0;
            spriteClips[ 3 ].w =  64;
            spriteClips[ 3 ].h = 205;
        }

    }

    bool isGameOver(Texture enermy[], Texture enermyBullet[], Texture* ourPlane)
    {
        for(int i=0; i<4; i++)
        {
            int enermyBottom = enermy[i].y + ourPlane->getHeight() - 1/2*(ourPlane->getHeight());
            int enermyLeft = enermy[i].x+enermy[i].getWidth();
            int enermyRight = enermy[i].x-enermy[i].getWidth();
            if(ourPlane->y <= enermyBottom && enermyLeft > ourPlane->x && enermyRight < ourPlane->x)
            {
                return true;
            }
            int enermyBulletTop = enermyBullet[i].y + ourPlane->getHeight() - 1/2*(ourPlane->getHeight());
            int enermyBulletLeft = enermyBullet[i].x+enermyBullet[i].getWidth()- 20;
            int enermyBulletRight = enermyBullet[i].x-enermyBullet[i].getWidth();
            if(ourPlane->y <= enermyBulletTop && enermyBulletLeft > ourPlane->x && enermyBulletRight < ourPlane->x)
            {
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
        frame=0;
    }

    void drawExplosion(Texture *ourPlane, Texture *explosion, SDL_Renderer* renderer)
    {
        SDL_Rect* currentClip = &spriteClips[ frame / 4 ];
        explosion->renderClip( ourPlane->x-50, ourPlane->y-100, currentClip, renderer );
        ++frame;
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

    void drawMenu(SDL_Renderer* renderer,const int& lanchoi, bool& startGame, SDL_Event& e, Texture enermy[], Texture enermyBullet[])
    {
        start.render(renderer);
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

    void drawGamePlay(SDL_Renderer* renderer, Texture enermy[], vector<Texture*> & bullet, const int& diem, Texture enermyBullet[])
    {
        SDL_Color textColor = { 255, 255, 255 };
        score.loadFromRenderedText( to_string(diem), textColor, renderer );
        score.render(renderer);

        for(int i=0; i<4; i++)
        {
            enermy[i].render(renderer);
            enermyBullet[i].render(renderer);
        }
        for (unsigned int i=0; i<bullet.size(); i++)
        {
            Texture* p = bullet[i];
            if(p!=NULL)
            {
                moveBullet(p);
                p->render(renderer);
                if(p->y<0)
                {
                    delete p;
                    bullet.erase(bullet.begin()+i);
                }
            }
        }
    }

    void close( Texture enermy[], Texture enermyBullet[], Texture* ourPlane, Texture* explosion)
    {
        for (int i=0; i<4; i++)
        {
            enermy[i].free();
            enermyBullet[i].free();
        }
        huongDan.free();
        title.free();
        gameOver.free();
        start.free();
        score.free();
        explosion->free();
        delete explosion;
        ourPlane->free();
        delete ourPlane;
        enermy1.free();
        backgroundGame.free();
    }
