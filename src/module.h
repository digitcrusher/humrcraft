/*
 * module.h
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
