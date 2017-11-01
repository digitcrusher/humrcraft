/*
 * world.hpp
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
#ifndef WORLD_HPP
#define WORLD_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <GL/gl.h>
#include "utils.hpp"
#include "math.hpp"
#include "maze.h"

//TODO: add Interface class

namespace humrcraft {
    class World;
    class Object;
    class Shape;
    class Renderer;
    class Speaker;

    struct material { //TODO: add enum with common materials
        float restitution;
        float density; //kg/m^2
    };
    struct sound {
        uint8_t* buffer;
        int length;
        int position;
    };
    struct manifold {
        void* data;
        int size;
        Object* a;
        Object* b;
        math::V2f angle;
        math::V2f ra;
        math::V2f rb;
        float penetration;
        float restitution; //restitution
        math::V2f na; //normal a
        math::V2f nb; //normal b
        float ma; //mass a
        float mb; //mass b
        float pa; //momentum a
        float pb; //momentum b
        math::V2f fa; //force for a
        math::V2f fb; //force for b
    };
    class Object { //TODO: move from 2d to 3d //TODO: change to momentum instead of velocity //TODO: improve shared objects
        public:
            int id;
            utils::Vector<const char*> family;
            World* world; //TODO: change to Object*
            Shape* shape; //TODO: make this a Vector
            float time; //Seconds
            bool stationary;
            bool shared;
            math::V2f pos; //Position meters, x-x position, y-y position
            math::V2f ori; //Orientation radians, y-orientation
            math::V2f vel; //Velocity m/s, radians, x-speed, y-direction
            math::V2f rot; //Rotation radians, y-rotation
            Object(Shape* shape);
            Object(Shape* shape, bool shared, bool stationary);
            virtual ~Object();
            virtual void update(double delta);
            virtual void render(Renderer* renderer);
            virtual void speak(Speaker* speaker);
            virtual math::V2f getPos();
            virtual math::V2f getOri();
            virtual math::V2f getVel();
            virtual math::V2f getRot();
            virtual void setAbsPos(math::V2f pos);
            virtual void setAbsOri(math::V2f ori);
            virtual void setAbsVel(math::V2f vel);
            virtual void setAbsRot(math::V2f rot);
            virtual void applyImpulse(math::V2f j);
            virtual void applyTorque(math::V2f t);
            virtual bool checkFamily(Object* obj, const char* member, int level);
            virtual void collisionCallback(struct manifold* manifold);
            virtual Object& operator=(const Object& rvalue);
    };
    class World : public Object {
        public:
            float time;
            utils::List<Object*> objs;
            utils::Vector<bool (*)(struct manifold* manifold, World* world)> collisions;
            World();
            virtual ~World();
            virtual void update(double delta);
            virtual void render();
            virtual void speak();
            virtual int add(Object* obj);
            virtual void addCollision(bool (*collision)(struct manifold* manifold, World* world));
            virtual bool remove(int id);
            virtual bool destroy(int id);
            virtual void destroyAll();
            virtual Object* getObject(int id);
            virtual Object* getObject(const char* member, int level);
            virtual Object* getObject(const char* member, int level, int offset);
            virtual World& operator=(const World& rvalue);
    };
    class Shape : public Object { //TODO: add more radius functions
        public:
            Object* obj;
            struct material material;
            uint8_t r, g, b, a;
            Shape();
            Shape(struct material material);
            Shape(struct material material, int r, int g, int b, int a);
            virtual ~Shape();
            virtual void update(double delta);
            virtual void render(Renderer* renderer);
            virtual void speak(Speaker* speaker);
            virtual math::V2f getPos();
            virtual math::V2f getOri();
            virtual math::V2f getVel();
            virtual math::V2f getRot();
            virtual void setAbsPos(math::V2f pos);
            virtual void setAbsOri(math::V2f ori);
            virtual void setAbsVel(math::V2f vel);
            virtual void setAbsRot(math::V2f rot);
            virtual float getVolume();
            virtual float getInvMass();
            virtual math::V2f getRadius(math::V2f angle);
            virtual math::V2f getNormal(math::V2f angle);
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
            utils::List<struct sound> sounds;
            Speaker();
            virtual ~Speaker();
            virtual void begin();
            virtual void end();
            virtual void playAudio(const char* filename);
            virtual void pauseAudio(bool pause);
            static void audioCallback(void* userdata, uint8_t* stream, int size);
    };

    bool checkCollision(struct manifold* manifold, Object* a, Object* b, World* world);
    void resolveCollision(struct manifold manifold);
}

#endif
