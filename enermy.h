#ifndef ENERMY_H
#define ENERMY_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>
#include <vector>
#include <cmath>

#include "images.h"

void moveEnermy(Texture enermy[], const int & diem, const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT);
void orbit(int& x, int& y, const int& diem, int& BULLET_SPEED);
void moveEnermyBullet(Texture enermy[], Texture enermyBullet[], const int& diem, const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT);


#endif // ENERMY_H

