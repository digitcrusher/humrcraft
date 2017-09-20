/*
 * gane.hpp
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
#ifndef GAME_HPP
#define GAME_HPP
#include <stdlib.h>
#include "world.hpp"

class Thing;
class Gun;
class Projectile;

class Thing : public Object { //TODO: add model struct
    public:
        float maxhealth;
        float health;
        float damage;
        GLuint textureid;
        Thing *head, *rhand, *lhand, *chest, *back, *legs, *feet;
        void* data;
        size_t datasize;
        void (*constructorf)();
        void (*destructorf)();
        void (*updatef)(double delta);
        void (*renderf)(Renderer* renderer);
        void (*speakf)(Speaker* speaker);
        void (*usef)();
        void (*attackf)();
        void (*actionf)(const char* action);
        void (*collisionCallbackf)(manifold* manifold);
        Thing(void* data, size_t datasize, void (*constructorf)(), void (*destructorf)(), Shape* shape, float health, float damage, GLuint textureid);
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
        Gun(void* data, size_t datasize, void (*constructorf)(), void (*destructorf)(), Shape* shape, float health, GLuint textureid);
        virtual ~Gun();
        //virtual void use();
};
class Projectile : public Thing {
    public:
        float speed;
        Projectile(void* data, size_t datasize, void (*constructorf)(), void (*destructorf)(), Shape* shape, float health, float damage, float speed, GLuint textureid);
        virtual ~Projectile();
        virtual void update(double delta);
        virtual void render(Renderer* renderer);
        virtual void collisionCallback(manifold* manifold);
};

#endif
