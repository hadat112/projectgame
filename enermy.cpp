
#include "enermy.h"

int ENERMY_SPEED=7;

void moveEnermy(Texture enermy[], const int& diem, const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT)
{
    ENERMY_SPEED = 7 + diem*1/500;
    for(int i=0; i<4; i++)
    {
        if(enermy[i].y>SCREEN_HEIGHT)
        {
            enermy[i].x=rand()%(SCREEN_WIDTH-enermy[i].getWidth());
            enermy[i].y=-100*i;
        }
        enermy[i].y += ENERMY_SPEED;
    }
}

void orbit(int& x, int& y, const int& diem, int& BULLET_SPEED){
    BULLET_SPEED = 10 + diem*1/500;
    if(diem>=100) y+=BULLET_SPEED;
    if(diem>200) {
        x=x+5*sin(float(y/100));
    }
}

void moveEnermyBullet(Texture enermy[], Texture enermyBullet[], const int& diem, const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT){
    int BULLET_SPEED=10;
    for(int i=0;i<4;i++){
        if(enermyBullet[i].y>SCREEN_HEIGHT)
        {
            enermyBullet[i].x = enermy[i].x+20;
            enermyBullet[i].y = enermy[i].y;
        }
        orbit(enermyBullet[i].x, enermyBullet[i].y, diem, BULLET_SPEED);
    }
}
