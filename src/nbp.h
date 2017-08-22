/*
 * nbp.h Node Based Pathfinding
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
struct NBP_path {
    struct NBP_node** nodes;
    size_t length;
};

struct NBP_node* NBP_createNode(int x, int y);
void NBP_destroyNode(struct NBP_node* node);
void NBP_destroyPath(struct NBP_path* path);
bool NBP_connect(struct NBP_node* node, struct NBP_node* node1);
struct NBP_path* NBP_AStarPathFind(struct NBP_node* start, struct NBP_node* goal);
struct NBP_path* NBP_TreePathFind(struct NBP_node* start, struct NBP_node* goal);

#ifdef __cplusplus
}
#endif

#endif
