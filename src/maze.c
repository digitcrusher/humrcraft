/*
 * maze.c
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
#include <stdlib.h>
#include <math.h>
#include "maze.h"

struct world* createWorld(int w, int h) {
    struct world* world = (struct world*)malloc(sizeof(struct world));
    world->w = w;
    world->h = h;
    world->tiles = (struct tile*)malloc(sizeof(struct tile)*w*h);
    return world;
}
void deleteWorld(struct world* world) {
    free(world->tiles);
}
void resetWorld(struct world* world, struct tile to) {
    for(int i=0; i<world->w*world->h; i++) {
        world->tiles[i] = to;
    }
}
struct tile* getTile(struct world* world, int x, int y) {
    return world->tiles+x%world->w+y%world->h*world->w;
}
bool isInBounds(struct world* world, int x, int y) {
    return x >= 0 && y >= 0 && x < world->w && y < world->h;
}
bool compareTile(struct tile t1, struct tile t2) {
    return t1.type == t2.type && t1.direction == t2.direction;
}
struct lfr generateLFR(struct world* world, int xs, int ys, int rot) {
    int i = -1;
    struct lfr lfr;
    lfr.h = rot;
    get:;
    int x=0,y=0;
    switch((rot+i+4) % 4) {
        case Up:
            y=-1;
            break;
        case Left:
            x=-1;
            break;
        case Down:
            y=1;
            break;
        case Right:
            x=1;
            break;
    }
    switch(i) {
        case -1:
            lfr.l = *getTile(world, xs+x, ys+y);
            break;
        case 0:
            lfr.f = *getTile(world, xs+x, ys+y);
            break;
        case 1:
            lfr.r = *getTile(world, xs+x, ys+y);
            break;
    }
    if(i!=1) {
        i++;
        goto get;
    }
    return lfr;
}
bool compareLFR(struct lfr lfr1, struct lfr lfr2) {
    return compareTile(lfr1.l, lfr2.l) &&
           compareTile(lfr1.f, lfr2.f) &&
           compareTile(lfr1.r, lfr2.r);
}
bool createLinearMaze(struct world* world, int startx, int starty, struct tile start, struct tile end, struct tile fillwith, int corridorlen, int seed) {
    if(!isInBounds(world, startx, starty)) {
        return 1;
    }
    /*int x = startx;
    int y = starty;
    int heading = Right;
    srand(seed);
    for(int i=0; i<world->w*world->h/corridorlen; i++) {
        randomize:;
        int new = rand()%3-1;
        int newx = ((heading+new+4)%4 == Left)*-1+((heading+new+4)%4 == Right)*1;
        int newy = ((heading+new+4)%4 == Up)*-1+((heading+new+4)%4 == Down)*1;
        if(!isInBounds(world, x+newx*(corridorlen-1), y+newy*(corridorlen-1)) || compareTile(*getTile(world, x+newx*(corridorlen-1), y+newy*(corridorlen-1)), fillwith)*new) {
            goto randomize;
        }
        heading += new;
        for(int j=0; j<corridorlen-1; j++) {
            *getTile(world, x, y) = fillwith;
            x += newx;
            y += newy;
        }
    }
    *getTile(world, startx, starty) = start;
    *getTile(world, x, y) = end;*/
    struct path {
        int x, y;
    };
    return 0;
}
bool solve(struct world* world, int* x, int* y, int* heading, struct lfr* last) {
    struct lfr pathlfr = {{Path, 0}, {Path, 0}, {Path, 0}, 0};
    struct lfr walllfr = {{Wall, 0}, {Wall, 0}, {Wall, 0}, 0};
    struct lfr curr = generateLFR(world, *x, *y, *heading);
    if(getTile(world, *x, *y)->type == Exit) {
        return 1;
    }
    if(compareLFR(curr, walllfr)) {
        *heading = (*heading+2)%4;
        *last = curr;
    }else if(!compareLFR(curr, *last)) {
        bool again=1;
        choose:;
        int new = (*heading+(rand()%3-1)+4)%4;
        int newx = (new == Left)*-1+(new == Right)*1;
        int newy = (new == Up)*-1+(new == Down)*1;
        bool path = compareLFR(curr, pathlfr);
        if(!isInBounds(world, *x, *y) || getTile(world, *x+newx, *y+newy)->type == Wall) {
            goto choose;
        }
        if(again && path) {
            again = !again;
            goto choose;
        }
        *heading = new;
        *last = curr;
    }
    int headingx = (*heading == Left)*-1+(*heading == Right)*1;
    int headingy = (*heading == Up)*-1+(*heading == Down)*1;
    getTile(world, *x, *y)->type = Footprint;
    getTile(world, *x, *y)->direction = *heading;
    *x += headingx;
    *y += headingy;
    /*
    //check if there is an exit around
    bool exit=map[(point.x-1)+w*point.y].type == Exit ||
              map[(point.x+1)+w*point.y].type == Exit ||
              map[point.x+w*(point.y-1)].type == Exit ||
              map[point.x+w*(point.y+1)].type == Exit;*/
    return 0;
}
