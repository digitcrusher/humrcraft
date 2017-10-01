/*
 * nbp.h Node Based Pathfinding
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
