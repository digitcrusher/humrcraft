/*
 * game.cpp
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
#include <math.h>
#include <iostream>
#include "game.h"
#include "graphics.h"
#include "shapes.h"

V2f carteToPolar(V2f carte) {
    return {(float)sqrt(pow(carte.x, 2)+pow(carte.y, 2)), fatp({0, 0}, carte)};
}
V2f polarToCarte(V2f polar) {
    return {polar.x*(float)cos(polar.y), polar.x*(float)sin(polar.y)};
}
float dotProductPolar(V2f v1, V2f v2) {
    return v1.x*v2.x*cos(v2.y-v1.y);
}
float dotProductCarte(V2f v1, V2f v2) {
    return v1.x*v2.x+v1.y*v2.y;
}

Game::Game() {
    this->time = 0;
}
Game::~Game() {
}
void Game::update(double delta) {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(!this->objs.isFree(i)) {
            this->objs[i]->update(delta);
        }
    }
    for(unsigned int i=0; i<this->objs.size(); i++) {
        Object* a = this->objs[i];
        if(a->shape) {
            for(unsigned int j=i+1; j<this->objs.size(); j++) {
                Object* b = this->objs[j];
                if(b->shape) {
                    V2f angle;
                    angle.y = (float)fatp(a->pos, b->pos);
                    float ra = a->shape->getRadius(angle);
                    float rb = b->shape->getRadius(angle+(V2f){M_PI, 0});
                    V2f pos1 = {(float)fmax(a->pos.x, b->pos.x), (float)fmax(a->pos.y, b->pos.y)};
                    V2f pos2 = {(float)fmin(a->pos.x, b->pos.x), (float)fmin(a->pos.y, b->pos.y)};
                    float penetration = ra+rb-sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2));
                    if(penetration > 0) {
                        float e, na, nb, ma, mb, pa, pb, f;

                        e = fmin(a->shape->restitution, b->shape->restitution); //restitution
                        na = a->shape->getNormal(angle).y; //normal a
                        nb = b->shape->getNormal(angle+(V2f){0, M_PI}).y; //normal b
                        if(a->shape->invmass) {
                            ma = a->shape->invmass;
                        }else {
                            ma = 1;
                        }
                        if(b->shape->invmass) {
                            mb = b->shape->invmass;
                        }else {
                            mb = 1;
                        }
                        pa = a->vel.x/ma;
                        pb = b->vel.x/mb;

                        f = (pa+pb)*cos(a->vel.y-nb)*-(1+e)/2+penetration*(float)delta;

                        a->applyImpulse({f, nb});

                        f = (pa+pb)*cos(b->vel.y-na)*-(1+e)/2+penetration*(float)delta;

                        b->applyImpulse({f, na});
                    }
                }
            }
        }
    }
    this->time += delta;
}
void Game::render() {
    Renderer* render;
    if(!(render = (Renderer*)this->getObject("Renderer"))) return;
    render->begin();
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(!this->objs.isFree(i)) {
            this->objs[i]->render(render);
        }
    }
    render->end();
}
int Game::add(Object* obj) {
    obj->world = this;
    return (obj->id = this->objs.add(obj));
}
bool Game::remove(int id) {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(this->objs[i]->id == id) {
            this->objs.remove(i);
            return 0;
        }
    }
    return 1;
}
bool Game::destroy(int id) {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(this->objs[i]->id == id) {
            delete this->objs[i];
            this->objs.getArray()[i].free = 1;
            return 0;
        }
    }
    return 1;
}
Object* Game::getObject(int id) {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(this->objs[i]->id == id) {
            return this->objs[i];
        }
    }
    return NULL;
}
Object* Game::getObject(const char* classname) {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(!strcmp(this->objs[i]->classname, classname)) {
            return this->objs[i];
        }
    }
    return NULL;
}
void Game::generate(int seed, V2f mins, V2f maxs) {
    srand(seed);
    for(unsigned int i=0; i<this->thingtypes.size(); i++) {
        if(this->thingtypes[i]->generation.generate) {
            for(int x=mins.x; x<maxs.x; x++) {
                for(int y=mins.y; y<maxs.y; y++) {
                    srand(rand());
                    int n = rand()%(int)(1/this->thingtypes[i]->generation.occurence);
                    if(n == 1) {
                        Thing* thing = this->createThing(i);
                        thing->pos = {(float)x, (float)y};
                        this->add(thing);
                    }
                }
            }
        }
    }
}
unsigned int Game::registerThing(Thing* thing) {
    return this->thingtypes.add(thing);
}
Thing* Game::createThing(unsigned int type) {
    if(type >= 0 && type < this->thingtypes.size()) {
        Thing* temp = this->thingtypes[type];
        Thing* thing = new Thing(temp->health, temp->attack, temp->generation
                                ,temp->updatef, temp->renderf);
        thing->type = type;
        return thing;
    }
    return NULL;
}
Game& Game::operator=(const Game& rvalue) {
    this->time = rvalue.time;
    this->objs = rvalue.objs;
    this->thingtypes = rvalue.thingtypes;
    return *this;
}

Object::Object(Shape* shape) {
    this->id = 0;
    this->classname = "Object";
    this->world = NULL;
    this->shape = shape;
    if(this->shape) {
        this->shape->obj = this;
    }
    this->time = 0;
    this->pos = {0, 0};
    this->rot = {0, 0};
    this->vel = {0, 0};
    this->rspd = {0, 0};
}
Object::Object(Shape* shape, V2f pos, V2f rot) : Object(shape) {
    this->pos = pos;
    this->rot = rot;
}
Object::~Object() {
    if(this->world) {
        this->world->remove(this->id);
    }
}
void Object::update(double delta) {
    if(this->shape) {
        this->shape->update(delta);    
    }
    this->pos.x += cos(this->vel.y)*this->vel.x*delta;
    this->pos.y += sin(this->vel.y)*this->vel.x*delta;
    this->rot.y += rspd.y;
//    if(this->world) {
//        this->vspd *= 1-this->world->friction*this->invmass*delta;
//    }
    this->time += delta;
}
void Object::render(Renderer* render) {
    if(this->shape) {
        this->shape->render(render);    
    }
}
void Object::applyImpulse(V2f j) {
    if(this->shape && this->shape->invmass) {
        this->vel = carteToPolar(polarToCarte({j.x*this->shape->invmass, j.y})+polarToCarte(this->vel));
    }
}
Object& Object::operator=(const Object& rvalue) {
    this->time = rvalue.time;
    this->pos = rvalue.pos;
    this->rot = rvalue.rot;
    this->vel = rvalue.vel;
    this->rspd = rvalue.rspd;
    return *this;
}

Shape::Shape() : Object(NULL) {
    this->classname = "Shape";
    this->restitution = 1;
    this->invmass = 1;
    this->r = 255;
    this->g = 255;
    this->b = 255;
}
Shape::Shape(float restitution, float mass) : Shape() {
    this->restitution = restitution;
    this->invmass = 1/mass;
}
Shape::Shape(float restitution, float mass, int r, int g, int b) : Shape(restitution, mass) {
    this->r = r;
    this->g = g;
    this->b = b;
}
Shape::~Shape() {
    this->~Object();
}
void Shape::update(double delta) {
}
void Shape::render(Renderer* render) {
}
V2f Shape::getPos() {
    return {this->obj->pos.x+this->pos.x*(float)cos(this->obj->rot.y)
           ,this->obj->pos.y+this->pos.y*(float)sin(this->obj->rot.y)};
}
V2f Shape::getRot() {
    return this->rot+this->obj->rot;
}
float Shape::getRadius(V2f angle) {
    return 0;
}
V2f Shape::getNormal(V2f angle) {
    return {angle.y+(float)M_PI, 0};
}

Renderer::Renderer(const char* title, int w, int h, int zoom) : Object(NULL) {
    this->classname = "Renderer";
    this->zoom = zoom;
    /*this->size.x = w;
    this->size.y = h;
    this->zbuff = (float*)malloc(sizeof(float)*this->size.x*this->size.y);
    this->screen = (char*)malloc(sizeof(char)*this->size.x*this->size.y);*/
    this->window = SDL_CreateWindow(title,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    w,
                                    h,
                                    SDL_WINDOW_SHOWN |
                                    SDL_WINDOW_RESIZABLE |
                                    SDL_WINDOW_MOUSE_FOCUS |
                                    SDL_WINDOW_INPUT_FOCUS);
}
Renderer::~Renderer() {
    this->~Object();
    /*free(this->zbuff);
    free(this->screen);*/
    SDL_DestroyWindow(this->window);
}
void Renderer::begin() {
    /*this->fbefore = KL_stdterm->flags;
    KL_stdterm->flags = TERMINAL_DEFAULT_FLAGS-(TERMINAL_N_UPDATE*TERMINAL_DEFAULT_FLAGS & TERMINAL_N_UPDATE);
    KL_flush(KL_stdterm, TERMINAL_OUTPUT);
    memset(this->zbuff, 0, sizeof(float)*this->size.x*this->size.y);
    memset(this->screen, '\0', sizeof(char)*this->size.x*this->size.y);*/
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
}
void Renderer::end() {
    /*for(int y=0; y<this->size.y; y++) {
        for(int x=0; x<this->size.x; x++) {
            KL_cwrite(KL_stdterm, *(this->screen+x+y*this->size.x));
        }
        KL_cwrite(KL_stdterm, '\n');
    }
	KL_swritef(KL_stdterm, "%d TPS\n%d FPS\n", this->ticks, this->frames);
    KL_stdterm->flags = this->fbefore;
    KL_updateTerminal(KL_stdterm);*/
    SDL_UpdateWindowSurface(this->window);
}
bool Renderer::getEvent(SDL_Event* event) {
    return SDL_PollEvent(event);
}
void Renderer::draw(char c, V3f pos) {
    /*V2f p = {this->size.x/2+(pos.x-this->pos.x), this->size.y/2-(pos.y-this->pos.y)};
    if(squareInSquare({0, 0, (float)this->size.x-1, (float)this->size.y-1}
               ,{p.x, p.y, p.x, p.y})) {
        if(pos.z >= *(this->zbuff+(int)p.x+(int)p.y*this->size.x)) {
            *(this->zbuff+(int)p.x+(int)p.y*this->size.x) = pos.z;
            *(this->screen+(int)p.x+(int)p.y*this->size.x) = c;
        }
    }*/
}
int Renderer::mapRGB(int r, int g, int b) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    return SDL_MapRGB(screen->format, r, g, b);
}
int Renderer::getPixel(V2f pos) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    V2f p = {screen->w/2+(pos.x-this->pos.x)*this->zoom, screen->h/2-(pos.y-this->pos.y)*this->zoom};
    return ::getPixel(screen, (int)p.x, (int)p.y); // use no scope (global scope)
}
void Renderer::drawPixel(V2f pos, int color) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    V2f p = {screen->w/2+(pos.x-this->pos.x)*this->zoom, screen->h/2-(pos.y-this->pos.y)*this->zoom};
    ::drawPixel(screen, (int)p.x, (int)p.y, color);
}
void Renderer::drawCircle(V2f pos, int r, int color) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    V2f p = {screen->w/2+(pos.x-this->pos.x)*this->zoom, screen->h/2-(pos.y-this->pos.y)*this->zoom};
    ::drawCircle(screen, (int)p.x, (int)p.y, r*this->zoom, color);
}
void Renderer::drawSquare(V2f pos, V2f rot, int sidelen, int color) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    V2f p = {screen->w/2+(pos.x-this->pos.x)*this->zoom, screen->h/2-(pos.y-this->pos.y)*this->zoom};
    ::drawSquare(screen, (int)p.x, (int)p.y, rot.y, sidelen*this->zoom, color);
}
void Renderer::drawEllipse(V2f pos1, V2f pos2, int color) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    V2f p1 = {screen->w/2+(pos1.x-this->pos.x)*this->zoom, screen->h/2-(pos1.y-this->pos.y)*this->zoom};
    V2f p2 = {screen->w/2+(pos2.x-this->pos.x)*this->zoom, screen->h/2-(pos2.y-this->pos.y)*this->zoom};
    ::drawEllipse(screen, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, color);
}
void Renderer::drawLine(V2f pos1, V2f pos2, int color) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    V2f p1 = {screen->w/2+(pos1.x-this->pos.x)*this->zoom, screen->h/2-(pos1.y-this->pos.y)*this->zoom};
    V2f p2 = {screen->w/2+(pos2.x-this->pos.x)*this->zoom, screen->h/2-(pos2.y-this->pos.y)*this->zoom};
    ::drawLine(screen, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, color);
}
void Renderer::drawRectangle(V2f pos1, V2f pos2, int color) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    V2f p1 = {screen->w/2+(pos1.x-this->pos.x)*this->zoom, screen->h/2-(pos1.y-this->pos.y)*this->zoom};
    V2f p2 = {screen->w/2+(pos2.x-this->pos.x)*this->zoom, screen->h/2-(pos2.y-this->pos.y)*this->zoom};
    ::drawRectangle(screen, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, color);
}
int Renderer::drawImage(V2f pos, SDL_Surface* image) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    V2f p = {screen->w/2+(pos.x-this->pos.x)*this->zoom, screen->h/2-(pos.y-this->pos.y)*this->zoom};
    return ::drawImage(image, (int)p.x, (int)p.y, screen);
}
void Renderer::drawGraph(float (*func)(float), float zoom, V2f pos, int color) {
    SDL_Surface* screen = SDL_GetWindowSurface(this->window);
    V2f p = {screen->w/2+(pos.x-this->pos.x)*this->zoom, screen->h/2-(pos.y-this->pos.y)*this->zoom};
    ::drawGraph(screen, func, zoom*this->zoom, p.x, p.y, color);
}
Renderer& Renderer::operator=(const Renderer& rvalue) {
    Object::operator=(rvalue); //Call base class = operator
    /*this->size = rvalue.size;
    this->zbuff = (float*)realloc(this->zbuff, sizeof(float)*this->size.x*this->size.y);
    this->screen = (char*)realloc(this->screen, sizeof(char)*this->size.x*this->size.y);
    memcpy(this->zbuff, rvalue.zbuff, sizeof(float)*this->size.x*this->size.y);
    memcpy(this->screen, rvalue.screen, sizeof(char)*this->size.x*this->size.y);*/
    return *this;
}

Thing::Thing(float health, float attack, genSettings generation
            ,void (*updatef)(Thing* thing, double delta)
            ,void (*renderf)(Thing* thing, Renderer* context)) : Object(new Square(2)) {
    this->classname = "Thing";
    this->type = 0;
    this->bagcurr = 0;
    this->generation = generation;
    this->deathcounter = 0;
    this->deathtime = 60;
    this->delay = 0;
    this->health = health;
    this->attack = attack;
    this->updatef = updatef;
    this->renderf = renderf;
}
Thing::~Thing() {
    this->~Object();
    if(this->world) {
        for(unsigned int i=0; i<this->bag.size(); i++) {
            this->bagcurr = i;
            drop();
        }
    }
}
void Thing::update(double delta) {
    Object::update(delta);
    if(this->deathcounter >= deathtime) {
        this->~Thing();
        return;
    }
    if(this->health <= 0) {
        this->deathcounter += delta;
    }
    if(this->delay > 0) {
        this->delay -= delta;
    }
    if(this->updatef) {
        this->updatef(this, delta);
    }
}
void Thing::render(Renderer* render) {
    Object::render(render);
    if(this->renderf) {
        this->renderf(this, render);
    }
}
void Thing::hit() {
}
void Thing::put() {
}
void Thing::pick() {
    if(this->world) {
        for(unsigned int i=0; i<this->world->objs.size(); i++) {
            Object* obj = this->world->objs[i];
            if(!strcmp(obj->classname, "Thing")) {
                if(((Thing*)obj)->health <= 0) {
                    if(obj->pos == this->pos) { //TODO: Proper collision detection
                        this->bag.add((Thing*)obj);
                        this->world->objs.remove(obj->id);
                        return;
                    }
                }
            }
        }
    }
}
void Thing::drop() {
    if(bagcurr >= 0 && bagcurr < this->bag.size()) {
        this->bag[bagcurr]->pos = this->pos;
        this->bag[bagcurr]->rot = this->rot;
        this->bag[bagcurr]->vel = this->vel;
        this->world->add(this->bag[bagcurr]);
        this->bag.remove(bagcurr);
    }
}
Thing& Thing::operator=(const Thing& rvalue) {
    Object::operator=(rvalue); //Call base class = operator
    this->type = rvalue.type;
    this->bag = rvalue.bag;
    this->bagcurr = rvalue.bagcurr;
    this->deathcounter = rvalue.deathcounter;
    this->deathtime = rvalue.deathtime;
    this->delay = rvalue.delay;
    this->health = rvalue.health;
    this->attack = rvalue.attack;
    this->updatef = rvalue.updatef;
    this->renderf = rvalue.renderf;
    return *this;
}
void Thing::defaultRenderf(Thing* thing, Renderer* render) {
    switch(thing->type) {
        case Tree:
            render->draw('Y', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Log:
            render->draw('=', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Stick:
            render->draw('/', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Flower:
            render->draw('*', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Grass:
            render->draw('"', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Wheat:
            render->draw('w', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Seeds:
            render->draw('.', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Flint:
            render->draw('^', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Stone:
            render->draw('n', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Water:
            render->draw('~', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Human:
            render->draw('@', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Pig: //TODO
            render->draw('m', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Cow: //TODO
            render->draw('M', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Hamster: //TODO
            render->draw('o', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case Bread:
            render->draw('B', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case RawMeat:
            render->draw('O', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        case CookedMeat:
            render->draw('0', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
        default:
            render->draw('#', {thing->pos.x, thing->pos.y, (float)!thing->generation.generate});
            break;
    }
}
