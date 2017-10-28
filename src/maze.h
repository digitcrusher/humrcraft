/*
 * maze.h
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
#ifndef MAZE_H
#define MAZE_H
#include <stdbool.h>

//TODO: add the "MAZE_" prefix to variables

enum {
    Wall,
    Path,
    Footprint,
    Exit
};
enum {
    Up,
    Right,
    Down,
    Left
};
struct tile {
    int type, direction;
};
struct world {
    int w, h;
    struct tile* tiles;
};
struct lfr {
    struct tile l, f, r;
    int h;
};
#ifdef __cplusplus
extern "C" {
#endif
struct world* createWorld(int w, int h);
void deleteWorld(struct world* world);
void resetWorld(struct world* world, struct tile to);
struct tile* getTile(struct world* world, int x, int y);
bool isInBounds(struct world* world, int x, int y);
bool compareTile(struct tile t1, struct tile t2);
struct lfr generateLFR(struct world* world, int xs, int ys, int rot);
bool compareLFR(struct lfr lfr1, struct lfr lfr2);
bool createLinearMaze(struct world* world, int startx, int starty, struct tile start, struct tile end, struct tile fillwith, int corridorlen, int seed); //TODO: make an iterative maze generation function
bool solve(struct world* world, int* x, int* y, int* heading, struct lfr* last);
#ifdef __cplusplus
}
#endif

#endif
