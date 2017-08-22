/*
 * world.h
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
#ifndef WORLD_H
#define WORLD_H
#include <utils/utils.h>
#include <utils/math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

template<typename T> struct ListElem {
    T elem;
    bool free;
};
template<typename T> class List {
    public:
        KL_Vector<ListElem<T>> elems;
        List();
        virtual ~List();
        virtual size_t size();
        virtual size_t capacity();
        virtual ListElem<T>* getArray();
        virtual int add(T elem);
        virtual bool remove(unsigned int x);
        virtual bool isFree(unsigned int x);
        virtual T* get(unsigned int x);
        virtual T operator[](const unsigned int& rvalue);
        virtual List<T>& operator=(const List<T>& rvalue);
};
template<typename T> List<T>::List() {
}
template<typename T> List<T>::~List() {
}
template<typename T> size_t List<T>::size() {
    return this->elems.size();
}
template<typename T> size_t List<T>::capacity() {
    return this->elems.capacity();
}
template<typename T> ListElem<T>* List<T>::getArray() {
    return this->elems.getArray();
}
template<typename T> int List<T>::add(T elem) {
    for(unsigned int i=0; i<this->elems.size(); i++) {
        if(this->elems.getArray()[i].free) {
            this->elems.getArray()[i].free = 0;
            this->elems.getArray()[i].elem = elem;
            return i;
        }
    }
    this->elems.pushBack({elem, 0});
    return this->elems.size()-1;
}
template<typename T> bool List<T>::remove(unsigned int x) {
    if(x >= 0 && x < this->elems.size()) {
        this->elems.getArray()[x].free = 1;
        return 0;
    }
    return 1;
}
template<typename T> bool List<T>::isFree(unsigned int x) {
    if(x >= 0 && x < this->elems.size()) {
        return this->elems.getArray()[x].free;
    }
    return 0;
}
template<typename T> T* List<T>::get(unsigned int x) {
    if(x >= 0 && x < this->elems.size()) {
        return &this->elems.getArray()[x].elem;
    }
    return NULL;
}
template<typename T> T List<T>::operator[](const unsigned int& rvalue) {
    return *this->get(rvalue);
}
template<typename T> List<T>& List<T>::operator=(const List<T>& rvalue) {
    this->elems = rvalue.elems;
    return *this;
}

class World;
class Object;
class Shape;
class Renderer;
class Speaker;
class Thing;

enum {
    //Organic
    Tree,
    Log,
    Stick,
    Flower,
    Grass,
    Wheat,
    Seeds,
    //Stone
    Flint,
    Stone,
    //Fluid
    Water, //TODO
    //Animal
    Human,
    Pig,
    Cow,
    Hamster,
    //Food
    Bread,
    RawMeat,
    CookedMeat
};
struct genSettings {
    bool generate;
    float occurence; //1 in 1/occurence chance
};
struct manifold {
    Object* a;
    Object* b;
    V2f angle;
    V2f ra;
    V2f rb;
    V2f starta;
    V2f enda;
    V2f tempstartb;
    V2f tempendb;
    V2f startb;
    V2f endb;
    float penetration;
    float e; //restitution
    V2f na; //normal a
    V2f nb; //normal b
    float ma; //mass a
    float mb; //mass b
    float pa; //momentum a
    float pb; //momentum b
    V2f fa; //force for a
    V2f fb; //force for b
};
struct sound {
    uint8_t* buffer;
    size_t length;
    uint32_t position;
};
class Object { //TODO: move from 2d to 3d
    public:
        unsigned int id;
        KL_Vector<const char*> family;
        World* world;
        Shape* shape;
        float time; //Seconds
        V2f pos; //Position meters, x-x position, y-y position
        V2f ori; //Orientation radians, y-orientation
        V2f vel; //Velocity m/s, radians, x-speed, y-direction
        V2f rot; //Rotation radians, y-rotation
        Object(Shape* shape);
        Object(Shape* shape, V2f pos, V2f rot);
        virtual ~Object();
        virtual void update(double delta);
        virtual void render(Renderer* renderer);
        virtual void speak(Speaker* speaker);
        virtual V2f getPos();
        virtual V2f getOri();
        virtual V2f getVel();
        virtual V2f getRot();
        virtual void setAbsPos(V2f pos);
        virtual void setAbsOri(V2f ori);
        virtual void setAbsVel(V2f vel);
        virtual void setAbsRot(V2f rot);
        virtual void applyImpulse(V2f j);
        virtual void applyTorque(V2f t);
        virtual void collisionCallback(manifold* manifold);
        virtual Object& operator=(const Object& rvalue);
};
class World : public Object {
    public:
        float time;
        List<Object*> objs;
        List<Thing*> thingtypes;
        World();
        virtual ~World();
        virtual void update(double delta);
        virtual void render();
        virtual void speak();
        virtual int add(Object* obj);
        virtual bool remove(unsigned int id);
        virtual bool destroy(unsigned int id);
        virtual void destroyAll();
        virtual Object* getObject(unsigned int id);
        virtual Object* getObject(const char* member, unsigned int level);
        virtual Object* getObject(const char* member, unsigned int level, unsigned int offset);
        virtual World& operator=(const World& rvalue);
};
class Shape : public Object {
    public:
        Object* obj;
        float restitution;
        float invmass; //Inverse mass Kilograms
        uint8_t r, g, b, a; //a affects transparency on texture
        SDL_Surface* texture;
        bool texmode; //0-solid, 1-textured
        Shape();
        Shape(float restitution, float mass);
        Shape(float restitution, float mass, int r, int g, int b, int a);
        Shape(float restitution, float mass, SDL_Surface* texture);
        virtual ~Shape();
        virtual void update(double delta);
        virtual void render(Renderer* renderer);
        virtual void speak(Speaker* speaker);
        virtual V2f getPos();
        virtual V2f getOri();
        virtual V2f getVel();
        virtual V2f getRot();
        virtual void setAbsPos(V2f pos);
        virtual void setAbsOri(V2f ori);
        virtual void setAbsVel(V2f vel);
        virtual void setAbsRot(V2f rot);
        virtual V2f getRadius(V2f angle);
        virtual V2f getNormal(V2f angle);
        virtual Shape& operator=(const Shape& rvalue);
};
class Renderer : public Object { //TODO: add shaders
    public:
        int zoom; //Pixels per Meter
        Renderer();
        virtual ~Renderer();
        virtual void begin();
        virtual void end();
        virtual Renderer& operator=(const Renderer& rvalue);
};
class Speaker : public Object { //TODO: add sound shaders
    public:
        SDL_AudioSpec spec;
        SDL_AudioDeviceID device;
        List<sound> sounds;
        Speaker();
        virtual ~Speaker();
        virtual void begin();
        virtual void end();
        virtual void playAudio(const char* filename);
        virtual void pauseAudio(bool pause);
        static void audioCallback(void* userdata, uint8_t* stream, int size);
};
class Thing : public Object {
    public:
        Thing();
        virtual ~Thing();
        virtual void update(double delta);
        virtual void render(Renderer* renderer);
        virtual void speak(Speaker* speaker);
};

bool checkCollision(manifold* manifold, Object* a, Object* b); //TODO: implement line intersection collision detection and collision callback
void resolveCollision(manifold manifold);

#endif
