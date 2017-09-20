/*
 * maze.h
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
#ifndef MAZE_H
#define MAZE_H
#include <stdbool.h>

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
