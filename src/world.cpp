/*
 * world.cpp
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
#include "world.h"
#include "graphics.h"
#include "shapes.h"
#include "renderers.h"

bool checkCollision(manifold* manifold, Object* a, Object* b) {
    if(a->shape && b->shape) {
        V2f angle;
        angle.y = (float)fatp(a->getPos(), b->getPos());
        float ra = a->shape->getRadius(angle);
        float rb = b->shape->getRadius(angle+(V2f){0, M_PI});
        V2f pos1 = {(float)fmax(a->getPos().x, b->getPos().x), (float)fmax(a->getPos().y, b->getPos().y)};
        V2f pos2 = {(float)fmin(a->getPos().x, b->getPos().x), (float)fmin(a->getPos().y, b->getPos().y)};
        float penetration = ra+rb-sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2));
        if(manifold) {
            manifold->a = a;
            manifold->b = b;
            manifold->angle = angle;
            manifold->penetration = penetration;
        }
        if(penetration > 0) {
            return 1;
        }
    }
    return 0;
}
bool resolveCollision(manifold manifold) {
    if(manifold.a->shape && manifold.b->shape) {
        if(manifold.penetration > 0) {
            float e, na, nb, ma, mb, pa, pb, f;

            e = fmin(manifold.a->shape->restitution, manifold.b->shape->restitution); //restitution
            na = manifold.a->shape->getNormal(manifold.angle).y; //normal a
            nb = manifold.b->shape->getNormal(manifold.angle+(V2f){0, M_PI}).y; //normal b
            if(manifold.a->shape->invmass) {
                ma = manifold.a->shape->invmass;
            }else {
                ma = 1;
            }
            if(manifold.b->shape->invmass) {
                mb = manifold.b->shape->invmass;
            }else {
                mb = 1;
            }
            pa = manifold.a->getVel().x/ma;
            pb = manifold.b->getVel().x/mb;

            f = (pa+pb)*cos(manifold.a->getVel().y-nb)*-(1+e)/2+manifold.penetration;

            manifold.a->applyImpulse({f, nb});

            f = (pa+pb)*cos(manifold.b->getVel().y-na)*-(1+e)/2+manifold.penetration;

            manifold.b->applyImpulse({f, na});
            return 1;
        }
    }
    return 0;
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
void World::generate(int seed, V2f mins, V2f maxs) {
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
unsigned int World::registerThing(Thing* thing) {
    return this->thingtypes.add(thing);
}
Thing* World::createThing(unsigned int type) {
    if(type < this->thingtypes.size()) {
        Thing* temp = this->thingtypes[type];
        Thing* thing = new Thing(temp->health, temp->attack, temp->generation
                                ,temp->updatef, temp->renderf);
        thing->type = type;
        return thing;
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
float Shape::getRadius(V2f angle) {
    return 0;
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

Thing::Thing(float health, float attack, genSettings generation
            ,void (*updatef)(Thing* thing, double delta)
            ,void (*renderf)(Thing* thing, Renderer* context)) : Object(new Circle(1)) {
    this->family.pushBack("Thing");
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
void Thing::render(Renderer* renderer) {
    Object::render(renderer);
    if(this->renderf) {
        this->renderf(this, renderer);
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
            if(((Thing*)obj)->health <= 0) {
                if(obj->pos == this->getPos()) { //TODO: Proper collision detection
                    this->bag.add((Thing*)obj);
                    this->world->objs.remove(obj->id);
                    return;
                }
            }
        }
    }
}
void Thing::drop() {
    if(bagcurr < this->bag.size()) {
        this->bag[bagcurr]->pos = this->getPos();
        this->bag[bagcurr]->ori = this->getOri();
        this->bag[bagcurr]->vel = this->getVel();
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
void Thing::defaultRenderf(Thing* thing, Renderer* renderer) {
    if(!(2 < renderer->family.size() && !strcmp(renderer->family[2], "ClassicRenderer"))) return;
    switch(thing->type) {
        case Tree:
            ((ClassicRenderer*)renderer)->draw('Y', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Log:
            ((ClassicRenderer*)renderer)->draw('=', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Stick:
            ((ClassicRenderer*)renderer)->draw('/', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Flower:
            ((ClassicRenderer*)renderer)->draw('*', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Grass:
            ((ClassicRenderer*)renderer)->draw('"', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Wheat:
            ((ClassicRenderer*)renderer)->draw('w', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Seeds:
            ((ClassicRenderer*)renderer)->draw('.', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Flint:
            ((ClassicRenderer*)renderer)->draw('^', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Stone:
            ((ClassicRenderer*)renderer)->draw('n', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Water:
            ((ClassicRenderer*)renderer)->draw('~', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Human:
            ((ClassicRenderer*)renderer)->draw('@', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Pig: //TODO
            ((ClassicRenderer*)renderer)->draw('m', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Cow: //TODO
            ((ClassicRenderer*)renderer)->draw('M', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Hamster: //TODO
            ((ClassicRenderer*)renderer)->draw('o', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case Bread:
            ((ClassicRenderer*)renderer)->draw('B', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case RawMeat:
            ((ClassicRenderer*)renderer)->draw('O', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        case CookedMeat:
            ((ClassicRenderer*)renderer)->draw('0', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
        default:
            ((ClassicRenderer*)renderer)->draw('#', {thing->getPos().x, thing->getPos().y, (float)!thing->generation.generate});
            break;
    }
}
