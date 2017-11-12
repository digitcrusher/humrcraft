/*
 * world.cpp
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
#include <iostream>
#include "renderers.hpp"
#include "world.hpp"
#include "shapes.hpp"

namespace humrcraft {
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
        this->stationary = 0;
        this->shared = 0;
    }
    Object::Object(Shape* shape, bool shared, bool stationary) : Object(shape) {
        this->stationary = stationary;
        this->shared = shared;
    }
    Object::~Object() {
        if(this->world) {
            this->world->remove(this->id);
        }
        if(this->shape && !this->shape->shared) {
            delete this->shape;
        }
    }
    void Object::update(double delta) {
        this->time += delta;
        if(this->shape) {
            this->shape->update(delta);
        }
        if(!this->stationary) {
            this->pos.x += cos(this->vel.y)*this->vel.x*delta;
            this->pos.y += sin(this->vel.y)*this->vel.x*delta;
            this->ori.y += rot.y;
        }
    //    if(this->world) {
    //        this->vspd *= 1-this->world->friction*this->getInvMass()*delta;
    //    }
    }
    void Object::interface(Interface* interface) {
        if(this->shape) {
            this->shape->interface(interface);
        }
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
    math::V2f Object::getPos() {
        return {this->pos.x
               ,this->pos.y};
    }
    math::V2f Object::getOri() {
        return this->ori;
    }
    math::V2f Object::getVel() {
        return this->vel;
    }
    math::V2f Object::getRot() {
        return this->rot;
    }
    void Object::setAbsPos(math::V2f pos) {
        this->pos = pos;
    }
    void Object::setAbsOri(math::V2f ori) {
        this->ori = ori;
    }
    void Object::setAbsVel(math::V2f vel) {
        this->vel = vel;
    }
    void Object::setAbsRot(math::V2f rot) {
        this->rot = rot;
    }
    void Object::applyImpulse(math::V2f j) {
        if(this->shape && !isinf(j.x)) {
            this->vel = math::carteToPolar(math::polarToCarte({j.x*(isinf(this->shape->getInvMass())?1:this->shape->getInvMass()), j.y})+math::polarToCarte(this->vel));
        }
    }
    void Object::applyTorque(math::V2f t) {
        if(this->shape && !isinf(t.x)) {
            this->rot.y = this->rot.y+t.y*(1/t.x*(isinf(this->shape->getInvMass())?1:this->shape->getInvMass()));
        }
    }
    bool Object::checkFamily(Object* obj, const char* member, int level) {
        return level < obj->family.size() && !strcmp(obj->family[level], member);
    }
    void Object::collisionCallback(struct manifold* manifold) {
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
        this->collisions.pushBack(humrcraft::shapes::circleCircle);
        this->collisions.pushBack(humrcraft::shapes::rectangleRectangle);
    }
    World::~World() {
    }
    void World::update(double delta) {
        Object::update(delta);
        for(int i=0; i<this->objs.size(); i++) {
            if(!this->objs.isFree(i)) {
                this->objs[i]->update(delta);
            }
        }
        for(int i=0; i<this->objs.size(); i++) {
            if(!this->objs.isFree(i)) {
                Object* a = this->objs[i];
                if(a->shape) {
                    for(int j=i+1; j<this->objs.size(); j++) {
                        if(!this->objs.isFree(j)) {
                            Object* b = this->objs[j];
                            if(b->shape) {
                                struct manifold manifold;
                                checkCollision(&manifold, a, b, this);
                                resolveCollision(manifold);
                            }
                        }
                    }
                }
            }
        }
    }
    void World::interface() {
        Interface* interface;
        int offset = 0;
        while((interface = (Interface*)this->getObject("Interface", 1, offset))) {
            if(this->checkFamily(interface, "Renderer", 2) || this->checkFamily(interface, "Speaker", 2)) {
                continue;
            }
            interface->begin();
            for(int i=0; i<this->objs.size(); i++) {
                if(!this->objs.isFree(i)) {
                    this->objs[i]->interface(interface);
                }
            }
            interface->end();
            offset = interface->id+1;
        }
    }
    void World::render() {
        Renderer* renderer;
        int offset = 0;
        while((renderer = (Renderer*)this->getObject("Renderer", 2, offset))) {
            renderer->begin();
            for(int i=0; i<this->objs.size(); i++) {
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
        int offset = 0;
        while((speaker = (Speaker*)this->getObject("Speaker", 2, offset))) {
            speaker->begin();
            for(int i=0; i<this->objs.size(); i++) {
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
    void World::addCollision(bool (*collision)(struct manifold* manifold, World* world)) {
        this->collisions.pushBack(collision);
    }
    bool World::remove(int id) {
        for(int i=0; i<this->objs.size(); i++) {
            if(this->objs[i]->id == id && !this->objs.isFree(i)) {
                this->objs.remove(i);
                return 0;
            }
        }
        return 1;
    }
    bool World::destroy(int id) {
        for(int i=0; i<this->objs.size(); i++) {
            if(this->objs[i]->id == id && !this->objs.isFree(i) && !this->objs[i]->shared) {
                delete this->objs[i];
                this->objs.getArray()[i].free = 1;
                return 0;
            }
        }
        return 1;
    }
    void World::destroyAll() {
        for(int i=0; i<this->objs.size(); i++) {
            if(!this->objs.isFree(i) && !this->objs[i]->shared) {
                delete this->objs[i];
                this->objs.getArray()[i].free = 1;
            }
        }
    }
    Object* World::getObject(int id) {
        for(int i=0; i<this->objs.size(); i++) {
            if(this->objs[i]->id == id && !this->objs.isFree(i)) {
                return this->objs[i];
            }
        }
        return NULL;
    }
    Object* World::getObject(const char* member, int level) {
        for(int i=0; i<this->objs.size(); i++) {
            if(this->checkFamily(this->objs[i], member, level) && !this->objs.isFree(i)) {
                return this->objs[i];
            }
        }
        return NULL;
    }
    Object* World::getObject(const char* member, int level, int offset) {
        for(int i=offset; i<this->objs.size(); i++) {
            if(this->checkFamily(this->objs[i], member, level) && !this->objs.isFree(i)) {
                return this->objs[i];
            }
        }
        return NULL;
    }
    World& World::operator=(const World& rvalue) {
        Object::operator=(rvalue); //Call base class = operator
        this->time = rvalue.time;
        this->objs = rvalue.objs;
        return *this;
    }

    Shape::Shape() : Object(NULL) {
        this->family.pushBack("Shape");
        this->material = {0, 0};
        this->r = 255;
        this->g = 255;
        this->b = 255;
        this->a = 255;
    }
    Shape::Shape(struct material material) : Shape() {
        this->material = material;
    }
    Shape::Shape(struct material material, int r, int g, int b, int a) : Shape(material) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    Shape::~Shape() {
    }
    void Shape::update(double delta) {
        Object::update(delta);
    }
    void Shape::interface(Interface* interface) {
        Object::interface(interface);
    }
    void Shape::render(Renderer* renderer) {
        Object::render(renderer);
    }
    void Shape::speak(Speaker* speaker) {
        Object::speak(speaker);
    }
    math::V2f Shape::getPos() {
        return {this->obj->getPos().x+this->pos.x*(float)cos(this->obj->getOri().y)
               ,this->obj->getPos().y+this->pos.y*(float)sin(this->obj->getOri().y)};
    }
    math::V2f Shape::getOri() {
        return this->ori+this->obj->getOri();
    }
    math::V2f Shape::getVel() {
        return this->vel+this->obj->getVel();
    }
    math::V2f Shape::getRot() {
        return this->rot+this->obj->getRot();
    }
    void Shape::setAbsPos(math::V2f pos) {
        this->pos = pos-this->obj->getPos();
    }
    void Shape::setAbsOri(math::V2f ori) {
        this->ori = ori-this->obj->getOri();
    }
    void Shape::setAbsVel(math::V2f vel) {
        this->vel = vel-this->obj->getVel();
    }
    void Shape::setAbsRot(math::V2f rot) {
        this->rot = rot-this->obj->getRot();
    }
    float Shape::getVolume() {
        return 1;
    }
    float Shape::getInvMass() {
        return 1/(this->getVolume()*this->material.density);
    }
    math::V2f Shape::getRadius(math::V2f angle) {
        return {0, angle.y};
    }
    math::V2f Shape::getNormal(math::V2f angle) {
        return {angle.y+(float)math::pi, 0};
    }
    Shape& Shape::operator=(const Shape& rvalue) {
        Object::operator=(rvalue);
        this->material = material;
        this->r = rvalue.r;
        this->g = rvalue.g;
        this->b = rvalue.b;
        this->a = rvalue.a;
        return *this;
    }

    Interface::Interface() : Object(NULL) {
        this->family.pushBack("Interface");
    }
    Interface::~Interface() {
    }
    void Interface::begin() {
    }
    void Interface::end() {
    }
    Interface& Interface::operator=(const Interface& rvalue) {
        Object::operator=(rvalue);
        return *this;
    }

    Renderer::Renderer() : Interface() {
        this->family.pushBack("Renderer");
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

    Speaker::Speaker() : Interface() {
        this->family.pushBack("Speaker");
        SDL_AudioSpec wanted;
        memset(&wanted, 0, sizeof(wanted));
        wanted.freq = 11025;
        wanted.format = AUDIO_U8;
        wanted.channels = 1;
        wanted.samples = 4096;
        wanted.callback = this->audioCallback;
        wanted.userdata = this;
        if(!(this->device = SDL_OpenAudioDevice(NULL, 0, &wanted, &this->spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE))) {
            std::cerr<<"SDL_OpenAudioDevice error: "<<SDL_GetError()<<'\n';
            throw;
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
        this->sounds.add({newbuffer, (int)cvt.len, 0});
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
        for(int i=0; i<speaker->sounds.size(); i++) {
            if(!speaker->sounds.getArray()[i].free) {
                if(!speaker->sounds[i].buffer || speaker->sounds[i].position>=speaker->sounds[i].length) {
                    speaker->sounds.remove(i);
                    continue;
                }
                size = size>speaker->sounds[i].length ? speaker->sounds[i].length : size;
                SDL_MixAudioFormat(stream, speaker->sounds[i].buffer+speaker->sounds[i].position, speaker->spec.format, size, SDL_MIX_MAXVOLUME);
                speaker->sounds.getArray()[i].elem.position += size;
            }
        }
    }

    bool checkCollision(struct manifold* output, Object* a, Object* b, World* world) {
        if(a && b && a->shape && b->shape) {
            struct manifold manifold;
            memset(&manifold, 0, sizeof(struct manifold));
            manifold.a = a;
            manifold.b = b;
            manifold.angle = fatp(manifold.a->getPos(), manifold.b->getPos());
            manifold.ra = manifold.a->shape->getRadius(manifold.angle);
            manifold.rb = manifold.b->shape->getRadius(manifold.angle+(math::V2f){0, (float)math::pi});
            manifold.restitution = fmin(manifold.a->shape->material.restitution, manifold.b->shape->material.restitution);
            manifold.na = manifold.a->shape->getNormal(manifold.ra);
            manifold.nb = manifold.b->shape->getNormal(manifold.rb);
            manifold.ma = manifold.a->shape->getInvMass();
            manifold.mb = manifold.b->shape->getInvMass();
            if(isinf(manifold.ma)) {
                manifold.ma = 1;
            }
            if(isinf(manifold.mb)) {
                manifold.mb = 1;
            }
            manifold.pa = manifold.a->getVel().x/manifold.ma;
            manifold.pb = manifold.b->getVel().x/manifold.mb;
            int i;
            for(i=0; i<world->collisions.size(); i++) {
                if(!world->collisions[i] || world->collisions[i](&manifold, world)) {
                    continue;
                }
                break;
            }
            if(i==world->collisions.size()) {
                math::V2f starta;
                math::V2f enda;
                math::V2f tempstartb;
                math::V2f tempendb;
                math::V2f startb;
                math::V2f endb;
                starta.x = 0;
                starta.y = 0;
                enda.x = manifold.ra.x;
                enda.y = 0;
                tempstartb.x = cos(-(manifold.ra.y))*(manifold.b->shape->getPos().x-manifold.a->shape->getPos().x)-sin(-(manifold.ra.y))*(manifold.b->shape->getPos().y-manifold.a->shape->getPos().y);
                tempstartb.y = sin(-(manifold.ra.y))*(manifold.b->shape->getPos().x-manifold.a->shape->getPos().x)+cos(-(manifold.ra.y))*(manifold.b->shape->getPos().y-manifold.a->shape->getPos().y);
                tempendb.x = cos(manifold.rb.y-manifold.ra.y)*manifold.rb.x+tempstartb.x;
                tempendb.y = sin(manifold.rb.y-manifold.ra.y)*manifold.rb.x+tempstartb.y;
                startb.x = fmin(tempstartb.x, tempendb.x);
                startb.y = fmin(tempstartb.y, tempendb.y);
                endb.x = fmax(tempstartb.x, tempendb.x);
                endb.y = fmax(tempstartb.y, tempendb.y);
                if(startb.x <= enda.x && endb.x >= starta.x && startb.y <= starta.y && endb.y >= starta.y) {
                    manifold.penetration = 1;
                }
                manifold.size = sizeof(math::V2f)*6;
                manifold.data = malloc(manifold.size);
                *(math::V2f*)((char*)manifold.data+sizeof(math::V2f)*0) = starta;
                *(math::V2f*)((char*)manifold.data+sizeof(math::V2f)*1) = enda;
                *(math::V2f*)((char*)manifold.data+sizeof(math::V2f)*2) = tempstartb;
                *(math::V2f*)((char*)manifold.data+sizeof(math::V2f)*3) = tempendb;
                *(math::V2f*)((char*)manifold.data+sizeof(math::V2f)*4) = startb;
                *(math::V2f*)((char*)manifold.data+sizeof(math::V2f)*5) = endb;
            }
            manifold.fa = {(manifold.pa+manifold.pb)*(float)cos((manifold.a->getVel()-manifold.nb).y)*-(1+manifold.restitution)/2+manifold.penetration, manifold.nb.y};
            manifold.fb = {(manifold.pa+manifold.pb)*(float)cos((manifold.b->getVel()-manifold.na).y)*-(1+manifold.restitution)/2+manifold.penetration, manifold.na.y};
            if(output) {
                memcpy(output, &manifold, sizeof(struct manifold));
            }
            if(manifold.penetration > 0) {
                a->collisionCallback(&manifold);
                b->collisionCallback(&manifold);
                return 1;
            }
        }
        return 0;
    }
    void resolveCollision(struct manifold manifold) {
        if(manifold.a && manifold.b && manifold.a->shape && manifold.b->shape) {
            if(manifold.penetration > 0) {
                manifold.a->applyImpulse(manifold.fa);
                manifold.b->applyImpulse(manifold.fb);
            }
            if(manifold.data) {
                free(manifold.data);
            }
        }
    }
}
