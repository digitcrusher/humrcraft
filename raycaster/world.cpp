/*
 * world.cpp
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
#include <utils/math.h>
#include <iostream>
#include "world.h"
#include "graphics.h"
#include "shapes.h"
#include "renderers.h"

bool checkCollision(manifold* manifold, Object* a, Object* b) {
    if(a && b && a->shape && b->shape) {
        V2f angle;
        angle.y = (float)fatp(a->getPos(), b->getPos());
        V2f ra = a->shape->getRadius(angle);
        V2f rb = b->shape->getRadius(angle+(V2f){0, M_PI});

        /*V2f pos1 = {(float)fmax(a->shape->getPos().x, b->shape->getPos().x), (float)fmax(a->shape->getPos().y, b->shape->getPos().y)};
        V2f pos2 = {(float)fmin(a->shape->getPos().x, b->shape->getPos().x), (float)fmin(a->shape->getPos().y, b->shape->getPos().y)};
        float penetration = ra.x+rb.x-sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2));*/

        V2f starta;
        V2f enda;
        V2f tempstartb;
        V2f tempendb;
        V2f startb;
        V2f endb;
        starta.x = 0;
        starta.y = 0;
        enda.x = ra.x;
        enda.y = 0;
        tempstartb.x = cos(-(ra.y))*(b->shape->getPos().x-a->shape->getPos().x)-sin(-(ra.y))*(b->shape->getPos().y-a->shape->getPos().y);
        tempstartb.y = sin(-(ra.y))*(b->shape->getPos().x-a->shape->getPos().x)+cos(-(ra.y))*(b->shape->getPos().y-a->shape->getPos().y);
        tempendb.x = cos(rb.y-ra.y)*rb.x+tempstartb.x;
        tempendb.y = sin(rb.y-ra.y)*rb.x+tempstartb.y;
        startb.x = fmin(tempstartb.x, tempendb.x);
        startb.y = fmin(tempstartb.y, tempendb.y);
        endb.x = fmax(tempstartb.x, tempendb.x);
        endb.y = fmax(tempstartb.y, tempendb.y);
        float penetration=0;
        if(startb.x <= enda.x && endb.x >= starta.x && startb.y <= starta.y && endb.y >= starta.y) {
            penetration = 1;
        }
        float e = fmin(a->shape->restitution, b->shape->restitution);
        V2f na = a->shape->getNormal(ra);
        V2f nb = b->shape->getNormal(rb);
        float ma;
        float mb;
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
        float pa = a->getVel().x/ma;
        float pb = b->getVel().x/mb;
        V2f fa = {(pa+pb)*(float)cos((a->getVel()-nb).y)*-(1+e)/2+penetration, nb.y};
        V2f fb = {(pa+pb)*(float)cos((b->getVel()-na).y)*-(1+e)/2+penetration, na.y};
        if(manifold) {
            memset(manifold, 0, sizeof(struct manifold));
            manifold->a = a;
            manifold->b = b;
            manifold->ra = ra;
            manifold->rb = rb;
            manifold->angle = angle;
            manifold->starta = starta;
            manifold->enda = enda;
            manifold->tempstartb = tempstartb;
            manifold->tempendb = tempendb;
            manifold->startb = startb;
            manifold->endb = endb;
            if(penetration) {
                manifold->penetration = penetration;
                manifold->e = e;
                manifold->na = na;
                manifold->nb = nb;
                manifold->ma = ma;
                manifold->mb = mb;
                manifold->pa = pa;
                manifold->pb = pb;
                manifold->fa = fa;
                manifold->fb = fb;
            }
        }
        if(penetration > 0) {
            a->collisionCallback(manifold);
            b->collisionCallback(manifold);
            return 1;
        }
    }
    return 0;
}
void resolveCollision(manifold manifold) {
    if(manifold.a && manifold.b && manifold.a->shape && manifold.b->shape) {
        manifold.a->applyImpulse(manifold.fa);
        manifold.b->applyImpulse(manifold.fb);
    }
}

Object::Object(Shape* shape) {
    this->id = 0;
    this->family.pushBack("Object");
    this->world = NULL;
    this->shape = shape;
    if(this->shape) {
        this->shape->obj = this;
    }
    this->time = 0;
    this->pos = {0, 0};
    this->ori = {0, 0};
    this->vel = {0, 0};
    this->rot = {0, 0};
}
Object::Object(Shape* shape, V2f pos, V2f ori) : Object(shape) {
    this->pos = pos;
    this->ori = ori;
}
Object::~Object() {
    if(this->world) {
        this->world->remove(this->id);
    }
    if(this->shape) {
        delete this->shape;
    }
}
void Object::update(double delta) {
    if(this->shape) {
        this->shape->update(delta);
    }
    this->pos.x += cos(this->vel.y)*this->vel.x*delta;
    this->pos.y += sin(this->vel.y)*this->vel.x*delta;
    this->ori.y += rot.y;
//    if(this->world) {
//        this->vspd *= 1-this->world->friction*this->invmass*delta;
//    }
    this->time += delta;
}
void Object::render(Renderer* renderer) {
    if(this->shape) {
        this->shape->render(renderer);
    }
}
void Object::speak(Speaker* speaker) {
    if(this->shape) {
        this->shape->speak(speaker);
    }
}
V2f Object::getPos() {
    return {this->pos.x
           ,this->pos.y};
}
V2f Object::getOri() {
    return this->ori;
}
V2f Object::getVel() {
    return this->vel;
}
V2f Object::getRot() {
    return this->rot;
}
void Object::setAbsPos(V2f pos) {
    this->pos = pos;
}
void Object::setAbsOri(V2f ori) {
    this->ori = ori;
}
void Object::setAbsVel(V2f vel) {
    this->vel = vel;
}
void Object::setAbsRot(V2f rot) {
    this->rot = rot;
}
void Object::applyImpulse(V2f j) {
    float m;
    if(this->shape && this->shape->invmass) {
        m = this->shape->invmass;
    }else {
        m = 1;
    }
    this->vel = carteToPolar(polarToCarte({j.x*m, j.y})+polarToCarte(this->vel));
}
void Object::applyTorque(V2f t) {
    float m;
    if(this->shape && this->shape->invmass) {
        m = this->shape->invmass;
    }else {
        m = 1;
    }
    this->rot.y = this->rot.y+t.y*(1/t.x*m);
}
void Object::collisionCallback(manifold* manifold) {
    if(this->shape) {
        this->shape->collisionCallback(manifold);
    }
}
Object& Object::operator=(const Object& rvalue) {
    this->time = rvalue.time;
    this->pos = rvalue.pos;
    this->ori = rvalue.ori;
    this->vel = rvalue.vel;
    this->rot = rvalue.rot;
    return *this;
}

World::World() : Object(NULL){
    this->family.pushBack("World");
}
World::~World() {
}
void World::update(double delta) {
    Object::update(delta);
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(!this->objs.isFree(i)) {
            this->objs[i]->update(delta);
        }
    }
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(!this->objs.isFree(i)) {
            Object* a = this->objs[i];
            if(a->shape) {
                for(unsigned int j=i+1; j<this->objs.size(); j++) {
                    if(!this->objs.isFree(j)) {
                        Object* b = this->objs[j];
                        if(b->shape) {
                            manifold manifold;
                            checkCollision(&manifold, a, b);
                            resolveCollision(manifold);
                        }
                    }
                }
            }
        }
    }
    this->time += delta;
}
void World::render() {
    Renderer* renderer;
    unsigned int offset = 0;
    while((renderer = (Renderer*)this->getObject("Renderer", 1, offset))) {
        renderer->begin();
        for(unsigned int i=0; i<this->objs.size(); i++) {
            if(!this->objs.isFree(i)) {
                this->objs[i]->render(renderer);
            }
        }
        renderer->end();
        offset = renderer->id+1;
    }
}
void World::speak() {
    Speaker* speaker;
    unsigned int offset = 0;
    while((speaker = (Speaker*)this->getObject("Speaker", 1, offset))) {
        speaker->begin();
        for(unsigned int i=0; i<this->objs.size(); i++) {
            if(!this->objs.isFree(i)) {
                this->objs[i]->speak(speaker);
            }
        }
        speaker->end();
        offset = speaker->id+1;
    }
}
int World::add(Object* obj) {
    obj->world = this;
    return (obj->id = this->objs.add(obj));
}
bool World::remove(unsigned int id) {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(this->objs[i]->id == id && !this->objs.isFree(i)) {
            this->objs.remove(i);
            return 0;
        }
    }
    return 1;
}
bool World::destroy(unsigned int id) {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(this->objs[i]->id == id && !this->objs.isFree(i)) {
            delete this->objs[i];
            this->objs.getArray()[i].free = 1;
            return 0;
        }
    }
    return 1;
}
void World::destroyAll() {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(!this->objs.isFree(i)) {
            delete this->objs[i];
            this->objs.getArray()[i].free = 1;
        }
    }
}
Object* World::getObject(unsigned int id) {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(this->objs[i]->id == id && !this->objs.isFree(i)) {
            return this->objs[i];
        }
    }
    return NULL;
}
Object* World::getObject(const char* member, unsigned int level) {
    for(unsigned int i=0; i<this->objs.size(); i++) {
        if(level < this->objs[i]->family.size() && !strcmp(this->objs[i]->family[level], member) && !this->objs.isFree(i)) {
            return this->objs[i];
        }
    }
    return NULL;
}
Object* World::getObject(const char* member, unsigned int level, unsigned int offset) {
    for(unsigned int i=offset; i<this->objs.size(); i++) {
        if(level < this->objs[i]->family.size() && !strcmp(this->objs[i]->family[level], member) && !this->objs.isFree(i)) {
            return this->objs[i];
        }
    }
    return NULL;
}
World& World::operator=(const World& rvalue) {
    Object::operator=(rvalue); //Call base class = operator
    this->time = rvalue.time;
    this->objs = rvalue.objs;
    this->thingtypes = rvalue.thingtypes;
    return *this;
}

Shape::Shape() : Object(NULL) {
    this->family.pushBack("Shape");
    this->restitution = 1;
    this->invmass = 1;
    this->r = 255;
    this->g = 255;
    this->b = 255;
    this->a = 255;
    this->texture = NULL;
    this->texmode = 0;
}
Shape::Shape(float restitution, float mass) : Shape() {
    this->restitution = restitution;
    if(mass != 0) {
        this->invmass = 1/mass;
    }else {
        this->invmass = 0;
    }
}
Shape::Shape(float restitution, float mass, int r, int g, int b, int a) : Shape(restitution, mass) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    this->texmode = 0;
}
Shape::Shape(float restitution, float mass, SDL_Surface* texture) : Shape(restitution, mass) {
    this->texture = texture;
    this->texmode = 1;
    bool loop = 1;
    for(int x=0; x<this->texture->w && loop; x++) {
        for(int y=0; y<this->texture->h && loop; y++) {
            uint8_t r, g, b, a;
            SDL_GetRGBA(::getPixel(this->texture, x, y), this->texture->format, &r, &g, &b, &a);
            if(a<255) {
                this->a = 0;
                loop = 0;
            }
        }
    }
}
Shape::~Shape() {
}
void Shape::update(double delta) {
    Object::update(delta);
}
void Shape::render(Renderer* renderer) {
    Object::render(renderer);
}
void Shape::speak(Speaker* speaker) {
    Object::speak(speaker);
}
V2f Shape::getPos() {
    return {this->obj->getPos().x+this->pos.x*(float)cos(this->obj->getOri().y)
           ,this->obj->getPos().y+this->pos.y*(float)sin(this->obj->getOri().y)};
}
V2f Shape::getOri() {
    return this->ori+this->obj->getOri();
}
V2f Shape::getVel() {
    return this->vel+this->obj->getVel();
}
V2f Shape::getRot() {
    return this->rot+this->obj->getRot();
}
void Shape::setAbsPos(V2f pos) {
    this->pos = pos-this->obj->getPos();
}
void Shape::setAbsOri(V2f ori) {
    this->ori = ori-this->obj->getOri();
}
void Shape::setAbsVel(V2f vel) {
    this->vel = vel-this->obj->getVel();
}
void Shape::setAbsRot(V2f rot) {
    this->rot = rot-this->obj->getRot();
}
V2f Shape::getRadius(V2f angle) {
    return {0, angle.y};
}
V2f Shape::getNormal(V2f angle) {
    return {angle.y+(float)M_PI, 0};
}
Shape& Shape::operator=(const Shape& rvalue) {
    Object::operator=(rvalue);
    this->restitution = rvalue.restitution;
    this->invmass = rvalue.invmass;
    this->r = rvalue.r;
    this->g = rvalue.g;
    this->b = rvalue.b;
    this->a = rvalue.a;
    return *this;
}

Renderer::Renderer() : Object(NULL) {
    this->family.pushBack("Renderer");
    this->zoom = 1;
}
Renderer::~Renderer() {
}
void Renderer::begin() {
}
void Renderer::end() {
}
Renderer& Renderer::operator=(const Renderer& rvalue) {
    Object::operator=(rvalue);
    return *this;
}

Speaker::Speaker() : Object(NULL) {
    this->family.pushBack("Speaker");
    SDL_AudioSpec wanted;
    memset(&wanted, 0, sizeof(wanted));
    wanted.freq = 11025;
    wanted.format = AUDIO_U8;
    wanted.channels = 1;
    wanted.samples = 4096;
    wanted.callback = this->audioCallback;
    wanted.userdata = this;
    this->device = SDL_OpenAudioDevice(NULL, 0, &wanted, &this->spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if(!this->device) {
        return;
    }
    this->pauseAudio(0);
}
Speaker::~Speaker() {
    SDL_CloseAudioDevice(this->device);
}
void Speaker::begin() {
}
void Speaker::end() {
}
void Speaker::playAudio(const char* filename) {
    SDL_LockAudioDevice(this->device);
    uint8_t* buffer;
    uint32_t length;
    SDL_AudioSpec spec=this->spec;
    if(!SDL_LoadWAV(filename, &spec, &buffer, &length)) {
        SDL_UnlockAudioDevice(this->device);
        return;
    }
    SDL_AudioCVT cvt;
    SDL_BuildAudioCVT(&cvt, spec.format, spec.channels, spec.freq, this->spec.format, this->spec.channels, this->spec.freq);
    cvt.len = length;
    cvt.buf = buffer;
    SDL_ConvertAudio(&cvt);
    uint8_t* newbuffer = (uint8_t*)malloc(sizeof(uint8_t)*cvt.len_cvt);
    memcpy(newbuffer, buffer, sizeof(uint8_t)*cvt.len);
    this->sounds.add({newbuffer, (size_t)cvt.len, 0});
    SDL_FreeWAV(buffer);
    SDL_UnlockAudioDevice(this->device);
}
void Speaker::pauseAudio(bool pause) {
    SDL_PauseAudioDevice(this->device, pause);
}
void Speaker::audioCallback(void* userdata, uint8_t* stream, int size) {
    memset(stream, 0, size);
    if(!userdata) {
        return;
    }
    Speaker* speaker = (Speaker*)userdata;
    for(unsigned int i=0; i<speaker->sounds.size(); i++) {
        if(!speaker->sounds.getArray()[i].free) {
            if(!speaker->sounds[i].buffer || speaker->sounds[i].position>=speaker->sounds[i].length) {
                speaker->sounds.remove(i);
                continue;
            }
            size = (unsigned int)size>speaker->sounds[i].length ? speaker->sounds[i].length : size;
            SDL_MixAudioFormat(stream, speaker->sounds[i].buffer+speaker->sounds[i].position, speaker->spec.format, size, SDL_MIX_MAXVOLUME);
            speaker->sounds.getArray()[i].elem.position += size;
        }
    }
}

Thing::Thing() : Object(new Circle(1)) {
    this->family.pushBack("Thing");
}
Thing::~Thing() {
}
void Thing::update(double delta) {
    Object::update(delta);
}
void Thing::render(Renderer* renderer) {
    Object::render(renderer);
}
void Thing::speak(Speaker* speaker) {
    Object::speak(speaker);
}
