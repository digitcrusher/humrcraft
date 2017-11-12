/*
 * digitcrusher's Public License
 *
 * wadArchivizer - a CLI software used for archivizing files in the wad format
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
#include <string.h>
#include "wad.h"

int main(int argc, char** argv) {
    for(int i=1; i<argc; i++) {
        if(!strcmp(argv[i], "create")) {
            if(!(i+1 < argc)) {
                return 1;
            }
            i++;
            struct WAD_wad* wad = WAD_createWAD();
            WAD_saveWAD(wad, argv[i]);
            WAD_destroyWAD(wad);
        }else if(!strcmp(argv[i], "add")) {
            if(!(i+2 < argc)) {
                return 1;
            }
            i++;
            char* wadname = argv[i];
            struct WAD_wad* wad = WAD_loadWAD(wadname);
            i++;
            WAD_addNode(wad, WAD_loadFile(argv[i]));
            WAD_saveWAD(wad, wadname);
            WAD_destroyWAD(wad);
        }else if(!strcmp(argv[i], "extract")) {
            if(!(i+1 < argc)) {
                return 1;
            }
            i++;
            struct WAD_wad* wad = WAD_loadWAD(argv[i]);
            WAD_extractFiles(wad);
            WAD_destroyWAD(wad);
        }
    }
    return 0;
}
