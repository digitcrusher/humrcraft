/*
 * renderers.cpp
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
#include "renderers.h"
#include "shapes.h"
#include "graphics.h"
#include <utils/utils.h>
#include <iostream>

Raycaster::Raycaster(const char* title, int w, int h) : Renderer(title, w, h) {
    this->family.pushBack("Raycaster");
    this->fov = M_PI/2;
    this->fos = 25;
}
Raycaster::~Raycaster() {
    this->~Renderer();
}
void Raycaster::begin() {
    Renderer::begin();
}
void Raycaster::end() {
    int start = KL_getMS();
    this->raycast(0, this->buffer->w);
    Renderer::end();
    std::cout<<KL_getMS()-start<<'\n';
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
