/*
 * brainfuck.c
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
#include <stdio.h>
#include "brainfuck.h"
#define size 30000

void BF_execute(const char* code) {
    char array[size] = {0};
    char* ptr = array;
    for(unsigned int i=0; code[i]; i++) {
        switch(code[i]) {
            case '>':
                ++ptr;
                break;
            case '<':
                --ptr;
                break;
            case '+':
                ++*ptr;
                break;
            case '-':
                --*ptr;
                break;
            case '.':
                putchar(*ptr);
                break;
            case ',':
                *ptr = getchar();
                break;
            case '[':
                if(!*ptr) {
                    int brackets = 1;
                    while(brackets > 0 && code[++i]) {
                        if(code[i] == '[') {
                            brackets++;
                        }
                        if(code[i] == ']') {
                            brackets--;
                        }
                    }
                }
                break;
            case ']':
                if(*ptr) {
                    int brackets = 1;
                    while(brackets > 0 && code[--i]) {
                        if(code[i] == '[') {
                            brackets--;
                        }
                        if(code[i] == ']') {
                            brackets++;
                        }
                    }
                }
                break;
        }
    }
}
void BF_executeFile(const char* filename) {
    char code[size+1] = {0};
    FILE* file = fopen(filename, "r");
    fread(code, sizeof(*code), size, file);
    fclose(file);
    BF_execute(code);
}
