/*
 * gui.hpp
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
#ifndef GUI_HPP
#define GUI_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "utils.hpp"

//TODO: implement styles and more widgets

enum States {
    None,
    Press,
    Release,
    Hover
};
class GUI_Widget {
    public:
        Vector<const char*> family;
        float time;
        void (*eventListener)(int state); //type - one from States
        bool enable;
        bool visible;
        int state;
        SDL_Color bgcolor;
        SDL_Color fgcolor;
        int x, y;
        int w, h;
        GUI_Widget();
        virtual ~GUI_Widget();
        virtual GUI_Widget* setTime(float time);
        virtual GUI_Widget* setEventListener(void (*eventListener)(int state));
        virtual GUI_Widget* setEnabled(bool enable);
        virtual GUI_Widget* setVisibility(bool visible);
        virtual GUI_Widget* setState(int state);
        virtual GUI_Widget* setBGColor(SDL_Color bgcolor);
        virtual GUI_Widget* setFGColor(SDL_Color fgcolor);
        virtual GUI_Widget* setPosition(int x, int y);
        virtual GUI_Widget* setSize(int w, int h);
        virtual GUI_Widget* setBounds(int x, int y, int w, int h);
        virtual void update(float delta);
        virtual void render(SDL_Surface* surface);
        virtual void processEvent(SDL_Event event);
};
class GUI_Frame : public GUI_Widget {
    public:
        Vector<GUI_Widget*> widgets;
        GUI_Frame();
        virtual ~GUI_Frame();
        virtual void update(float delta);
        virtual void render(SDL_Surface* surface);
        virtual void add(GUI_Widget* widget);
        virtual void processEvent(SDL_Event event);
};
class GUI_Label : public GUI_Widget {
    public:
        TTF_Font* font;
        const char* text;
        GUI_Label(TTF_Font* font, const char* text);
        virtual ~GUI_Label();
        virtual GUI_Label* setFont(TTF_Font* font);
        virtual GUI_Label* setText(const char* text);
        virtual void render(SDL_Surface* surface);
};
class GUI_Button : public GUI_Frame {
    public:
        GUI_Button();
        virtual ~GUI_Button();
        virtual void render(SDL_Surface* surface);
};
class GUI_ProgressBar : public GUI_Widget {
    public:
        float progress;
        GUI_ProgressBar();
        virtual ~GUI_ProgressBar();
        virtual GUI_ProgressBar* setProgress(float progress);
        virtual void render(SDL_Surface* surface);
};

#endif
