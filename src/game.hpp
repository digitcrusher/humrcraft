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
#include <SDL2/SDL_image.h>
#include "world.hpp"
#include "maze.h"

namespace humrcraft {
    namespace game {
        class Thing;
        class Game;
        class Textures;
        class Tiles;
        class Gun;
        class Projectile;

        class Thing : public Object { //TODO: add model struct //TODO: add movement functions
            public:
                float maxhealth;
                float health;
                float damage;
                char* name;
                GLuint textureid;
                Thing *head, *rhand, *lhand, *chest, *back, *legs, *feet;
                void* data;
                int datasize;
                Thing* (*recreatef)(Thing* base, void* data, int datasize);
                void (*initf)(Thing* thing);
                void (*uninitf)(Thing* thing);
                void (*updatef)(Thing* thing, double delta);
                void (*renderf)(Thing* thing, Renderer* renderer);
                void (*speakf)(Thing* thing, Speaker* speaker);
                void (*usef)(Thing* thing);
                void (*attackf)(Thing* thing);
                void (*actionf)(Thing* thing, const char* action);
                void (*collisionCallbackf)(Thing* thing, struct manifold* manifold);
                Thing(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing), Shape* shape, float health, float damage, GLuint textureid);
                virtual ~Thing();
                virtual Thing* recreate(void* data, int datasize);
                virtual void update(double delta);
                virtual void render(Renderer* renderer);
                virtual void speak(Speaker* speaker);
                virtual void use();
                virtual void attack();
                virtual void action(const char* action);
                virtual void collisionCallback(struct manifold* manifold);
        };
        class Game : public World {
            public:
                utils::List<Thing*> things;
                Game();
                virtual ~Game();
                virtual int addThing(Thing* thing);
                virtual Thing* createThing(int x, void* data, int datasize);
        };
        class Resources : public Object { //TODO: implement wads
            public:
                /*struct texturesheet {
                    SDL_Surface* texturesheet;
                };
                struct texturesheet* createTextureSheet(const char* filename) {
                    struct texturesheet* texturesheet = (struct texturesheet*)malloc(sizeof(texturesheet));

                    return texturesheet;
                }
                SDL_Surface* getTexture(struct texturesheet* texturesheet) {
                }*/
                utils::Vector<GLuint> textures;
                Resources();
                virtual ~Resources();
                virtual void addTexture(SDL_Surface* surface);
                virtual GLuint getTexture(int n);
                virtual void loadResourcePack(const char* filename) {
                }
        };
        class Tiles : public Object {
            public:
                int sizex;
                int sizey;
                int* tiles;
                Resources* resources;
                struct world* world;
                int x, y, heading;
                struct lfr last;
                Tiles(Resources* resources);
                virtual ~Tiles();
                virtual void update(double delta);
                virtual void render(Renderer* renderer);
        };
        class Gun : public Thing {
            public:
                Projectile* sample;
                Gun(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing), Shape* shape, float health, GLuint textureid);
                virtual ~Gun();
                //virtual void use();
        };
        class Projectile : public Thing {
            public:
                float speed;
                Projectile(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing), Shape* shape, float health, float damage, float speed, GLuint textureid);
                virtual ~Projectile();
                virtual void update(double delta);
                virtual void render(Renderer* renderer);
                virtual void collisionCallback(struct manifold* manifold);
        };
    }
}

#endif
