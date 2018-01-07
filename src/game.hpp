/*
 * game.hpp
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
#include <SDL2/SDL_image.h>
#include "renderers.hpp"
#include "world.hpp"
#include "maze.h"

namespace humrcraft {
    namespace game {
        class Thing;
        class Block;
        class Level;
        class Game;
        class Resources;
        //class Tiles;
        class Gun;
        class Projectile;

        class Thing : public Object { //TODO: add model struct //TODO: add movement functions
            public:
                char* name;
                Resources* resources;
                int textureid;
                Thing *head, *rhand, *lhand, *chest, *back, *legs, *feet;
                void* data;
                int datasize;
                float maxhealth;
                float health;
                float damage;
                Thing* (*recreatef)(Thing* base, void* data, int datasize);
                void (*initf)(Thing* thing);
                void (*uninitf)(Thing* thing);
                void (*updatef)(Thing* thing, double delta);
                void (*interfacef)(Thing* thing, Interface* interface);
                void (*renderf)(Thing* thing, Renderer* renderer);
                void (*speakf)(Thing* thing, Speaker* speaker);
                void (*usef)(Thing* thing);
                void (*attackf)(Thing* thing);
                void (*actionf)(Thing* thing, const char* action);
                void (*collisionCallbackf)(Thing* thing, struct manifold* manifold);
                Thing(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing)/*, Shape* shape, float health, float damage, int textureid*/);
                virtual ~Thing();
                virtual Thing* recreate(void* data, int datasize);
                virtual void update(double delta);
                virtual void interface(Interface* interface);
                virtual void render(Renderer* renderer);
                virtual void speak(Speaker* speaker);
                virtual void use();
                virtual void attack();
                virtual void action(const char* action);
                virtual void collisionCallback(struct manifold* manifold);
                static Thing* defaultRecreatef(Thing* base, void* data, int datasize);
        };
        class Block : public Thing {
            public:
                Block(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing));
                virtual ~Block();
                static Thing* defaultRecreatef(Thing* base, void* data, int datasize);
        };
        class Level : public Thing { //TODO: add chunks
            public:
                Block** blocks;
                math::V2iPair size;
                math::V2f blocksize;
                math::V2iPair visibleblocks;
                Level(math::V2iPair size, math::V2f blocksize);
                virtual ~Level();
                virtual void update(double delta);
                virtual void interface(Interface* interface);
                virtual void render(Renderer* renderer);
                virtual void speak(Speaker* speaker);
                virtual math::V2i getSize();
                virtual Block** getBlock(math::V2i pos);
                virtual Block** getBlock(math::V2f pos);
                virtual math::V2i BlocksMapPos(math::V2f pos);
                virtual math::V2f BlocksGetPos(math::V2i pos);
                virtual void generateLevel();
        };
        class Game : public World {
            public:
                utils::List<Thing*> things;
                utils::List<Block*> blocks;
                Game();
                virtual ~Game();
                virtual int registerThing(Thing* thing);
                virtual int registerBlock(Block* block);
                virtual Thing* recreateThing(int x, void* data, int datasize);
                virtual Block* recreateBlock(int x, void* data, int datasize);
        };
        class Resources : public Interface { //TODO: implement wads
            public:
                utils::Vector<SDL_Surface*> textures;
                Resources();
                virtual ~Resources();
                virtual void addTexture(SDL_Surface* surface);
                virtual SDL_Surface* getTexture(int n);
        };
        /*class Tiles : public Object {
            public:
                int sizex;
                int sizey;
                int* tiles;
                Resources* resources;
                *//*struct world* world;
                int x, y, heading;
                struct lfr last;*//*
                Tiles(Resources* resources);
                virtual ~Tiles();
                virtual void update(double delta);
                virtual void render(Renderer* renderer);
        };*/
        class Gun : public Thing {
            public:
                Projectile* sample;
                Gun(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing));
                virtual ~Gun();
                //virtual void use();
        };
        class Projectile : public Thing {
            public:
                float speed;
                Projectile(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing));
                virtual ~Projectile();
                virtual void update(double delta);
                virtual void render(Renderer* renderer);
                virtual void collisionCallback(struct manifold* manifold);
        };
    }
}

#endif
