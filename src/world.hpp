/*
 * world.hpp
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
#ifndef WORLD_HPP
#define WORLD_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <GL/gl.h>
#include "utils.hpp"
#include "math.hpp"

class World;
class Object;
class Shape;
class Renderer;
class Speaker;
class Thing;

struct material { //TODO: add enum with common materials
    float restitution;
    float density; //kg/m^2
};
struct sound {
    uint8_t* buffer;
    size_t length;
    uint32_t position;
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
    float restitution; //restitution
    V2f na; //normal a
    V2f nb; //normal b
    float ma; //mass a
    float mb; //mass b
    float pa; //momentum a
    float pb; //momentum b
    V2f fa; //force for a
    V2f fb; //force for b
};
class Object { //TODO: move from 2d to 3d //TODO: change to momentum instead of velocity
    public:
        unsigned int id;
        Vector<const char*> family;
        World* world;
        Shape* shape;
        float time; //Seconds
        bool stationary;
        V2f pos; //Position meters, x-x position, y-y position
        V2f ori; //Orientation radians, y-orientation
        V2f vel; //Velocity m/s, radians, x-speed, y-direction
        V2f rot; //Rotation radians, y-rotation
        Object(Shape* shape);
        Object(Shape* shape, bool stationary);
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
        virtual bool checkFamily(Object* obj, const char* member, unsigned int level);
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
class Shape : public Object { //TODO: add more radius functions
    public:
        Object* obj;
        material mat;
        uint8_t r, g, b, a;
        Shape();
        Shape(material mat);
        Shape(material mat, int r, int g, int b, int a);
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
        virtual float getVolume();
        virtual float getInvMass();
        virtual V2f getRadius(V2f angle);
        virtual V2f getNormal(V2f angle);
        virtual Shape& operator=(const Shape& rvalue);
};
class Renderer : public Object { //TODO: add shaders
    public:
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
        float health;
        GLuint textureid;
        Thing(Shape* shape, float health, GLuint textureid);
        virtual ~Thing();
        virtual void update(double delta);
        virtual void render(Renderer* renderer);
        virtual void speak(Speaker* speaker);
        virtual void use();
        virtual void attack();
};

bool checkCollision(manifold* manifold, Object* a, Object* b);
void resolveCollision(manifold manifold);

#endif
