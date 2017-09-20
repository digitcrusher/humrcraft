/*
 * renderers.cpp
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
#include <iostream>
#include "renderers.hpp"
#include "graphics.hpp"

SDLRenderer::SDLRenderer(const char* title, int w, int h) : Renderer() {
    this->family.pushBack("SDLRenderer");
    if(!(this->window = SDL_CreateWindow(title,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    w,
                                    h,
                                    SDL_WINDOW_SHOWN |
                                    SDL_WINDOW_RESIZABLE |
                                    SDL_WINDOW_MOUSE_FOCUS |
                                    SDL_WINDOW_INPUT_FOCUS))) {
        std::cerr<<"SDL_CreateWindow error: "<<SDL_GetError()<<'\n';
        throw;
    }
    this->buffer = NULL;
    this->zoom = 1;
}
SDLRenderer::~SDLRenderer() {
    SDL_DestroyWindow(this->window);
    if(this->buffer) SDL_FreeSurface(this->buffer);
}
void SDLRenderer::begin() {
    Renderer::begin();
    if(!this->buffer) SDL_FreeSurface(this->buffer);
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
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    this->buffer = SDL_CreateRGBSurface(0, screen->w, screen->h, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(this->buffer, NULL, SDL_MapRGB(this->buffer->format, 0, 0, 0));
}
void SDLRenderer::end() {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    ::drawImage(this->buffer, 0, 0, screen);
    SDL_UpdateWindowSurface(this->window);
    Renderer::end();
}
bool SDLRenderer::getEvent(SDL_Event* event) {
    return SDL_PollEvent(event);
}
V2i SDLRenderer::SDLMapPos(V2f pos) {
    return {this->buffer->w/2+(int)((pos.x-this->getPos().x)*this->zoom), this->buffer->h/2-(int)((pos.y-this->getPos().y)*this->zoom)};
}
V2f SDLRenderer::SDLGetPos(V2i pos) {
    return {(float)(pos.x-this->buffer->w/2)/this->zoom+this->getPos().x, (float)(this->buffer->h/2-pos.y)/this->zoom+this->getPos().y};
}
int SDLRenderer::mapRGB(uint8_t r, uint8_t g, uint8_t b) {
    return SDL_MapRGB(this->buffer->format, r, g, b);
}
void SDLRenderer::getRGB(int color, uint8_t* r, uint8_t* g, uint8_t* b) {
    SDL_GetRGB(color, this->buffer->format, r, g, b);
}
int SDLRenderer::mapRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return SDL_MapRGBA(this->buffer->format, r, g, b, a);
}
void SDLRenderer::getRGBA(int color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) {
    SDL_GetRGBA(color, this->buffer->format, r, g, b, a);
}
int SDLRenderer::getPixel(V2f pos) {
    V2i p = this->SDLMapPos(pos);
    return ::getPixel(this->buffer, p.x, p.y); // use no scope (global scope)
}
void SDLRenderer::drawPixel(V2f pos, int color) {
    V2i p = this->SDLMapPos(pos);
    ::drawPixel(this->buffer, p.x, p.y, color);
}
void SDLRenderer::drawCircle(V2f pos, int r, int color) {
    V2i p = this->SDLMapPos(pos);
    ::drawCircle(this->buffer, p.x, p.y, r*this->zoom, color);
}
void SDLRenderer::drawSquare(V2f pos, V2f ori, int sidelen, int color) {
    V2i p = this->SDLMapPos(pos);
    ::drawSquare(this->buffer, p.x, p.y, ori.y, sidelen*this->zoom, color);
}
void SDLRenderer::drawEllipse(V2f pos1, V2f pos2, int color) {
    V2i p1 = this->SDLMapPos(pos1);
    V2i p2 = this->SDLMapPos(pos2);
    ::drawEllipse(this->buffer, p1.x, p1.y, p2.x, p2.y, color);
}
void SDLRenderer::drawLine(V2f pos1, V2f pos2, int color) {
    V2i p1 = this->SDLMapPos(pos1);
    V2i p2 = this->SDLMapPos(pos2);
    ::drawLine(this->buffer, p1.x, p1.y, p2.x, p2.y, color);
}
void SDLRenderer::drawRectangle(V2f pos1, V2f pos2, int color) {
    V2i p1 = this->SDLMapPos(pos1);
    V2i p2 = this->SDLMapPos(pos2);
    ::drawRectangle(this->buffer, p1.x, p1.y, p2.x, p2.y, color);
}
int SDLRenderer::drawImage(V2f pos, SDL_Surface* image) {
    V2i p = this->SDLMapPos(pos)-(V2i){image->w/2, image->h/2};
    return ::drawImage(image, p.x, p.y, this->buffer);
}

SDLGLRenderer::SDLGLRenderer(const char* title, int w, int h, int mayorv, int minorv) : Renderer() {
    this->family.pushBack("SDLGLRenderer");
    this->w = w;
    this->h = h;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, mayorv);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorv);
    if(!(this->window = SDL_CreateWindow(title,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    this->w,
                                    this->h,
                                    SDL_WINDOW_OPENGL |
                                    SDL_WINDOW_SHOWN |
                                    SDL_WINDOW_RESIZABLE |
                                    SDL_WINDOW_MOUSE_FOCUS |
                                    SDL_WINDOW_INPUT_FOCUS))) {
        std::cerr<<"SDL_CreateWindow error: "<<SDL_GetError()<<'\n';
        throw;
    }
    if(!(this->context = SDL_GL_CreateContext(this->window))) {
        std::cerr<<"SDL_GL_CreateContext error: "<<SDL_GetError()<<'\n';
        throw;
    }
    this->zoom = 1;
    bool fail=0;
    GLenum error = GL_NO_ERROR;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout<<"OpenGL initialization error: "<<gluErrorString(error)<<'\n';
        fail = 1;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout<<"OpenGL initialization error: "<<gluErrorString(error)<<'\n';
        fail = 1;
    }
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout<<"OpenGL initialization error: "<<gluErrorString(error)<<'\n';
        fail = 1;
    }
    glClearColor(0, 0, 0, 1);
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout<<"OpenGL initialization error: "<<gluErrorString(error)<<'\n';
        fail = 1;
    }
    if(fail) {
        throw;
    }
}
SDLGLRenderer::~SDLGLRenderer() {
    SDL_DestroyWindow(this->window);
}
void SDLGLRenderer::begin() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glRotatef(-this->getOri().y, 0, 0, 1);
    glTranslatef(-this->GLMapPos(Object::getPos(), -this->getOri()).x, -this->GLMapPos(Object::getPos(), -this->getOri()).y, 0);
    SDL_GetWindowSize(this->window, &this->w, &this->h);
    glViewport(0, 0, this->w, this->h);
}
void SDLGLRenderer::end() {
    SDL_GL_SwapWindow(this->window);
}
bool SDLGLRenderer::getEvent(SDL_Event* event) {
    return SDL_PollEvent(event);
}
V2f SDLGLRenderer::GLMapPos(V2f pos, V2f rot) {
    V2f pos1 = {(float)1/(this->w/2)*pos.x*this->zoom, (float)1/(this->h/2)*pos.y*this->zoom};
    V2f pos2 = {(float)1/(this->h/2)*pos.x*this->zoom, (float)1/(this->w/2)*pos.y*this->zoom};
    float s = fabs(trianglewave(rot.y-this->getOri().y));
    float c = fabs(trianglewave(rot.y-this->getOri().y+M_PI/2));
    return {pos1.x*c+pos2.x*s, pos1.y*c+pos2.y*s};
}
V2f SDLGLRenderer::GLGetPos(V2f pos, V2f rot) {
    return {pos.x/this->zoom/((float)1/(this->w/2)), pos.y/this->zoom/((float)1/(this->h/2))};
}
V2i SDLGLRenderer::SDLMapPos(V2f pos) {
    return {this->w/2+(int)((pos.x-this->getPos().x)*this->zoom), this->h/2-(int)((pos.y-this->getPos().y)*this->zoom)};
}
V2f SDLGLRenderer::SDLGetPos(V2i pos) {
    return {(float)(pos.x-this->w/2)/this->zoom+this->getPos().x, (float)(this->h/2-pos.y)/this->zoom+this->getPos().y};
}
