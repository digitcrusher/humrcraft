/*
 * gane.hpp
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
#ifndef GAME_HPP
#define GAME_HPP
#include <stdlib.h>
#include "world.hpp"

class Thing;
class Gun;
class Projectile;

class Thing : public Object { //TODO: add model struct //TODO: add recreate function
    public:
        float maxhealth;
        float health;
        float damage;
        GLuint textureid;
        Thing *head, *rhand, *lhand, *chest, *back, *legs, *feet;
        void* data;
        size_t datasize;
        void (*initf)(Thing* thing);
        void (*uninitf)();
        void (*updatef)(double delta);
        void (*renderf)(Renderer* renderer);
        void (*speakf)(Speaker* speaker);
        void (*usef)();
        void (*attackf)();
        void (*actionf)(const char* action);
        void (*collisionCallbackf)(manifold* manifold);
        Thing(void* data, size_t datasize, void (*initf)(), void (*uninitf)(), Shape* shape, float health, float damage, GLuint textureid);
        virtual ~Thing();
        virtual void update(double delta);
        virtual void render(Renderer* renderer);
        virtual void speak(Speaker* speaker);
        virtual void use();
        virtual void attack();
        virtual void action(const char* action);
        virtual void collisionCallback(manifold* manifold);
};
class Gun : public Thing {
    public:
        Projectile* sample;
        Gun(void* data, size_t datasize, void (*initf)(), void (*uninitf)(), Shape* shape, float health, GLuint textureid);
        virtual ~Gun();
        //virtual void use();
};
class Projectile : public Thing {
    public:
        float speed;
        Projectile(void* data, size_t datasize, void (*initf)(), void (*uninitf)(), Shape* shape, float health, float damage, float speed, GLuint textureid);
        virtual ~Projectile();
        virtual void update(double delta);
        virtual void render(Renderer* renderer);
        virtual void collisionCallback(manifold* manifold);
};

#endif
