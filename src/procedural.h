/*
 * procedural.h
 * textcraft Source Code
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
#ifndef PROCEDURAL_H
#define PROCEDURAL_H
#include <SDL2/SDL.h>

#include <time.h>
#include <math.h>
#include "graphics.h"

bool tree(SDL_Surface* surface, int color, int seed, int startx, int starty, float startangle, size_t length, size_t lengthdiff
          ,float rotation, float rotationdiff, float branchchance, float branchangle, float branchanglediff);

#endif
