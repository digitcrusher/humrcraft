/*
 * graphics.hpp
 * humrcraft Source Code
 * Available on Github
 *
 * Copyright (C) 2017 Karol "digitcrusher" Łacina
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <SDL2/SDL.h>

int getPixel(SDL_Surface* surface, int x, int y);
void drawPixel(SDL_Surface* surface, int x, int y, int color);
void drawCircle(SDL_Surface* surface, int x, int y, int r, int color);
void drawSquare(SDL_Surface* surface, int x, int y, float rot, int sidelen, int color);
void drawEllipse(SDL_Surface* surface, int x1, int y1, int x2, int y2, int color);
void drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, int color);
void drawRectangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int color);
int drawImage(SDL_Surface* from, int x, int y, SDL_Surface* to);
void drawGraph(SDL_Surface* surface, float (*func)(float), float zoom, float x, float y, int color);

#endif
