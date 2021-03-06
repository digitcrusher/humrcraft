/*
 * terminal.cpp
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
#ifndef _WIN32
#include "terminal.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "utils.hpp"

namespace terminal {
    #if defined(_WIN32)
    static unsigned int windowid=0;
    static LRESULT CALLBACK terminal_WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
        PAINTSTRUCT ps;
        switch(iMsg) {
            case WM_PAINT:
    		    BeginPaint(hwnd, &ps);
        		EndPaint(hwnd, &ps);
                return 0;
                break;
        }
        return DefWindowProc(hwnd, iMsg, wParam, lParam); //Dump remainning message that wasn't calculated
    }
    void* terminal_thread(void* term) {
        while(1) updateTerminal((struct terminal*)term);
        return NULL;
    }
    #endif
    struct terminal* createTerminal(int w, int h, int flags
                                  ,void (*close)(struct terminal*), void (*redraw)(struct terminal*)) {
        struct terminal* term = (struct terminal*)malloc(sizeof(struct terminal));
        term->fontw = DEFAULT_FONT_WIDTH;
        term->fonth = DEFAULT_FONT_HEIGHT;
        term->offsetx = DEFAULT_OFFSETX;
        term->offsety = DEFAULT_OFFSETY;
        term->margintop = DEFAULT_MARGINTOP;
        term->marginright = DEFAULT_MARGINRIGHT;
        term->marginleft = DEFAULT_MARGINLEFT;
        term->marginbottom = DEFAULT_MARGINBOTTOM;
        term->close = close;
        term->redraw = redraw;
        term->buffw = w;
        term->buffh = h;
        term->ibuff = (char*)malloc(GET_BUFF_BITS(term));
        term->obuff = (char*)malloc(GET_BUFF_BITS(term));
        term->icurx = 0;
        term->icury = 0;
        term->ocurx = 0;
        term->ocury = 0;
        term->flags = flags;
        memset(term->obuff, '\0', GET_BUFF_BYTES(term));
        memset(term->ibuff, '\0', GET_BUFF_BYTES(term));
    #if defined(__linux__)
        term->d = XOpenDisplay(NULL);
        if(!term->d) {
            return NULL;
        }
        term->s = DefaultScreen(term->d);
        term->w = XCreateSimpleWindow(term->d, RootWindow(term->d, term->s), 0, 0
                                    ,GET_TEXTAREA_WIDTH(term)
                                    ,GET_TEXTAREA_HEIGHT(term)
                                    ,1, WhitePixel(term->d, term->s), BlackPixel(term->d, term->s));
        XSetStandardProperties(term->d, term->w, "Terminal", "Terminal", None, NULL, 0, NULL);
        XSelectInput(term->d, term->w, StructureNotifyMask | ExposureMask | KeyPressMask);
        term->gc = XCreateGC(term->d, term->w, 0, 0);
        XSetBackground(term->d, term->gc, BlackPixel(term->d, term->s));
        XSetForeground(term->d, term->gc, WhitePixel(term->d, term->s));
        XClearWindow(term->d, term->w);
        XMapWindow(term->d, term->w);
    #elif defined(_WIN32)
        char* szAppName = utils::uitos(windowid++);
        WNDCLASSEX wndclass; //Temporary structure with window settings
        wndclass.cbSize        = sizeof(wndclass); //Size of WNDCLASSEX
        wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Style parameters
        wndclass.lpfnWndProc   = terminal_WndProc; //Pointer to WndProc which handles messages
        wndclass.cbClsExtra    = 0;
        wndclass.cbWndExtra    = 0;
        wndclass.hInstance     = GetModuleHandle(NULL);
        wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION); //Load icon for taskbar
        wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW); //Load cursor for window
        wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH); //Window background color
        wndclass.lpszMenuName  = NULL;
        wndclass.lpszClassName = szAppName; //Window ID
        wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION); //Load icon for title bar
        if(!RegisterClassEx(&wndclass)) { //Register wndclass structure
            return NULL;
        }
        term->hwnd = CreateWindow(szAppName, //Window ID
                            "Terminal", //Window title
                            WS_OVERLAPPEDWINDOW, //Window style
                            CW_USEDEFAULT, //Starting x position
                            CW_USEDEFAULT, //Starting y position
                            GET_TEXTAREA_WIDTH(term), //Starting width
                            GET_TEXTAREA_HEIGHT(term), //Starting height
                            NULL, //Handle to parent window
                            NULL, //Handle to parent window menu
                            GetModuleHandle(NULL), //Handle to window initiation argument
                            NULL
                            );
        if(!term->hwnd) {
            return NULL;
        }
        ShowWindow(term->hwnd, 5); //Show window
        UpdateWindow(term->hwnd); //Redraw window
        free(szAppName);
        pthread_create(&term->thread, NULL, terminal_thread, (void*)term);
    #endif
        return term;
    }
    void destroyTerminal(struct terminal* term) {
    #if defined(__linux__)
        XFreeGC(term->d, term->gc);
        XDestroyWindow(term->d, term->w);
        XCloseDisplay(term->d);
    #elif defined(_WIN32)
        pthread_cancel(term->thread);
        DestroyWindow(term->hwnd);
    #endif
        free(term->ibuff);
        free(term->obuff);
        free(term);
    }
    void redrawTerminal(struct terminal* term) {
    #if defined(__linux__)
        XWindowAttributes wa;
        XGetWindowAttributes(term->d, term->w, &wa);
        term->p = XCreatePixmap(term->d, term->w
                                ,GET_TEXTAREA_WIDTH(term)
                                ,GET_TEXTAREA_HEIGHT(term)
                                ,wa.depth);
        XSetBackground(term->d, term->gc, WhitePixel(term->d, term->s));
        XSetForeground(term->d, term->gc, BlackPixel(term->d, term->s));
        XFillRectangle(term->d, term->p, term->gc, 0, 0
                      ,GET_TEXTAREA_WIDTH(term)
                      ,GET_TEXTAREA_HEIGHT(term));
        for(int x=0; x<term->buffw; x++) {
            for(int y=0; y<term->buffh; y++) {
                char c[2];
                *c = GET_OBUFF_CHAR(term, x, y);
                *(c+1) = '\0';
                XSetBackground(term->d, term->gc, BlackPixel(term->d, term->s));
                XSetForeground(term->d, term->gc, WhitePixel(term->d, term->s));
                if(term->flags & CURSOR && x == term->ocurx && y == term->ocury) {
                    XFillRectangle(term->d, term->p, term->gc
                                  ,GET_CHAR_X_COORD(term, x)-term->offsetx-term->marginleft
                                  ,GET_CHAR_Y_COORD(term, y)-term->offsety-term->margintop
                                  ,term->fontw+term->marginleft+term->marginright
                                  ,term->fonth+term->margintop+term->marginbottom);
                    XSetForeground(term->d, term->gc, BlackPixel(term->d, term->s));
                    XDrawString(term->d, term->p, term->gc
                               ,GET_CHAR_X_COORD(term, x)
                               ,GET_CHAR_Y_COORD(term, y), c, strlen(c));
                    XSetForeground(term->d, term->gc, WhitePixel(term->d, term->s));
                }else {
                    XDrawString(term->d, term->p, term->gc
                               ,GET_CHAR_X_COORD(term, x)
                               ,GET_CHAR_Y_COORD(term, y), c, strlen(c));
                }
            }
        }
        if(term->redraw != NULL) {
            term->redraw(term);
        }
        XCopyArea(term->d, term->p, term->w, term->gc, 0, 0
                 ,GET_TEXTAREA_WIDTH(term)
                 ,GET_TEXTAREA_HEIGHT(term), 0, 0);
        XFreePixmap(term->d, term->p);
    #elif defined(_WIN32)
        RECT rect;
        GetClientRect(term->hwnd, &rect);
        HDC hdcTemp = GetDC(term->hwnd);
        term->hdc = CreateCompatibleDC(hdcTemp);
        HBITMAP hbmMem = CreateCompatibleBitmap(hdcTemp, rect.right-rect.left, rect.bottom-rect.top);
        HGDIOBJ hbmOld = SelectObject(term->hdc, hbmMem);
        HGDIOBJ font = CreateFont(0, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET
                    ,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Fixedsys");
        SelectObject(term->hdc, font);
        for(int x=0; x<term->buffw; x++) {
            for(int y=0; y<term->buffh; y++) {
                char c[2];
                *c = GET_OBUFF_CHAR(term, x, y);
                *(c+1) = '\0';
                if(term->flags & CURSOR && x == term->ocurx && y == term->ocury) {
                    SetDCPenColor(term->hdc, RGB(255, 255, 255));
                    RECT rect = {GET_CHAR_X_COORD(term, x)-term->offsetx-term->marginleft
                                ,GET_CHAR_Y_COORD(term, y)-term->offsety-term->margintop
                                ,GET_CHAR_X_COORD(term, x+1)-term->offsetx-term->marginleft
                                ,GET_CHAR_Y_COORD(term, y+1)-term->offsety-term->margintop};
                    FillRect(term->hdc, &rect, (HBRUSH)GetStockObject(DC_PEN));
                    SetTextColor(term->hdc, RGB(0, 0, 0));
                    SetBkColor(term->hdc, RGB(255, 255, 255));
                    TextOut(term->hdc, GET_CHAR_X_COORD(term, x)
                           ,GET_CHAR_Y_COORD(term, y), c, strlen(c));
                }else {
                    SetTextColor(term->hdc, RGB(255, 255, 255));
                    SetBkColor(term->hdc, RGB(0, 0, 0));
                    TextOut(term->hdc, GET_CHAR_X_COORD(term, x)
                           ,GET_CHAR_Y_COORD(term, y), c, strlen(c));
                }
            }
        }
        if(term->redraw != NULL) {
            term->redraw(term);
        }
        BitBlt(hdcTemp,rect.left,rect.top,rect.right-rect.left, rect.bottom-rect.top,term->hdc,0,0,SRCCOPY);
        SelectObject(term->hdc, hbmOld);
        DeleteObject(hbmMem);
        DeleteObject(font);
        DeleteDC(term->hdc);
    #endif
    }
    void updateTerminal(struct terminal* term) {
    #if defined(__linux__)
        XEvent e;
        while(XPending(term->d)) {
            XNextEvent(term->d, &e);
            switch(e.type) {
                case Expose:
                    redrawTerminal(term);
                    break;
                case KeyPress:
                    char buff[16];
                    KeySym ks;
                    XLookupString(&e.xkey, buff, 16, &ks, NULL);
                    if(*buff == '\0') {
                        switch(ks) {
                            case XK_Up:
                                --term->icury;
                                if(term->flags & MOVE_OCUR) {
                                    --term->ocury;
                                }
                                break;
                            case XK_Left:
                                --term->icurx;
                                if(term->flags & MOVE_OCUR) {
                                    --term->ocurx;
                                }
                                break;
                            case XK_Down:
                                if(GET_CURR_IBUFF_CHAR(term) != '\0') {
                                    ++term->icury;
                                }
                                if(term->flags & MOVE_OCUR) {
                                    ++term->ocury;
                                }
                                break;
                            case XK_Right:
                                if(GET_CURR_IBUFF_CHAR(term) != '\0') {
                                    ++term->icurx;
                                }
                                if(term->flags & MOVE_OCUR) {
                                    ++term->ocurx;
                                }
                                break;
                        }
                        checkTerminal(term);
                        break;
                    }
                    switch(*buff) {
                        case '\b':
                            --term->icurx;
                            GET_CURR_IBUFF_CHAR(term) = '\0';
                            break;
                        default:
                            GET_CURR_IBUFF_CHAR(term) = *buff;
                            ++term->icurx;
                            break;
                    }
                    if(term->flags & IECHO) {
                        cwrite(term, *buff);
                    }
                    checkTerminal(term);
                    break;
                case DestroyNotify:
                    if(term->close != NULL) {
                        term->close(term);
                    }
                    break;
            }
        }
        redrawTerminal(term);
    #elif defined(_WIN32)
        UpdateWindow(term->hwnd);
        MSG msg;
        while(PeekMessage(&msg, term->hwnd, 0, 0, 1)) {
            TranslateMessage(&msg);
            switch(msg.message) {
                case WM_QUIT:
                    exit(0);
                    break;
                case WM_DESTROY:
                case WM_CLOSE:
                    if(term->close != NULL) {
                        term->close(term);
                    }
                    break;
                case WM_CREATE:
                    break;
                case WM_CHAR:
                    switch(msg.wParam) {
                        case '\b':
                            --term->icurx;
                            GET_CURR_IBUFF_CHAR(term) = '\0';
                            break;
                        default:
                            GET_CURR_IBUFF_CHAR(term) = msg.wParam;
                            ++term->icurx;
                            break;
                    }
                    if(term->flags & IECHO) {
                        cwrite(term, msg.wParam);
                    }
                    checkTerminal(term);
                    break;
                case WM_KEYDOWN:
                    switch(msg.wParam) {
                        case VK_UP:
                            --term->icury;
                            if(term->flags & MOVE_OCUR) {
                                --term->ocury;
                            }
                            break;
                        case VK_LEFT:
                            --term->icurx;
                            if(term->flags & MOVE_OCUR) {
                                --term->ocurx;
                            }
                            break;
                        case VK_DOWN:
                            if(GET_CURR_IBUFF_CHAR(term) != '\0') {
                                ++term->icury;
                            }
                            if(term->flags & MOVE_OCUR) {
                                ++term->ocury;
                            }
                            break;
                        case VK_RIGHT:
                            if(GET_CURR_IBUFF_CHAR(term) != '\0') {
                                ++term->icurx;
                            }
                            if(term->flags & MOVE_OCUR) {
                                ++term->ocurx;
                            }
                        break;
                    }
                    checkTerminal(term);
                    break;
                default:
                    DispatchMessage(&msg);
                    break;
            }
        }
        redrawTerminal(term);
    #endif
    }
    void checkTerminal(struct terminal* term) {
    #if defined(__linux__)
        XResizeWindow(term->d, term->w, GET_TEXTAREA_WIDTH(term), GET_TEXTAREA_HEIGHT(term));
    #elif defined(_WIN32)
    #endif
        if(term->ocurx < 0) {
            term->ocurx = term->buffw-1;
            --term->ocury;
        }
        if(term->ocury < 0) {
            term->ocurx = 0;
            term->ocury = 0;
        }
        if(term->ocurx >= term->buffw) {
            term->ocurx = 0;
            ++term->ocury;
        }
        if(term->ocury >= term->buffh) {
            --term->ocury;
            memmove(term->obuff, term->obuff+term->buffw, GET_BUFF_BYTES(term)-term->buffw);
            memset(&GET_CURR_OBUFF_CHAR(term), '\0', term->buffw);
        }
        if(term->icurx < 0) {
            term->icurx = term->buffw-1;
            --term->icury;
        }
        if(term->icury < 0) {
            term->icurx = 0;
            term->icury = 0;
        }
        if(term->icurx >= term->buffw) {
            term->icurx = 0;
            ++term->icury;
        }
        if(term->icury >= term->buffh) {
            --term->icury;
            memmove(term->ibuff, term->ibuff+term->buffw, GET_BUFF_BYTES(term)-term->buffw);
            memset(&GET_CURR_IBUFF_CHAR(term), '\0', term->buffw);
        }
    }
    void swritef(struct terminal* term, const char* fmt, ...) {
        va_list vl;
        va_start(vl, fmt);
        int written=0;
        for(unsigned int i=0; i<strlen(fmt); i++) {
            switch(fmt[i]) {
                case '%':
                    ++i;
                    if(!(i<strlen(fmt))) {
                        return;
                    }
                    switch(fmt[i]) {
                        case 'd':
                        case 'i': {
                            int val=va_arg(vl, int);
                            char* str=utils::itos(val);
                            swrite(term, str);
                            written += strlen(str);
                            } break;
                        case 'u': {
                            unsigned int val=va_arg(vl, unsigned int);
                            char* str=utils::uitos(val);
                            swrite(term, str);
                            written += strlen(str);
                            } break;
                        case 'f':
                        case 'F': {
                            double val=va_arg(vl, double);
                            char* str=utils::ftos(val);
                            swrite(term, str);
                            written += strlen(str);
                            } break;
                        case 'c': {
                            char val=va_arg(vl, int);
                            cwrite(term, val);
                            ++written;
                            } break;
                        case 's': {
                            char* val=va_arg(vl, char*);
                            swrite(term, val);
                            written += strlen(val);
                            } break;
                        case 'n': {
                            int* val=va_arg(vl, int*);
                            *val = written;
                            } break;
                        case '%': {
                            cwrite(term, fmt[i]);
                            ++written;
                            } break;
                    }
                    break;
                default:
                    cwrite(term, fmt[i]);
                    ++written;
                    break;
            }
        }
        va_end(vl);
    }
    void swrite(struct terminal* term, const char* str) {
        for(unsigned int i=0; i<strlen(str); i++) {
            cwrite(term, str[i]);
        }
    }
    char* sread(struct terminal* term) {
        int len;
        char chr;
        updateTerminal(term);
        for(len = 0; !(*(term->ibuff+len) == '\n' || *(term->ibuff+len) == '\r' || *(term->ibuff+len) == '\0'); len++);
        if(*(term->ibuff+len) == '\n' && *(term->ibuff+len) == '\r') {
            goto getter;
        }
        chr = GET_CURR_IBUFF_CHAR(term);
        while(chr != '\n' && chr != '\r') {
            updateTerminal(term);
            chr = *(&GET_CURR_IBUFF_CHAR(term)-1);
        }
        len = term->icurx+term->icury*term->buffw;
        getter:
        char* str = (char*)malloc(sizeof(char)*len);
        for(int i=0; i<len; i++) {
            *(str+i) = cread(term);
        }
        *(str+len-1) = '\0';
        return str;
    }
    void cwrite(struct terminal* term, char chr) {
        switch(chr) {
            case '\r':
            case '\n':
                term->ocurx = 0;
                ++term->ocury;
                if(term->flags & N_UPDATE) {
                    updateTerminal(term);
                }
                break;
            case '\b':
                --term->ocurx;
                GET_CURR_OBUFF_CHAR(term) = '\0';
                break;
            default:
                GET_CURR_OBUFF_CHAR(term) = chr;
                ++term->ocurx;
                break;
        }
        checkTerminal(term);
    }
    char cread(struct terminal* term) {
        while(*term->ibuff == '\0') {
            updateTerminal(term);
        }
        char chr = *term->ibuff;
        --term->icurx;
        checkTerminal(term);
        memmove(term->ibuff, term->ibuff+1, GET_BUFF_BYTES(term)-1);
        GET_IBUFF_CHAR(term, term->buffw-1, term->buffh-1) = '\0';
        return chr;
    }
    void flush(struct terminal* term, bool buff) {
        if(buff) {
            memset(term->ibuff, '\0', GET_BUFF_BYTES(term));
            term->icurx = 0;
            term->icury = 0;
        }else {
            memset(term->obuff, '\0', GET_BUFF_BYTES(term));
            term->ocurx = 0;
            term->ocury = 0;
        }
    }
    bool kbhit(struct terminal* term) {
        return *term->ibuff;
    }
    void gotoxy(struct terminal* term, int x, int y) {
        term->ocurx = x;
        term->ocury = y;
    }
}
#endif
