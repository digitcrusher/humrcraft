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

namespace GUI {
    enum States {
        None,
        Press,
        Release,
        Hover
    };
    class Widget {
        public:
            utils::Vector<const char*> family;
            float time;
            void (*eventListener)(int state); //type - one from States
            bool enable;
            bool visible;
            int state;
            SDL_Color bgcolor;
            SDL_Color fgcolor;
            int x, y;
            int w, h;
            Widget();
            virtual ~Widget();
            virtual Widget* setTime(float time);
            virtual Widget* setEventListener(void (*eventListener)(int state));
            virtual Widget* setEnabled(bool enable);
            virtual Widget* setVisibility(bool visible);
            virtual Widget* setState(int state);
            virtual Widget* setBGColor(SDL_Color bgcolor);
            virtual Widget* setFGColor(SDL_Color fgcolor);
            virtual Widget* setPosition(int x, int y);
            virtual Widget* setSize(int w, int h);
            virtual Widget* setBounds(int x, int y, int w, int h);
            virtual void update(float delta);
            virtual void render(SDL_Surface* surface);
            virtual void processEvent(SDL_Event event);
    };
    class Frame : public Widget {
        public:
            utils::Vector<Widget*> widgets;
            Frame();
            virtual ~Frame();
            virtual void update(float delta);
            virtual void render(SDL_Surface* surface);
            virtual void add(Widget* widget);
            virtual void processEvent(SDL_Event event);
    };
    class Label : public Widget {
        public:
            TTF_Font* font;
            const char* text;
            Label(TTF_Font* font, const char* text);
            virtual ~Label();
            virtual Label* setFont(TTF_Font* font);
            virtual Label* setText(const char* text);
            virtual void render(SDL_Surface* surface);
    };
    class Button : public Frame {
        public:
            Button();
            virtual ~Button();
            virtual void render(SDL_Surface* surface);
    };
    class ProgressBar : public Widget {
        public:
            float progress;
            ProgressBar();
            virtual ~ProgressBar();
            virtual ProgressBar* setProgress(float progress);
            virtual void render(SDL_Surface* surface);
    };
}

#endif
