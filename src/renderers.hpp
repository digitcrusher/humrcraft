/*
 * renderers.hpp
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
#ifndef RENDERERS_HPP
#define RENDERERS_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "math.h"
#include "world.hpp"

class SDLRenderer;
class SDLGLRenderer;

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
        virtual V2i SDLMapPos(V2f pos);
        virtual V2f SDLGetPos(V2i pos);
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
        virtual V2f GLMapPos(V2f pos, V2f rot); //TODO: improve this and the one below
        virtual V2f GLGetPos(V2f pos, V2f rot);
        virtual V2i SDLMapPos(V2f pos);
        virtual V2f SDLGetPos(V2i pos);
};

#endif
