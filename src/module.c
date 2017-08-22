/*
 * MOD_module.c
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
#include <stdlib.h> //malloc, realloc
#include <string.h> //strlen, strcpy, strcmp
#include "module.h"

struct MOD_modboard* MOD_stdmodbrd;

struct MOD_routine* MOD_createRoutine(const char* name, void (*func)()) {
    struct MOD_routine* rtn = (struct MOD_routine*)malloc(sizeof(struct MOD_routine)); //Allocate memory for rtn->
    rtn->name = (char*)malloc(sizeof(char)*strlen(name)); //Allocate memory for name.
    strcpy(rtn->name, name); //Copy name to rtn->name.
    rtn->func = func; //Set the pointer.
    return rtn;
}
void MOD_destroyRoutine(struct MOD_routine* rtn) {
    free(rtn->name);
    free(rtn);
}
void MOD_addRtn(struct MOD_routine rtn, struct MOD_module* mod) {
    mod->size++; //Update the number of MOD_modules.
    mod->rtns = (struct MOD_routine*)realloc((void*)mod->rtns, sizeof(struct MOD_routine)*mod->size); //Reallocate MOD_routines.
    *(mod->rtns+mod->size-1) = rtn; //Copy rtn to the back of the array.
}
struct MOD_routine* MOD_getRtn(int x, struct MOD_module* mod) {
    if(x>=0 && x<mod->size) { //Is x in the bounds of the array?
        return mod->rtns+x; //Yes, return MOD_routine x.
    }
    return NULL; //No, return NULL.
}
struct MOD_routine* MOD_getRtnFromName(const char* x, struct MOD_module* mod) {
    for(int i=0; i>=0 && i<mod->size; i++) { //Iterate over the MOD_routines.
        if(!strcmp((mod->rtns+i)->name, x)) { //Do the names match?
            return mod->rtns+i; //Yes, return the matching MOD_routine.
        }
    }
    return NULL; //No, return NULL.
}
struct MOD_module* MOD_createModule(const char* name, void* data) {
    struct MOD_module* mod = (struct MOD_module*)malloc(sizeof(struct MOD_module));
    mod->name = (char*)malloc(sizeof(char)*strlen(name));
    strcpy(mod->name, name); //Copy name to mod->name.
    mod->data = data;
    mod->size = 0; //No MOD_routines at the time of the creation.
    mod->rtns = (struct MOD_routine*)malloc(sizeof(struct MOD_routine)*mod->size);
    return mod;
}
void MOD_destroyModule(struct MOD_module* mod) {
    free(mod->name);
    free(mod->data);
    free(mod->rtns);
    free(mod);
}
void MOD_addMod(struct MOD_module mod, struct MOD_modboard* brd) {
    brd->size++;
    brd->array = (struct MOD_module*)realloc((void*)brd->array, sizeof(struct MOD_module)*brd->size); //Reallocate.
    *(brd->array+brd->size-1) = mod; //Copy.
}
struct MOD_module* MOD_getMod(int x, struct MOD_modboard* brd) {
    if(x>=0 && x<brd->size) {
        return brd->array+x;
    }
    return NULL;
}
struct MOD_module* MOD_getModFromName(const char* x, struct MOD_modboard* brd) {
    for(int i=0; i>=0 && i<brd->size; i++) {
        if(!strcmp((brd->array+i)->name, x)) {
            return brd->array+i;
        }
    }
    return NULL;
}
struct MOD_modboard* MOD_createModBoard() {
    struct MOD_modboard* brd = (struct MOD_modboard*)malloc(sizeof(struct MOD_modboard));
    brd->size = 0;
    brd->array = (struct MOD_module*)malloc(sizeof(struct MOD_module)*brd->size);
    return brd;
}
void MOD_destroyModBoard(struct MOD_modboard* brd) {
    free(brd->array);
    free(brd);
}
void MOD_resetModBoard(struct MOD_modboard* brd) {
    brd->size = 0;
    free(brd->array);
    brd->array = (struct MOD_module*)malloc(sizeof(struct MOD_module)*brd->size);
}
