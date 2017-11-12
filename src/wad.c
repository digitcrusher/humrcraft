/*
 * wad.c
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
#include <stdio.h>
#include <string.h>
#include "wad.h"

struct WAD_node* WAD_createNode(const char* name, unsigned int size, const void* data) {
    struct WAD_node* node = (struct WAD_node*)malloc(sizeof(struct WAD_node));
    for(unsigned int i=0; i<255; i++) {
        if(i < strlen(name)) {
            node->name[i] = name[i];
        }else {
            node->name[i] = '\0';
        }
    }
    node->name[255] = '\0';
    node->size = size;
    node->data = malloc(node->size);
    memcpy(node->data, data, node->size);
    return node;
}
void WAD_destroyNode(struct WAD_node* node) {
    free(node->data);
    free(node);
}
struct WAD_node* WAD_loadFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if(!file) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);
    rewind(file);
    char* data = (char*)malloc(sizeof(char)*size);
    fread(data, sizeof(char), size, file);
    fclose(file);
    return WAD_createNode(filename, size, data);
}
bool WAD_extractFile(struct WAD_node* node) {
    FILE* file = fopen(node->name, "w");
    if(!file) {
        return 1;
    }
    fwrite(node->data, sizeof(char), node->size, file);
    fclose(file);
    return 0;
}
struct WAD_wad* WAD_createWAD() {
    struct WAD_wad* wad = (struct WAD_wad*)malloc(sizeof(struct WAD_wad));
    memcpy(wad->identification, WAD_IDENTIFICATION, 4);
    memcpy(wad->version, WAD_VERSION, 8);
    wad->size = 0;
    wad->nodes = (struct WAD_node**)malloc(sizeof(struct WAD_node*)*wad->size);
    return wad;
}
void WAD_destroyWAD(struct WAD_wad* wad) {
    for(unsigned int i=0; i<wad->size; i++) {
        WAD_destroyNode(wad->nodes[i]);
    }
    free(wad);
}
void WAD_addNode(struct WAD_wad* wad, struct WAD_node* node) {
    wad->size++;
    wad->nodes = (struct WAD_node**)realloc(wad->nodes, sizeof(struct WAD_node*)*wad->size);
    wad->nodes[wad->size-1] = node;
}
struct WAD_node* WAD_getNode(struct WAD_wad* wad, const char* name) {
    for(unsigned int i=0; i<wad->size; i++) {
        bool error=0;
        for(unsigned int j=0; j<256; j++) {
            if(wad->nodes[i]->name[j] == '\0' && name[j] == '\0') {
                break;
            }
            if(wad->nodes[i]->name[j] != name[j]) {
                error = 1;
                break;
            }
        }
        if(!error) {
            return wad->nodes[i];
        }
    }
    return NULL;
}
bool WAD_saveWAD(struct WAD_wad* wad, const char* filename) {
    FILE* file = fopen(filename, "w");
    if(!file) {
        return 1;
    }
    fwrite(wad, sizeof(char), sizeof(struct WAD_wad)-sizeof(struct WAD_node**), file);
    for(unsigned int i=0; i<wad->size; i++) {
        fwrite(wad->nodes[i], sizeof(char), sizeof(struct WAD_node)-sizeof(void*), file);
        fwrite(wad->nodes[i]->data, sizeof(char), wad->nodes[i]->size, file);
    }
    fclose(file);
    return 0;
}
struct WAD_wad* WAD_loadWAD(const char* filename) {
    FILE* file = fopen(filename, "r");
    if(!file) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file)-(sizeof(struct WAD_wad)-sizeof(struct WAD_node*));
    if(size < 0) {
        fclose(file);
        return NULL;
    }
    rewind(file);
    struct WAD_wad metadata;
    fread(&metadata, sizeof(char), sizeof(struct WAD_wad)-sizeof(struct WAD_node*), file);
    if(memcmp(metadata.identification, WAD_IDENTIFICATION, sizeof(char)*4) || memcmp(metadata.version, WAD_VERSION, sizeof(char)*8)) {
        fclose(file);
        return NULL;
    }
    struct WAD_wad* wad = (struct WAD_wad*)malloc(sizeof(struct WAD_wad));
    *wad = metadata;
    wad->nodes = (struct WAD_node**)malloc(sizeof(struct WAD_node*)*wad->size);
    for(unsigned int i=0; i<wad->size; i++) {
        struct WAD_node* node = WAD_createNode("", 0, NULL);
        fread(node, sizeof(char), sizeof(struct WAD_node)-sizeof(void*), file);
        node->data = malloc(node->size);
        fread(node->data, sizeof(char), node->size, file);
        wad->nodes[i] = node;
    }
    fclose(file);
    return wad;
}
bool WAD_extractFiles(struct WAD_wad* wad) {
    for(unsigned int i=0; i<wad->size; i++) {
        if(WAD_extractFile(wad->nodes[i])) {
            return 1;
        }
    }
    return 0;
}
