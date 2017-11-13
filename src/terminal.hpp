/*
 * terminal.hpp
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
#ifndef TERMINAL_HPP
#define TERMINAL_HPP
#if defined(__linux__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#elif defined(_WIN32)
#include <windows.h>
#include <pthread.h>
#endif

//TODO: add colored text

namespace terminal {
    //Buffers
    #define OUTPUT 0
    #define INPUT 1
    //Flags
    #define IECHO 1
    #define N_UPDATE 2
    #define MOVE_OCUR 4
    #define CURSOR 8
    //Default values
    #if defined(__linux__)
    #define DEFAULT_FONT_WIDTH 5
    #define DEFAULT_FONT_HEIGHT 11
    #define DEFAULT_OFFSETX 0
    #define DEFAULT_OFFSETY 9
    #define DEFAULT_MARGINTOP 1
    #define DEFAULT_MARGINRIGHT 0
    #define DEFAULT_MARGINBOTTOM 0
    #define DEFAULT_MARGINLEFT 1
    #elif defined(_WIN32)
    #define DEFAULT_FONT_WIDTH 8
    #define DEFAULT_FONT_HEIGHT 15
    #define DEFAULT_OFFSETX 0
    #define DEFAULT_OFFSETY 0
    #define DEFAULT_MARGINTOP 0
    #define DEFAULT_MARGINRIGHT 0
    #define DEFAULT_MARGINBOTTOM 0
    #define DEFAULT_MARGINLEFT 0
    #endif
    #define DEFAULT_BUFF_WIDTH 80
    #define DEFAULT_BUFF_HEIGHT 25
    #define DEFAULT_FLAGS (IECHO | N_UPDATE | MOVE_OCUR | CURSOR)

    #define GET_IBUFF_CHAR(t, x, y) ( *((t)->ibuff+(x)+(y)*(t)->buffw) )
    #define GET_OBUFF_CHAR(t, x, y) ( *((t)->obuff+(x)+(y)*(t)->buffw) )
    #define GET_CURR_IBUFF_CHAR(t) ( GET_IBUFF_CHAR((t), (t)->icurx, (t)->icury) )
    #define GET_CURR_OBUFF_CHAR(t) ( GET_OBUFF_CHAR((t), (t)->ocurx, (t)->ocury) )
    #define GET_BUFF_BYTES(t) ( (t)->buffw*(t)->buffh )
    #define GET_BUFF_BITS(t) ( sizeof(char)*GET_BUFF_BYTES((t)) )
    #define GET_CHAR_X_COORD(t, x) ( (t)->offsetx+((x)+1)*(t)->marginleft+(x)*(t)->marginright+(x)*(t)->fontw )
    #define GET_CHAR_Y_COORD(t, y) ( (t)->offsety+((y)+1)*(t)->margintop+(y)*(t)->marginbottom+(y)*(t)->fonth )
    #define GET_TEXTAREA_WIDTH(t) ( ((t)->buffw)*((t)->marginright+(t)->marginleft)+(t)->buffw*(t)->fontw )
    #define GET_TEXTAREA_HEIGHT(t) ( ((t)->buffh)*((t)->margintop+(t)->marginbottom)+(t)->buffh*(t)->fonth )

    struct terminal {
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
        void (*close)(struct terminal*);
        void (*redraw)(struct terminal*);

        int buffw, buffh;
        char* ibuff;
        char* obuff;
        int icurx, icury;
        int ocurx, ocury;
        int flags;
    };

    struct terminal* createTerminal(int w, int h, int flags
                                  ,void (*close)(struct terminal*), void (*redraw)(struct terminal*));
    void destroyTerminal(struct terminal* term);
    void redrawTerminal(struct terminal* term);
    void updateTerminal(struct terminal* term); //Calls close when receives a destroy message
    void checkTerminal(struct terminal* term);
    void swritef(struct terminal* term, const char* fmt, ...); //Simillar to printf
    void swrite(struct terminal* term, const char* str);
    char* sread(struct terminal* term); //Blocks until enter AKA '\n' is pressed.
    void cwrite(struct terminal* term, char chr);
    char cread(struct terminal* term); //Blocks until key is pressed.
    void flush(struct terminal* term, bool buff);
    bool kbhit(struct terminal* term);
    void gotoxy(struct terminal* term, int x, int y);
}

#endif
