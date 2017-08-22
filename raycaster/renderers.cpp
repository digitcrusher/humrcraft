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
#include "renderers.h"
#include "shapes.h"
#include "graphics.h"
#include <utils/utils.h>
#include <window/terminal.h>
#include <iostream>

SDLRenderer::SDLRenderer(const char* title, int w, int h) : Renderer() {
    this->family.pushBack("SDLRenderer");
    this->window = SDL_CreateWindow(title,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    w,
                                    h,
                                    SDL_WINDOW_SHOWN |
                                    SDL_WINDOW_RESIZABLE |
                                    SDL_WINDOW_MOUSE_FOCUS |
                                    SDL_WINDOW_INPUT_FOCUS);
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
}
SDLRenderer::~SDLRenderer() {
    SDL_DestroyWindow(this->window);
    SDL_FreeSurface(this->buffer);
}
void SDLRenderer::begin() {
    Renderer::begin();
    SDL_FreeSurface(this->buffer);
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
V2i SDLRenderer::mapPos(V2f pos) {
    return {this->buffer->w/2+(int)((pos.x-Object::getPos().x)*this->zoom), this->buffer->h/2-(int)((pos.y-Object::getPos().y)*this->zoom)};
}
V2f SDLRenderer::getPos(V2i pos) {
    return {(float)(pos.x-this->buffer->w/2)/this->zoom+Object::getPos().x, (float)(this->buffer->h/2-pos.y)/this->zoom+Object::getPos().y};
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
    V2i p = this->mapPos(pos);
    return ::getPixel(this->buffer, p.x, p.y); // use no scope (global scope)
}
void SDLRenderer::drawPixel(V2f pos, int color) {
    V2i p = this->mapPos(pos);
    ::drawPixel(this->buffer, p.x, p.y, color);
}
void SDLRenderer::drawCircle(V2f pos, int r, int color) {
    V2i p = this->mapPos(pos);
    ::drawCircle(this->buffer, p.x, p.y, r*this->zoom, color);
}
void SDLRenderer::drawSquare(V2f pos, V2f ori, int sidelen, int color) {
    V2i p = this->mapPos(pos);
    ::drawSquare(this->buffer, p.x, p.y, ori.y, sidelen*this->zoom, color);
}
void SDLRenderer::drawEllipse(V2f pos1, V2f pos2, int color) {
    V2i p1 = this->mapPos(pos1);
    V2i p2 = this->mapPos(pos2);
    ::drawEllipse(this->buffer, p1.x, p1.y, p2.x, p2.y, color);
}
void SDLRenderer::drawLine(V2f pos1, V2f pos2, int color) {
    V2i p1 = this->mapPos(pos1);
    V2i p2 = this->mapPos(pos2);
    ::drawLine(this->buffer, p1.x, p1.y, p2.x, p2.y, color);
}
void SDLRenderer::drawRectangle(V2f pos1, V2f pos2, int color) {
    V2i p1 = this->mapPos(pos1);
    V2i p2 = this->mapPos(pos2);
    ::drawRectangle(this->buffer, p1.x, p1.y, p2.x, p2.y, color);
}
int SDLRenderer::drawImage(V2f pos, SDL_Surface* image) {
    V2i p = this->mapPos(pos);
    return ::drawImage(image, p.x, p.y, this->buffer);
}

Raycaster::Raycaster(const char* title, int w, int h) : SDLRenderer(title, w, h) {
    this->family.pushBack("Raycaster");
    this->fov = M_PI/2;
    this->fos = 25;
}
Raycaster::~Raycaster() {
}
void Raycaster::begin() {
    SDLRenderer::begin();
}
void Raycaster::end() {
    this->raycast(0, this->buffer->w);
    SDLRenderer::end();
}
void Raycaster::raycast(int sx, int ex) {
    //int fogr=0, fogg=0, fogb=0;
    //int fogr=31, fogg=31, fogb=31;
    //int fogr=63, fogg=63, fogb=63;
    //int fogr=127, fogg=127, fogb=127;
    int fogr=255, fogg=255, fogb=255;
    float sa=sx/(this->buffer->w/this->fov), ea=ex/(this->buffer->w/this->fov);
    Object* ray = new Object(new Circle(0));
    float rayvel = 1;
    for(float i=sa; i<this->fov && i<ea; i+=this->fov/this->buffer->w) {
        float distance = 0;
        bool loop = 1;
        KL_Vector<float> collisionsd;
        KL_Vector<V2f> collisionsa;
        KL_Vector<Object*> collisions;
        ray->pos = Object::getPos();
        ray->vel = {rayvel, this->getOri().y+this->fov/2-i};
        while(loop && distance < this->fos) {
            for(unsigned int i=0; i<this->world->objs.size(); i++) {
                if(!this->world->objs.isFree(i)) {
                    Object* obj = this->world->objs[i];
                    if(obj->shape && checkCollision(NULL, ray, obj)) {
                        collisionsd.pushBack(distance);
                        collisionsa.pushBack({0, fatp(obj->getPos(), ray->getPos())});
                        collisions.pushBack(obj);
                        if(obj->shape->a >= 255) {
                            loop = 0;
                        }
                        break;
                    }
                }
            }
            ray->update(1);
            distance += rayvel;
        }
        if(collisions.size() > 0) {
            for(int j=collisions.size(); j-->0;) {
                if(collisions[j]->shape) {
                    float wallh = this->buffer->h/this->zoom;
                    int height = (wallh-collisionsd[j])*this->zoom;
                    if(height > 0) {
                        int sx=this->buffer->w/this->fov*i, sy=this->buffer->h/2-height/2;
                        int ey=this->buffer->h/2+height/2;
                        if(collisions[j]->shape->texmode) {
                            for(int k=0; k<ey-sy; k++) {
                                uint8_t r, g, b, a;
                                int x=(collisions[j]->shape->texture->w-1)/(M_PI*2)*(collisionsa[j].y+collisions[j]->shape->getOri().y);
                                int y=(float)(collisions[j]->shape->texture->h-1)/(ey-sy)*k;
                                SDL_GetRGBA(::getPixel(collisions[j]->shape->texture, x, y)
                                                     ,collisions[j]->shape->texture->format, &r, &g, &b, &a);
                                ::drawPixel(this->buffer, sx, sy+k, this->mapRGBA(r, g, b, a));
                                ::drawPixel(this->buffer, sx, sy+k, this->mapRGBA(fogr, fogg, fogb, 255/this->fos*collisionsd[j]*((float)a/255)));
                            }
                        }else {
                            for(int k=0; k<ey-sy; k++) {
                                uint8_t r, g, b, a;
                                r = collisions[j]->shape->r, g = collisions[j]->shape->g, b = collisions[j]->shape->b, a = collisions[j]->shape->a;
                                ::drawPixel(this->buffer, sx, sy+k, this->mapRGBA(r, g, b, a));
                                ::drawPixel(this->buffer, sx, sy+k, this->mapRGBA(fogr, fogg, fogb, 255/this->fos*collisionsd[j]*((float)a/255)));
                            }
                        }
                    }
                }
            }
        }
    }
}
void Raycaster::drawPixel(V2f pos, int color) {
}
void Raycaster::drawCircle(V2f pos, int r, int color) {
}
void Raycaster::drawSquare(V2f pos, V2f rot, int sidelen, int color) {
}
void Raycaster::drawEllipse(V2f pos1, V2f pos2, int color) {
}
void Raycaster::drawLine(V2f pos1, V2f pos2, int color) {
}
void Raycaster::drawRectangle(V2f pos1, V2f pos2, int color) {
}
int Raycaster::drawImage(V2f pos, SDL_Surface* image) {
    return 0;
}
