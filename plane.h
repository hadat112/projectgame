#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDl_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>

#include "images.h"

void movePlane(Texture* ourPlane, const int& SCREEN_WIDTH);
void turnLeft();
void turnRight();
void handlePlane(SDL_Event& e, vector<Texture*>& amo, SDL_Renderer* renderer, Texture* ourPlane);
void moveBullet(Texture* p);
#endif // PLANE_H
