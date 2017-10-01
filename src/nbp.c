/*
 * nbp.c Node Based Pathfinding
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
#include "nbp.h"

struct NBP_node* NBP_createNode(int x, int y) {
    struct NBP_node* node = (struct NBP_node*)malloc(sizeof(struct NBP_node));
    if(!node) {
        return NULL;
    }
    node->x = x;
    node->y = y;
    node->size = 0;
    node->connections = (struct NBP_node**)malloc(sizeof(struct NBP_node*)*node->size);
    return node;
}
void NBP_destroyNode(struct NBP_node* node) {
    free(node->connections);
    free(node);
}
void NBP_destroyPath(struct NBP_path* path) {
    free(path->nodes);
    free(path);
}
bool NBP_connect(struct NBP_node* node, struct NBP_node* node1) {
    if(!(node && node1)) {
        return 1;
    }
    struct NBP_node** newConnections = (struct NBP_node**)realloc(node->connections, sizeof(struct NBP_node*)*(node->size+1));
    if(!newConnections) {
        return 1;
    }
    node->connections = newConnections;
    node->connections[node->size] = node1;
    node->size++;
    newConnections = (struct NBP_node**)realloc(node1->connections, sizeof(struct NBP_node*)*(node1->size+1));
    if(!newConnections) {
        return 1;
    }
    node1->connections = newConnections;
    node1->connections[node1->size] = node;
    node1->size++;
    return 0;
}
struct NBP_path* NBP_AStarPathFind(struct NBP_node* start, struct NBP_node* goal) {
    struct openSetElement {
        struct NBP_node* node;
        bool free;
        int gScore;
        int fScore;
    };
    struct pathsElement {
        struct NBP_node* id;
        struct NBP_node* node;
    };
    if(!(start && goal)) {
        return NULL;
    }
    size_t closedSetSize = 0;
    struct NBP_node** closedSet = (struct NBP_node**)malloc(sizeof(struct NBP_node*)*closedSetSize);
    size_t openSetSize = 1;
    struct openSetElement* openSet = (struct openSetElement*)malloc(sizeof(struct openSetElement)*openSetSize);
    size_t pathsSize = 0;
    struct pathsElement* paths = (struct pathsElement*)malloc(sizeof(struct pathsElement)*pathsSize);
    openSet[0].node = start;
    openSet[0].free = 0;
    openSet[0].gScore = 0;
    openSet[0].fScore = (abs(start->x-goal->x)+abs(start->y-goal->y))/2;
    while(openSetSize > 0) {
        struct openSetElement current = openSet[0];
        for(unsigned int i=0; i<openSetSize; i++) {
            if(openSet[i].fScore < current.fScore) {
                current = openSet[i];
            }
        }
        if(current.node == goal) {
            size_t size=1;
            struct NBP_node* node = current.node;
            for(; node != start; size++) {
                for(unsigned int i=0; i<pathsSize; i++) {
                    if(paths[i].id == node) {
                        node = paths[i].node;
                        break;
                    }
                }
            }
            struct NBP_path* path = (struct NBP_path*)malloc(sizeof(path));
            path->length = size;
            path->nodes = (struct NBP_node**)malloc(sizeof(struct NBP_node*)*path->length);
            node = current.node;
            path->nodes[size-1] = node;
            for(unsigned int i=0; i<size; i++) {
                if(paths[i].id == node) {
                    node = paths[i].node;
                    path->nodes[size-i-1] = node;
                    break;
                }
            }
            free(closedSet);
            free(openSet);
            free(paths);
            return path;
        }
        for(unsigned int i=0; i<openSetSize; i++) {
            if(openSet[i].node == current.node) {
                openSet[i].free = 1;
                break;
            }
        }
        closedSet = (struct NBP_node**)realloc(closedSet, sizeof(struct NBP_node*)*(closedSetSize+1));
        closedSet[closedSetSize] = current.node;
        closedSetSize++;
        for(unsigned int i=0; i<current.node->size; i++) {
            unsigned int j, neighbor;
            for(j=0; j<closedSetSize; j++) {
                if(closedSet[j] == current.node->connections[i]) {
                    break;
                }
            }
            if(j != closedSetSize) {
                continue;
            }
            for(j=0; j<openSetSize; j++) {
                if(!openSet[j].free && openSet[j].node == current.node->connections[i]) {
                    break;
                }
            }
            if(j == openSetSize) {
                for(j=0; j<openSetSize; j++) {
                    if(openSet[j].free) {
                        openSet[j].node = current.node->connections[i];
                        openSet[j].free = 0;
                    }
                }
                if(j == openSetSize) {
                    openSet = (struct openSetElement*)realloc(openSet, sizeof(struct openSetElement)*(openSetSize+1));
                    openSet[openSetSize].node = current.node->connections[i];
                    openSet[openSetSize].free = 0;
                    openSetSize++;
                }
                neighbor = j;
                openSet[j].gScore = (unsigned int)-1>>2;
            }
            int tentative_gScore = current.gScore+(abs(current.node->x-openSet[neighbor].node->x)+abs(current.node->y-openSet[neighbor].node->y))/2;
            if(tentative_gScore >= openSet[neighbor].gScore) {
                continue;
            }
            paths = (struct pathsElement*)realloc(paths, sizeof(struct pathsElement)*(pathsSize+1));
            paths[pathsSize].id = openSet[neighbor].node;
            paths[pathsSize].node = current.node;
            pathsSize++;
            openSet[neighbor].gScore = tentative_gScore;
            openSet[neighbor].fScore = tentative_gScore+(abs(openSet[neighbor].node->x-goal->x)+abs(openSet[neighbor].node->y-goal->y))/2;
        }
    }
    free(closedSet);
    free(openSet);
    free(paths);
    return NULL;
}
struct NBP_path* NBP_TreePathFind(struct NBP_node* start, struct NBP_node* goal) {
    struct path {
        struct NBP_path* path;
        struct NBP_node* nextNode;
        int nextCost;
        int cost;
    };
    size_t pathsSize=1;
    struct path* paths = (struct path*)malloc(sizeof(struct path)*pathsSize);
    size_t exploredNodesSize = 1;
    struct NBP_node** exploredNodes = (struct NBP_node**)malloc(sizeof(struct NBP_node*)*exploredNodesSize);
    exploredNodes[0] = start;
    paths[0].path = (struct NBP_path*)malloc(sizeof(struct NBP_path));
    paths[0].path->length = 1;
    paths[0].path->nodes = (struct NBP_node**)malloc(sizeof(struct NBP_node*)*paths[0].path->length);
    paths[0].path->nodes[0] = start;
    paths[0].nextNode = NULL;
    paths[0].nextCost = 0;
    paths[0].cost = 0;
    while(pathsSize > 0) {
        for(unsigned int i=0; i<pathsSize-1; i++) {
            for(unsigned int j=0; j<pathsSize-i-1; j++) {
                if(paths[j].cost+paths[j].nextCost > paths[j+1].cost+paths[j+1].nextCost) {
                    struct path swap = paths[j];
                    paths[j] = paths[j+1];
                    paths[j+1] = swap;
                }
            }
        }
        struct path* current = paths;
        struct NBP_node* lastNode = current->path->nodes[current->path->length-1];
        if(lastNode == goal) {
            struct NBP_path* path = current->path;
            for(unsigned int j=0; j<pathsSize; j++) {
                if(paths+j != current) {
                    NBP_destroyPath(paths[j].path);
                }
            }
            free(paths);
            return path;
        }
        bool newPath=0;
        for(unsigned int j=0; j<lastNode->size; j++) {
            struct NBP_node* nextNode = lastNode->connections[j];
            bool stop=0;
            for(unsigned int k=0; k<exploredNodesSize; k++) {
                if(nextNode == exploredNodes[k]) {
                    stop = 1;
                    break;
                }
            }
            if(stop) {
                continue;
            }
            exploredNodes = (struct NBP_node**)realloc(exploredNodes, sizeof(struct NBP_node*)*(exploredNodesSize+1));
            exploredNodesSize++;
            exploredNodes[exploredNodesSize-1] = nextNode;
            if(newPath) {
                paths = (struct path*)realloc(paths, sizeof(struct path)*(pathsSize+1));
                pathsSize++;
                current = paths;
                paths[pathsSize-1].path = (struct NBP_path*)malloc(sizeof(struct NBP_path));
                paths[pathsSize-1].path->length = current->path->length;
                paths[pathsSize-1].path->nodes = (struct NBP_node**)malloc(sizeof(struct NBP_node*)*paths[pathsSize-1].path->length);
                memcpy(paths[pathsSize-1].path->nodes, current->path->nodes, sizeof(struct NBP_node*)*paths[pathsSize-1].path->length);
                paths[pathsSize-1].nextNode = nextNode;
                paths[pathsSize-1].nextCost = (abs(nextNode->x-lastNode->x)+abs(nextNode->y-lastNode->y))/2;
                paths[pathsSize-1].cost = current->cost;
            }else {
                current->nextCost = (abs(nextNode->x-lastNode->x)+abs(nextNode->y-lastNode->y))/2;
                current->nextNode = nextNode;
                newPath = 1;
            }
        }
        current->path->nodes = (struct NBP_node**)realloc(current->path->nodes, sizeof(struct NBP_node*)*(current->path->length+1));
        current->path->length++;
        current->path->nodes[current->path->length-1] = current->nextNode;
        current->cost += current->nextCost;
    }
    return NULL;
}
