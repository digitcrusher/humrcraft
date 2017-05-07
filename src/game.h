/*
 * game.h
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
#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include <utils/utils.h>
#include <utils/math.h>
#include <SDL2/SDL.h>

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

class Game;
class Object;
class Shape;
class Renderer;
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
class Object {
    public:
        int id;
        char* classname;
        Game* world;
        Shape* shape;
        float time; //Seconds
        V2f pos; //Position meters
        V2f rot; //Rotation radians
        V2f vel; //Velocity m/s, radians, x-speed, y-direction
        V2f rspd; //Rotation speed radians
        Object(Shape* shape);
        Object(Shape* shape, V2f pos, V2f rot);
        virtual ~Object();
        virtual void update(double delta);
        virtual void render(Renderer* render);
        virtual void applyImpulse(V2f j);
        virtual Object& operator=(const Object& rvalue);
};
class Game {
    public:
        float time;
        List<Object*> objs;
        List<Thing*> thingtypes;
        Game();
        virtual ~Game();
        virtual void update(double delta);
        virtual void render();
        virtual int add(Object* obj);
        virtual bool remove(int id);
        virtual bool destroy(int id);
        virtual Object* getObject(int id);
        virtual Object* getObject(const char* classname);
        virtual void generate(int seed, V2f mins, V2f maxs);
        virtual unsigned int registerThing(Thing* thing);
        virtual Thing* createThing(unsigned int type);
        virtual Game& operator=(const Game& rvalue);
};
class Shape : public Object {
    public:
        Object* obj;
        float restitution;
        float invmass; //Inverse mass Kilograms
        int r, g, b;
        Shape();
        Shape(float restitution, float mass);
        Shape(float restitution, float mass, int r, int g, int b);
        virtual ~Shape();
        virtual void update(double delta);
        virtual void render(Renderer* render);
        virtual V2f getPos();
        virtual V2f getRot();
        virtual float getRadius(V2f angle);
        virtual V2f getNormal(V2f angle);
};
class Renderer : public Object {
    public:
        /*int fbefore;
        V2i size;
        float* zbuff;
        char* screen;*/
        int zoom; //Pixels per Meter
        SDL_Window* window;
        Renderer(const char* title, int w, int h, int zoom);
        virtual ~Renderer();
        virtual void begin();
        virtual void end();
        virtual bool getEvent(SDL_Event* event);
        virtual void draw(char c, V3f pos);
        virtual int mapRGB(int r, int g, int b);
        virtual int getPixel(V2f pos);
        virtual void drawPixel(V2f pos, int color);
        virtual void drawCircle(V2f pos, int r, int color);
        virtual void drawSquare(V2f pos, V2f rot, int sidelen, int color);
        virtual void drawEllipse(V2f pos1, V2f pos2, int color);
        virtual void drawLine(V2f pos1, V2f pos2, int color);
        virtual void drawRectangle(V2f pos1, V2f pos2, int color);
        virtual int drawImage(V2f pos, SDL_Surface* image);
        virtual void drawGraph(float (*func)(float), float zoom, V2f pos, int color);
        virtual Renderer& operator=(const Renderer& rvalue);
};
class Thing : public Object {
    public:
        int type;
        List<Thing*> bag;
        unsigned int bagcurr;
        genSettings generation;
        float deathcounter;
        float deathtime;
        float delay;
        float health;
        float attack;
        /*
        void (*construct)(Thing* thing);
        void (*destruct)(Thing* thing);
        */
        void (*updatef)(Thing* thing, double delta);
        void (*renderf)(Thing* thing, Renderer* render);
        Thing(float health, float attack, genSettings generation
             ,void (*updatef)(Thing* thing, double delta)
             ,void (*renderf)(Thing* thing, Renderer* context));
        virtual ~Thing();
        virtual void update(double delta);
        virtual void render(Renderer* render);
        virtual void hit();
        virtual void put();
        virtual void pick();
        virtual void drop();
        virtual Thing& operator=(const Thing& rvalue);
        static void defaultRenderf(Thing* thing, Renderer* render);
};

#endif
