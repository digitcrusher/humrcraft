/*
 * renderers.hpp
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
#ifndef RENDERERS_HPP
#define RENDERERS_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "math.h"
#include "world.hpp"

class SDLRenderer : public Renderer {
    public:
        SDL_Window* window;
        SDL_Surface* buffer;
        float zoom; //Pixels per Meter
        SDLRenderer(const char* title, int w, int h);
        virtual ~SDLRenderer();
        virtual void begin();
        virtual void end();
        virtual bool getEvent(SDL_Event* event);
        virtual V2i mapPos(V2f pos);
        virtual V2f getPos(V2i pos);
        virtual int mapRGB(uint8_t r, uint8_t g, uint8_t b);
        virtual void getRGB(int color, uint8_t* r, uint8_t* g, uint8_t* b);
        virtual int mapRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        virtual void getRGBA(int color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a);
        virtual int getPixel(V2f pos);
        virtual void drawPixel(V2f pos, int color);
        virtual void drawCircle(V2f pos, int r, int color);
        virtual void drawSquare(V2f pos, V2f rot, int sidelen, int color);
        virtual void drawEllipse(V2f pos1, V2f pos2, int color);
        virtual void drawLine(V2f pos1, V2f pos2, int color);
        virtual void drawRectangle(V2f pos1, V2f pos2, int color);
        virtual int drawImage(V2f pos, SDL_Surface* image);
};
class SDLGLRenderer : public Renderer {
    public:
        int w, h;
        SDL_Window* window;
        SDL_GLContext context;
        float zoom; //Pixels per Meter
        SDLGLRenderer(const char* title, int w, int h, int mayorv, int minorv);
        virtual ~SDLGLRenderer();
        virtual void begin();
        virtual void end();
        virtual bool getEvent(SDL_Event* event);
        virtual V2f glMapPos(V2f pos);
        virtual V2f glGetPos(V2f pos);
        virtual V2i mapPos(V2f pos);
        virtual V2f getPos(V2i pos);
};

#endif
