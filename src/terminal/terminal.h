/*
 * terminal.h
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
#ifndef KAROLSLIB_TERMINAL_H
#define KAROLSLIB_TERMINAL_H
#if defined(__linux__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#elif defined(_WIN32)
#include <windows.h>
#include <pthread.h>
#endif

#define TERMINAL_GET_IBUFF_CHAR(t, x, y) ( *((t)->ibuff+(x)+(y)*(t)->buffw) )
#define TERMINAL_GET_OBUFF_CHAR(t, x, y) ( *((t)->obuff+(x)+(y)*(t)->buffw) )
#define TERMINAL_GET_CURR_IBUFF_CHAR(t) ( TERMINAL_GET_IBUFF_CHAR((t), (t)->icurx, (t)->icury) )
#define TERMINAL_GET_CURR_OBUFF_CHAR(t) ( TERMINAL_GET_OBUFF_CHAR((t), (t)->ocurx, (t)->ocury) )
#define TERMINAL_GET_BUFF_BYTES(t) ( (t)->buffw*(t)->buffh )
#define TERMINAL_GET_BUFF_BITS(t) ( sizeof(char)*TERMINAL_GET_BUFF_BYTES((t)) )
#define TERMINAL_GET_CHAR_X_COORD(t, x) ( (t)->offsetx+((x)+1)*(t)->marginleft+(x)*(t)->marginright+(x)*(t)->fontw )
#define TERMINAL_GET_CHAR_Y_COORD(t, y) ( (t)->offsety+((y)+1)*(t)->margintop+(y)*(t)->marginbottom+(y)*(t)->fonth )
#define TERMINAL_GET_TEXTAREA_WIDTH(t) ( ((t)->buffw)*((t)->marginright+(t)->marginleft)+(t)->buffw*(t)->fontw )
#define TERMINAL_GET_TEXTAREA_HEIGHT(t) ( ((t)->buffh)*((t)->margintop+(t)->marginbottom)+(t)->buffh*(t)->fonth )

//Buffers
#define TERMINAL_OUTPUT 0
#define TERMINAL_INPUT 1
//Flags
#define TERMINAL_IECHO 1
#define TERMINAL_N_UPDATE 2
#define TERMINAL_MOVE_OCUR 4
#define TERMINAL_CURSOR 8
//Default values
#if defined(__linux__)
#define TERMINAL_DEFAULT_FONT_WIDTH 5
#define TERMINAL_DEFAULT_FONT_HEIGHT 11
#define TERMINAL_DEFAULT_OFFSETX 0
#define TERMINAL_DEFAULT_OFFSETY 9
#define TERMINAL_DEFAULT_MARGINTOP 1
#define TERMINAL_DEFAULT_MARGINRIGHT 0
#define TERMINAL_DEFAULT_MARGINBOTTOM 0
#define TERMINAL_DEFAULT_MARGINLEFT 1
#elif defined(_WIN32)
#define TERMINAL_DEFAULT_FONT_WIDTH 8
#define TERMINAL_DEFAULT_FONT_HEIGHT 15
#define TERMINAL_DEFAULT_OFFSETX 0
#define TERMINAL_DEFAULT_OFFSETY 0
#define TERMINAL_DEFAULT_MARGINTOP 0
#define TERMINAL_DEFAULT_MARGINRIGHT 0
#define TERMINAL_DEFAULT_MARGINBOTTOM 0
#define TERMINAL_DEFAULT_MARGINLEFT 0
#endif
#define TERMINAL_DEFAULT_BUFF_WIDTH 80
#define TERMINAL_DEFAULT_BUFF_HEIGHT 25
#define TERMINAL_DEFAULT_FLAGS (TERMINAL_IECHO | TERMINAL_N_UPDATE | TERMINAL_MOVE_OCUR | TERMINAL_CURSOR)

struct KL_terminal {
#if defined(__linux__)
    Display* d;
    Window w;
    GC gc;
    Pixmap p;
    int s;
#elif defined(_WIN32)
    HWND hwnd;
    HDC hdc;
    pthread_t thread;
#endif
    int fontw, fonth;
    int offsetx, offsety;
    int margintop, marginright, marginbottom, marginleft;
    void (*close)(KL_terminal*);
    void (*redraw)(KL_terminal*);

    int buffw, buffh;
    char* ibuff;
    char* obuff;
    int icurx, icury;
    int ocurx, ocury;
    int flags;
};

KL_terminal* KL_createTerminal(int w, int h, int flags
                              ,void (*close)(KL_terminal*), void (*redraw)(KL_terminal*));
void KL_destroyTerminal(KL_terminal* term);
void KL_redrawTerminal(KL_terminal* term);
void KL_updateTerminal(KL_terminal* term); //Calls close when received a destroy message
void KL_checkTerminal(KL_terminal* term);
void KL_swritef(KL_terminal* term, char* fmt, ...); //Simillar to printf
void KL_swrite(KL_terminal* term, const char* str);
char* KL_sread(KL_terminal* term); //Blocks until enter AKA '\n' is pressed.
void KL_cwrite(KL_terminal* term, char chr);
char KL_cread(KL_terminal* term); //Blocks until key is pressed.
void KL_flush(KL_terminal* term, bool buff);
bool KL_kbhit(KL_terminal* term);
void KL_gotoxy(KL_terminal* term, int x, int y);

extern KL_terminal* KL_stdterm; //Global terminal

#endif
