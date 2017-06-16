/*
 * brainfuck.cpp
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
#include <stdio.h>
#include "brainfuck.h"
#define size 30000

void execute(const char* code, int type) {
    char array[size] = {0};
    char storage = 0;
    char* ptr = array;
    for(unsigned int i=0; code[i] && code[i] != '@'; i++) {
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
        if(type == ExType1) { //TODO: logical shift
            switch(code[i]) {
                case '$':
                    storage = *ptr;
                    break;
                case '!':
                    *ptr = storage;
                    break;
                case '{':
                    *ptr >>= 1;
                    break;
                case '}':
                    *ptr <<= 1;
                    break;
                case '~':
                    *ptr = ~*ptr;
                    break;
                case '^':
                    *ptr ^= storage;
                    break;
                case '&':
                    *ptr &= storage;
                    break;
                case '|':
                    *ptr |= storage;
                    break;
            }
        }
    }
}
void executeFile(const char* filename, int type) {
    char code[size+1] = {0};
    FILE* file = fopen(filename, "r");
    fread(code, sizeof(*code), size, file);
    fclose(file);
    execute(code, type);
}
