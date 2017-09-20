/*
 * module.h
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
#ifndef MODULE_H
#define MODULE_H

struct MOD_routine {
    char* name; //Name for the MOD_routine - useful for name searching.
    void (*func)(); //Pointer to the function of type void ().
};
struct MOD_module { //MOD_module contains MOD_routines.
    char* name; //Same for MOD_routine except the name is for the MOD_module.
    void* data; //Pointer to MOD_module's data which may contain useful data, might be NULL.
    int size; //Number of MOD_routines the MOD_module has.
    struct MOD_routine* rtns; //Pointer to the MOD_routines.
};
struct MOD_modboard { //MOD_modboard contains array of MOD_modules, stdmodbrd is the global MOD_modboard.
    int size; //Number of MOD_modules.
    struct MOD_module* array; //Pointer to the MOD_modules.
};

struct MOD_routine* MOD_createRoutine(const char* name, void (*func)()); //Creates a struct and returns a pointer to it
void MOD_destroyRoutine(struct MOD_routine* rtn); //Deallocate rtn.
void MOD_addRtn(struct MOD_routine rtn, struct MOD_module* mod); //Add rtn to mod
struct MOD_routine* MOD_getRtn(int x, struct MOD_module* mod); //Get MOD_routine x from mod.
struct MOD_routine* MOD_getRtnFromName(const char* x, struct MOD_module* mod);
struct MOD_module* MOD_createModule(const char* name, void* data);
void MOD_destroyModule(struct MOD_module* mod);
void MOD_addMod(struct MOD_module mod, struct MOD_modboard* brd);
struct MOD_module* MOD_getMod(int x, struct MOD_modboard* brd);
struct MOD_module* MOD_getModFromName(const char* x, struct MOD_modboard* brd);
struct MOD_modboard* MOD_createModBoard();
void MOD_destroyModBoard(struct MOD_modboard* brd);
void MOD_resetModBoard(struct MOD_modboard* brd);

extern struct MOD_modboard* MOD_stdmodbrd; //Global MOD_modboard

#endif
