/*
 * graphics.cpp
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
#include "graphics.hpp"
#include "math.hpp"

namespace graphics {
    int getPixel(SDL_Surface* surface, int x, int y) {
        if(math::pointInSquare((math::V2i){x, y}, (math::V2iPair){0, 0, surface->w-1, surface->h-1})) {
            return *(int*)((char*)surface->pixels+(x+y*surface->w)*(surface->pitch/surface->w));
        }
        return 0;
    }
    void drawPixel(SDL_Surface* surface, int x, int y, int color) {
        if(math::pointInSquare((math::V2i){x, y}, (math::V2iPair){0, 0, surface->w-1, surface->h-1})) {
            uint8_t r1, g1, b1, a1, r2, g2, b2, a2;
            int pixel = getPixel(surface, x, y);
            SDL_GetRGBA(color, surface->format, &r1, &g1, &b1, &a1);
            SDL_GetRGBA(pixel, surface->format, &r2, &g2, &b2, &a2);
            if(a1 != 0) {
                *(int*)((char*)surface->pixels+(x+y*surface->w)*(surface->pitch/surface->w)) = SDL_MapRGBA(surface->format
                                                                      ,(r1*a1+r2*a2*(1-a1/255))/(a1+a2*(1-a1/255))
                                                                      ,(g1*a1+g2*a2*(1-a1/255))/(a1+a2*(1-a1/255))
                                                                      ,(b1*a1+b2*a2*(1-a1/255))/(a1+a2*(1-a1/255)), 255);
            }
        }
    }
    void drawCircle(SDL_Surface* surface, int x, int y, int r, int color) {
        for(double i=0; i<math::pi*2; i+=math::pi/4/r) {
            drawPixel(surface, (int)x+sin(i)*r, (int)y+cos(i)*r, color);
        }
    }
    void drawSquare(SDL_Surface* surface, int x, int y, float rot, int sidelen, int color) {
        for(double i=0; i<math::pi*2; i+=math::pi/4/sidelen) {
            drawPixel(surface, (int)x+sin(i)*math::squarer(i+-rot, sidelen), (int)y+cos(i)*math::squarer(i+-rot, sidelen), color);
        }
    }
    void drawEllipse(SDL_Surface* surface, int x1, int y1, int x2, int y2, int color) {
        int x = (x2-x1)/2;
        int y = (y2-y1)/2;
        for(double i=0; i<math::pi*2; i+=math::pi/4/(((x2-x1)/2+(y2-y1)/2)/2)) {
            drawPixel(surface, (int)x+sin(i)*((x2-x1)/2), (int)y+cos(i)*((y2-y1)/2), color);
        }
    }
    void drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, int color) {
        int d, dx, dy, ai, bi, xi, yi;
        int x = x1, y = y1;
        if (x1 < x2) {
            xi = 1;
            dx = x2 - x1;
        } else {
            xi = -1;
            dx = x1 - x2;
        }
        if (y1 < y2) {
            yi = 1;
            dy = y2 - y1;
        } else {
            yi = -1;
            dy = y1 - y2;
        }
        drawPixel(surface, x, y, color);
        if (dx > dy) {
            ai = (dy - dx) * 2;
            bi = dy * 2;
            d = bi - dx;
            while (x != x2) {
                if (d >= 0) {
                    x += xi;
                    y += yi;
                    d += ai;
                } else {
                    d += bi;
                    x += xi;
                }
                drawPixel(surface, x, y, color);
            }
        } else {
            ai = ( dx - dy ) * 2;
            bi = dx * 2;
            d = bi - dy;
            while (y != y2) {
                if (d >= 0) {
                    x += xi;
                    y += yi;
                    d += ai;
                } else {
                    d += bi;
                    y += yi;
                }
                drawPixel(surface, x, y, color);
            }
        }
    }
    void drawRectangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int color) {
        drawLine(surface, x1, y1, x2, y1, color);
        drawLine(surface, x2, y1, x2, y2, color);
        drawLine(surface, x2, y2, x1, y2, color);
        drawLine(surface, x1, y2, x1, y1, color);
    }
    int drawImage(SDL_Surface* from, int x, int y, SDL_Surface* to) {
        SDL_Rect from_rect;
        from_rect.x = -(x);
        from_rect.y = -(y);
        from_rect.w = x+from->w;
        from_rect.h = y+from->h;
        return SDL_BlitSurface(from, &from_rect, to, NULL);
    }
    void drawGraph(SDL_Surface* surface, float (*func)(float), float zoom, float x, float y, int color) {
        int w = surface->w;
        int h = surface->h;
        drawLine(surface, 0, h/2+y, w, h/2+y, color);
        drawLine(surface, w/2-x, 0, w/2-x, h, color);
        for(int i=0; i<w; i++) {
            drawPixel(surface, i, h/2-func((i+x-w/2)/zoom)*zoom+y, color);
        }
    }
}
