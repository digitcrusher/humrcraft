/*
 * nbp.h Node Based Pathfinding
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
#ifndef NBP_H
#define NBP_H
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct NBP_node {
    int x, y;
    struct NBP_node** connections;
    size_t size;
};
typedef struct NBP_node NBP_node;
struct NBP_path {
    NBP_node** nodes;
    size_t length;
};
typedef struct NBP_path NBP_path;

NBP_node* NBP_createNode(int x, int y);
void NBP_destroyNode(NBP_node* node);
void NBP_destroyPath(NBP_path* path);
bool NBP_connect(NBP_node* node, NBP_node* node1);
NBP_path* NBP_AStarPathFind(NBP_node* start, NBP_node* goal);
NBP_path* NBP_TreePathFind(NBP_node* start, NBP_node* goal);

#ifdef __cplusplus
}
#endif

#endif
