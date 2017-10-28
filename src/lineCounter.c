/*
 * digitcrusher's Public License
 *
 * lineCounter - a CLI software used for counting lines in files located in a directory
 * usage: lineCounter <dirpath>
 *
 * Copyright (C) 2017 Karol "digitcrusher" Łacina
 *
 * "The Software" is this software and the associated documentation files.
 * "The License" is this license and its permissions, restrictions and conditions.
 *
 * This is "Free Software" which means that anyone obtaining a copy of The Software
 * are permitted, free of charge, to use, copy, modify, merge, publish, distribute,
 * sublicense, and sell copies of The Software.
 * You must include The License in all copies or portions of The Software.
 * You may add your own permissions and conditions in all copies of The License
 * if they don't conflict with the above.
 *
 * THE SOFTWARE IS PROVIDED WITHOUT WARRANTY OF ANY KIND, AUTHORS SHALL NOT
 * BE LIABLE FOR ANY HARMS DONE BY THE SOFTWARE.
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>

char* scat(char* str, const char* str1, bool freestr) {
    if(!str || !str1) {
        return NULL;
    }
    char* buffer = (char*)malloc(sizeof(char)*(strlen(str)+strlen(str1)+1));
    memcpy(buffer, str, strlen(str));
    memcpy(buffer+strlen(str), str1, strlen(str1)+1);
    if(freestr) {
        free(str);
    }
    return buffer;
}
int countLines(char* dirpath) {
    int lines=0;
    DIR* dir;
    if(!(dir=opendir(dirpath))) {
        return 0;
    }
    struct dirent* dirent;
    while((dirent=readdir(dir))) {
        char* path=scat(dirpath, "/", 0);
        path = scat(path, dirent->d_name, 1);
        if(opendir(path)) {
            if(!strcmp(dirent->d_name, ".") || !strcmp(dirent->d_name, "..")) {
                free(path);
                continue;
            }
            printf("Dir: \"%s\"\n", path);
            lines += countLines(path);
        }else {
            printf("File: \"%s\"\n", path);
            char* buffer;
            size_t size;
            FILE* file;
            if(!(file = fopen(path, "r"))) {
                printf("File opening error, file: \"%s\"\n", dirent->d_name);
                continue;
            }
            fseek(file, 0, SEEK_END);
            size = ftell(file);
            rewind (file);
            if(!(buffer = (char*)malloc(sizeof(char)*size))) {
                printf("Memory allocating error, file: \"%s\"\n", dirent->d_name);
                fclose(file);
                continue;
            }
            if(!fread(buffer, sizeof(char), size, file)) {
                printf("File reading error, file: \"%s\"\n", dirent->d_name);
                free(buffer);
                fclose(file);
                continue;
            }
            for(int i=0; i<size; i++) {
                if(buffer[i] == '\n') {
                    lines++;
                }
            }
            free(buffer);
            fclose(file);
        }
        free(path);
    }
    closedir(dir);
    return lines;
}
int main(int argc, char** argv) {
    for(int i=1; i<argc; i++) {
        if(!strcmp(argv[i], "count")) {
            if(!(i+1 < argc)) {
                return 1;
            }
            i++;
            int lines = countLines(argv[i]);
            FILE* file;
            file = fopen("./lineCounter.cfg", "r");
            int lastlines = 0;
            fread(&lastlines, sizeof(int), 1, file);
            fclose(file);
            printf("Number of lines now: %i\nNumber of lines earlier: %i\nNumber of lines added: %i\n", lines, lastlines, lines-lastlines);
        }else if(!strcmp(argv[i], "reset")) {
            if(!(i+1 < argc)) {
                return 1;
            }
            i++;
            FILE* file;
            file = fopen("./lineCounter.cfg", "w");
            int lines = countLines(argv[i]);
            fwrite(&lines, sizeof(int), 1, file);
            fclose(file);
            printf("Number of lines: %i\n", lines);
        }
    }
    return 0;
}
