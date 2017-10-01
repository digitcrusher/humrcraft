/*
 * procedural.cpp
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
#include <time.h>
#include <math.h>
#include "graphics.hpp"
#include "procedural.hpp"

bool tree(SDL_Surface* surface, int color, int seed, int startx, int starty, float startangle,
          size_t length, size_t lengthdiff, float rotation, float rotationdiff, float branchchance, float branchangle, float branchanglediff) {
    if(!surface) {
        return 1;
    }
    int x=startx, y=starty;
    float angle=startangle;
    srand(seed);
    for(unsigned int i=0, calclength=length+(rand()%lengthdiff)*(rand()%2?1:-1); i<calclength; i++) {
        drawPixel(surface, x, y, color);
        if(branchchance && fmod(rand(), 1/branchchance)==0) {
            tree(surface, color, seed, x, y, branchangle+fmod(rand(), branchanglediff)*(rand()%2?1:-1)
                ,length, lengthdiff, rotation, rotationdiff, branchchance, branchangle, branchanglediff);
        }
        angle += rotation+fmod(rand(), rotationdiff)*(rand()%2?1:-1);
        x += cos(angle);
        y += sin(angle);
    }
    return 0;
}
