/*
 * wad.h
 * humrcraft Source Code
 * Available on Github
 *
 * Copyright (c) 2017 Karol "digitcrusher" Łacina
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation nodes (the "Software"), to deal
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
#ifndef WAD_H
#define WAD_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WAD_IDENTIFICATION "HWAD"
#define WAD_VERSION "1.0.0   "

struct WAD_node {
    char name[256];
    unsigned int size;
    void* data;
};
struct WAD_wad {
    char identification[4];
    char version[8];
    unsigned int size;
    struct WAD_node** nodes;
};
struct WAD_node* WAD_createNode(const char* name, unsigned int size, const void* data);
void WAD_destroyNode(struct WAD_node* node);
struct WAD_node* WAD_loadFile(const char* filename);
bool WAD_extractFile(struct WAD_node* node);
struct WAD_wad* WAD_createWAD();
void WAD_destroyWAD(struct WAD_wad* wad);
void WAD_addNode(struct WAD_wad* wad, struct WAD_node* node);
struct WAD_node* WAD_getNode(struct WAD_wad* wad, const char* name);
bool WAD_saveWAD(struct WAD_wad* wad, const char* filename);
struct WAD_wad* WAD_loadWAD(const char* filename);
bool WAD_extractFiles(struct WAD_wad* wad);

#ifdef __cplusplus
}
#endif

#endif
