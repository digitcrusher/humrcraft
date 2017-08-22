/*
 * gui.hpp
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
