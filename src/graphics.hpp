/*
 * graphics.hpp
 * humrcraft Source Code
 * Available on Github
 *
 * Copyright (c) 2017 Karol "digitcrusher" Łacina
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
