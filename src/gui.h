/*
 * gui.h
 * textcraft Source Code
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
#ifndef GUI_H
#define GUI_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <utils/utils.h>

enum States {
    None,
    Press,
    Release,
    Hover
};
class GUIWidget {
    public:
        KL_Vector<const char*> family;
        float time;
        void (*eventListener)(int state); //type - one from States
        bool enable;
        bool visible;
        int state;
        SDL_Color bgcolor;
        SDL_Color fgcolor;
        int x, y;
        int w, h;
        GUIWidget();
        virtual ~GUIWidget();
        virtual GUIWidget* setTime(float time);
        virtual GUIWidget* setEventListener(void (*eventListener)(int state));
        virtual GUIWidget* setEnabled(bool enable);
        virtual GUIWidget* setVisibility(bool visible);
        virtual GUIWidget* setState(int state);
        virtual GUIWidget* setBGColor(SDL_Color bgcolor);
        virtual GUIWidget* setFGColor(SDL_Color fgcolor);
        virtual GUIWidget* setPosition(int x, int y);
        virtual GUIWidget* setSize(int w, int h);
        virtual GUIWidget* setBounds(int x, int y, int w, int h);
        virtual void update(float delta);
        virtual void render(SDL_Surface* surface);
        virtual void processEvent(SDL_Event event);
};
class GUIFrame : public GUIWidget {
    public:
        KL_Vector<GUIWidget*> widgets;
        GUIFrame();
        virtual ~GUIFrame();
        virtual void update(float delta);
        virtual void render(SDL_Surface* surface);
        virtual void add(GUIWidget* widget);
        virtual void processEvent(SDL_Event event);
};
class GUILabel : public GUIWidget {
    public:
        TTF_Font* font;
        const char* text;
        GUILabel(TTF_Font* font, const char* text);
        virtual ~GUILabel();
        virtual GUILabel* setFont(TTF_Font* font);
        virtual GUILabel* setText(const char* text);
        virtual void render(SDL_Surface* surface);
};
class GUIButton : public GUIFrame {
    public:
        GUIButton();
        virtual ~GUIButton();
        virtual void render(SDL_Surface* surface);
};
class GUIProgressBar : public GUIWidget {
    public:
        float progress;
        GUIProgressBar();
        virtual ~GUIProgressBar();
        virtual GUIProgressBar* setProgress(float progress);
        virtual void render(SDL_Surface* surface);
};

#endif
