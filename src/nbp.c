/*
 * nbp.c Node Based Pathfinding
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
#include "nbp.h"

NBP_node* NBP_createNode(int x, int y) {
    NBP_node* node = (NBP_node*)malloc(sizeof(NBP_node));
    if(!node) {
        return NULL;
    }
    node->x = x;
    node->y = y;
    node->size = 0;
    node->connections = (NBP_node**)malloc(sizeof(NBP_node*)*node->size);
    return node;
}
void NBP_destroyNode(NBP_node* node) {
    free(node->connections);
    free(node);
}
void NBP_destroyPath(NBP_path* path) {
    free(path->nodes);
    free(path);
}
bool NBP_connect(NBP_node* node, NBP_node* node1) {
    if(!(node && node1)) {
        return 1;
    }
    NBP_node** newConnections = (NBP_node**)realloc(node->connections, sizeof(NBP_node*)*(node->size+1));
    if(!newConnections) {
        return 1;
    }
    node->connections = newConnections;
    node->connections[node->size] = node1;
    node->size++;
    newConnections = (NBP_node**)realloc(node1->connections, sizeof(NBP_node*)*(node1->size+1));
    if(!newConnections) {
        return 1;
    }
    node1->connections = newConnections;
    node1->connections[node1->size] = node;
    node1->size++;
    return 0;
}
NBP_path* NBP_AStarPathFind(NBP_node* start, NBP_node* goal) {
    struct openSetElement {
        NBP_node* node;
        bool free;
        int gScore;
        int fScore;
    };
    struct pathsElement {
        NBP_node* id;
        NBP_node* node;
    };
    if(!(start && goal)) {
        return NULL;
    }
    size_t closedSetSize = 0;
    NBP_node** closedSet = (NBP_node**)malloc(sizeof(NBP_node*)*closedSetSize);
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
            NBP_node* node = current.node;
            for(; node != start; size++) {
                for(unsigned int i=0; i<pathsSize; i++) {
                    if(paths[i].id == node) {
                        node = paths[i].node;
                        break;
                    }
                }
            }
            NBP_path* path = (NBP_path*)malloc(sizeof(path));
            path->length = size;
            path->nodes = (NBP_node**)malloc(sizeof(NBP_node*)*path->length);
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
        closedSet = (NBP_node**)realloc(closedSet, sizeof(NBP_node*)*(closedSetSize+1));
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
    //let's stop here for a moment and let's think $#! did i just write...
}
NBP_path* NBP_TreePathFind(NBP_node* start, NBP_node* goal) {
    struct path {
        NBP_path* path;
        NBP_node* nextNode;
        int nextCost;
        int cost;
    };
    size_t pathsSize=1;
    struct path* paths = (struct path*)malloc(sizeof(struct path)*pathsSize);
    size_t exploredNodesSize = 1;
    NBP_node** exploredNodes = (NBP_node**)malloc(sizeof(NBP_node*)*exploredNodesSize);
    exploredNodes[0] = start;
    paths[0].path = (NBP_path*)malloc(sizeof(NBP_path));
    paths[0].path->length = 1;
    paths[0].path->nodes = (NBP_node**)malloc(sizeof(NBP_node*)*paths[0].path->length);
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
        NBP_node* lastNode = current->path->nodes[current->path->length-1];
        if(lastNode == goal) {
            NBP_path* path = current->path;
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
            NBP_node* nextNode = lastNode->connections[j];
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
            exploredNodes = (NBP_node**)realloc(exploredNodes, sizeof(NBP_node*)*(exploredNodesSize+1));
            exploredNodesSize++;
            exploredNodes[exploredNodesSize-1] = nextNode;
            if(newPath) {
                paths = (struct path*)realloc(paths, sizeof(struct path)*(pathsSize+1));
                pathsSize++;
                current = paths;
                paths[pathsSize-1].path = (NBP_path*)malloc(sizeof(NBP_path));
                paths[pathsSize-1].path->length = current->path->length;
                paths[pathsSize-1].path->nodes = (NBP_node**)malloc(sizeof(NBP_node*)*paths[pathsSize-1].path->length);
                memcpy(paths[pathsSize-1].path->nodes, current->path->nodes, sizeof(NBP_node*)*paths[pathsSize-1].path->length);
                paths[pathsSize-1].nextNode = nextNode;
                paths[pathsSize-1].nextCost = (abs(nextNode->x-lastNode->x)+abs(nextNode->y-lastNode->y))/2;
                paths[pathsSize-1].cost = current->cost;
            }else {
                current->nextCost = (abs(nextNode->x-lastNode->x)+abs(nextNode->y-lastNode->y))/2;
                current->nextNode = nextNode;
                newPath = 1;
            }
        }
        current->path->nodes = (NBP_node**)realloc(current->path->nodes, sizeof(NBP_node*)*(current->path->length+1));
        current->path->length++;
        current->path->nodes[current->path->length-1] = current->nextNode;
        current->cost += current->nextCost;
    }
    return NULL;
}
