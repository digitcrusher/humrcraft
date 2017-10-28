/*
 * gui.cpp
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
#include <math.h>
#include "graphics.hpp"
#include "gui.hpp"

namespace GUI {
    Widget::Widget() {
        this->family.pushBack("Widget");
        this->time = 0;
        this->eventListener = NULL;
        this->enable = 1;
        this->visible = 1;
        this->state = None;
        this->bgcolor = {0, 0, 0, 0};
        this->fgcolor = {0, 0, 0, 0};
        this->x = 0;
        this->y = 0;
        this->w = 0;
        this->h = 0;
    }
    Widget::~Widget() {
    }
    Widget* Widget::setTime(float time) {
        this->time = time;
        return this;
    }
    Widget* Widget::setEventListener(void (*eventListener)(int state)) {
        this->eventListener = eventListener;
        return this;
    }
    Widget* Widget::setEnabled(bool enable) {
        this->enable = enable;
        return this;
    }
    Widget* Widget::setVisibility(bool visible) {
        this->visible = visible;
        return this;
    }
    Widget* Widget::setState(int state) {
        this->state = state;
        return this;
    }
    Widget* Widget::setBGColor(SDL_Color bgcolor) {
        this->bgcolor = bgcolor;
        return this;
    }
    Widget* Widget::setFGColor(SDL_Color fgcolor) {
        this->fgcolor = fgcolor;
        return this;
    }
    Widget* Widget::setPosition(int x, int y) {
        this->x = x;
        this->y = y;
        return this;
    }
    Widget* Widget::setSize(int w, int h) {
        this->w = w;
        this->h = h;
        return this;
    }
    Widget* Widget::setBounds(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        return this;
    }
    void Widget::update(float delta) {
        this->time += delta;
    }
    void Widget::render(SDL_Surface* surface) {
        if(!this->visible) return;
        uint8_t r, g, b, a;
        if(this->enable) {
            r = this->bgcolor.r;
            g = this->bgcolor.g;
            b = this->bgcolor.b;
            a = this->bgcolor.a;
        }else {
            r = 127;
            g = 127;
            b = 127;
            a = this->bgcolor.a;
        }
        SDL_Rect rect = {this->x, this->y, this->w, this->h};
        SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, r, g, b, a));
    }
    void Widget::processEvent(SDL_Event event) {
        int laststate = this->state;
        switch(event.type) {
            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button) {
                    case SDL_BUTTON_LEFT:
                        if(event.button.x > this->x && event.button.x < this->w && event.button.y > this->y && event.button.y < this->h) {
                            this->state = Press;
                        }
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch(event.button.button) {
                    case SDL_BUTTON_LEFT:
                        if(event.button.x > this->x && event.button.x < this->w && event.button.y > this->y && event.button.y < this->h) {
                            this->state = Release;
                        }
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                if(event.motion.x > this->x && event.motion.x < this->w && event.motion.y > this->y && event.motion.y < this->h) {
                    this->state = Hover;
                }
                break;
        }
        if(event.button.x > this->x && event.button.x < this->w && event.button.y > this->y && event.button.y < this->h) {
            if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                this->state = Press;
            }else if(laststate == Press) {
                this->state = Release;
            }
        }else {
            this->state = None;
        }
        if(laststate != this->state && this->eventListener) {
            this->eventListener(this->state);
        }
    }

    Frame::Frame() : Widget() {
        this->family.pushBack("Frame");
    }
    Frame::~Frame() {
    }
    void Frame::update(float delta) {
        Widget::update(delta);
        for(int i=0; i<this->widgets.size(); i++) {
            this->widgets[i]->update(delta);
        }
    }
    void Frame::render(SDL_Surface* surface) {
        Widget::render(surface);
        int rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif
        SDL_Surface* surface2 = SDL_CreateRGBSurface(0, this->w, this->h, 32, rmask, gmask, bmask, amask);
        SDL_FillRect(surface2, NULL, SDL_MapRGBA(surface2->format, 0, 0, 0, 0));
        for(int i=0; i<this->widgets.size(); i++) {
            this->widgets[i]->render(surface2);
        }
        graphics::drawImage(surface2, this->x, this->y, surface);
        SDL_FreeSurface(surface2);
    }
    void Frame::add(Widget* widget) {
        this->widgets.pushBack(widget);
    }
    void Frame::processEvent(SDL_Event event) {
        Widget::processEvent(event);
        for(int i=0; i<this->widgets.size(); i++) {
            this->widgets[i]->processEvent(event);
        }
    }

    Label::Label(TTF_Font* font, const char* text) : Widget() {
        this->family.pushBack("Label");
        this->font = font;
        this->text = text;
    }
    Label::~Label() {
    }
    Label* Label::setFont(TTF_Font* font) {
        this->font = font;
        return this;
    }
    Label* Label::setText(const char* text) {
        this->text = text;
        return this;
    }
    void Label::render(SDL_Surface* surface) {
        uint8_t r, g, b, a;
        if(this->enable) {
            r = this->bgcolor.r;
            g = this->bgcolor.g;
            b = this->bgcolor.b;
            a = this->bgcolor.a;
        }else {
            r = 127;
            g = 127;
            b = 127;
            a = this->bgcolor.a;
        }
        SDL_Rect dstrect = {this->x, this->y, this->w, this->h};
        SDL_FillRect(surface, &dstrect, SDL_MapRGBA(surface->format, r, g, b, a));
        if(this->enable) {
            r = this->fgcolor.r;
            g = this->fgcolor.g;
            b = this->fgcolor.b;
            a = this->fgcolor.a;
        }else {
            r = 127;
            g = 127;
            b = 127;
            a = this->fgcolor.a;
        }
        SDL_Surface* text = TTF_RenderText_Blended_Wrapped(this->font, this->text, {r, g, b, a}, this->w);
        SDL_Rect srcrect = {0, 0, this->w, this->h};
        SDL_BlitSurface(text, &srcrect, surface, &dstrect);
    }

    Button::Button() : Frame() {
    }
    Button::~Button() {
    }
    void Button::render(SDL_Surface* surface) {
        Widget::render(surface);
        uint8_t r, g, b, a;
        if(this->enable) {
            r = this->bgcolor.r-(this->state == Press)*128;
            g = this->bgcolor.g-(this->state == Press)*128;
            b = this->bgcolor.b-(this->state == Press)*128;
            a = this->bgcolor.a;
        }else {
            r = 127;
            g = 127;
            b = 127;
            a = this->bgcolor.a;
        }
        r = r>this->bgcolor.r?0:r;
        g = g>this->bgcolor.g?0:g;
        b = b>this->bgcolor.b?0:b;
        SDL_Rect rect = {this->x, this->y, this->w, this->h};
        SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, r, g, b, a));
        uint8_t r1, g1, b1, a1;
        uint8_t r2, g2, b2, a2;
        r1 = r+64;
        g1 = g+64;
        b1 = b+64;
        a1 = a;
        r2 = r-64;
        g2 = g-64;
        b2 = b-64;
        a2 = a;
        r1 = r1<r?r:r1;
        g1 = g1<g?g:g1;
        b1 = b1<b?b:b1;
        r2 = r2>r?0:r2;
        g2 = g2>g?0:g2;
        b2 = b2>b?0:b2;
        if(this->state == Press) {
            graphics::drawLine(surface, this->x, this->y, this->x+this->w-1, this->y, SDL_MapRGBA(surface->format, r2, g2, b2, a2));
            graphics::drawLine(surface, this->x, this->y, this->x, this->y+this->h-1, SDL_MapRGBA(surface->format, r2, g2, b2, a2));
            graphics::drawLine(surface, this->x+this->w-1, this->y, this->x+this->w-1, this->y+this->h-1, SDL_MapRGBA(surface->format, r1, g1, b1, a1));
            graphics::drawLine(surface, this->x+this->w-1, this->y+this->h-1, this->x, this->y+this->h-1, SDL_MapRGBA(surface->format, r1, g1, b1, a1));
        }else {
            graphics::drawLine(surface, this->x+this->w-1, this->y, this->x+this->w-1, this->y+this->h-1, SDL_MapRGBA(surface->format, r2, g2, b2, a2));
            graphics::drawLine(surface, this->x+this->w-1, this->y+this->h-1, this->x, this->y+this->h-1, SDL_MapRGBA(surface->format, r2, g2, b2, a2));
            graphics::drawLine(surface, this->x, this->y, this->x+this->w-1, this->y, SDL_MapRGBA(surface->format, r1, g1, b1, a1));
            graphics::drawLine(surface, this->x, this->y, this->x, this->y+this->h-1, SDL_MapRGBA(surface->format, r1, g1, b1, a1));
        }
        int border=1;
        int rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif
        SDL_Surface* surface2 = SDL_CreateRGBSurface(0, this->w-border*2, this->h-border*2, 32, rmask, gmask, bmask, amask);
        SDL_FillRect(surface2, NULL, SDL_MapRGBA(surface2->format, 0, 0, 0, 0));
        for(int i=0; i<this->widgets.size(); i++) {
            this->widgets[i]->render(surface2);
        }
        graphics::drawImage(surface2, this->x+border, this->y+border, surface);
        SDL_FreeSurface(surface2);
    }

    ProgressBar::ProgressBar() : Widget() {
        this->progress = 0;
    }
    ProgressBar::~ProgressBar() {
    }
    ProgressBar* ProgressBar::setProgress(float progress) {
        this->progress = progress;
        return this;
    }
    void ProgressBar::render(SDL_Surface* surface) {
        Widget::render(surface);
        uint8_t r, g, b, a;
        if(this->enable) {
            r = this->fgcolor.r;
            g = this->fgcolor.g;
            b = this->fgcolor.b;
            a = this->fgcolor.a;
        }else {
            r = 127;
            g = 127;
            b = 127;
            a = this->fgcolor.a;
        }
        SDL_Rect rect = {this->x, this->y, (int)fmin(this->w*this->progress, this->w), this->h};
        SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, r, g, b, a));
    }
}
