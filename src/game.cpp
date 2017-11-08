/*
 * game.cpp
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
#include "game.hpp"
#include "renderers.hpp"

namespace humrcraft {
    namespace game {
        Thing::Thing(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing), Shape* shape, float health, float damage, GLuint textureid) : Object(shape) {
            this->family.pushBack("Thing");
            this->maxhealth = health;
            this->health = this->maxhealth;
            this->damage = damage;
            this->textureid = textureid;
            this->head = NULL;
            this->rhand = NULL;
            this->lhand = NULL;
            this->chest = NULL;
            this->back = NULL;
            this->legs = NULL;
            this->feet = NULL;
            this->data = data;
            this->datasize = datasize;
            this->recreatef = recreatef;
            this->initf = initf;
            this->uninitf = uninitf;
            this->updatef = NULL;
            this->renderf = NULL;
            this->speakf = NULL;
            this->usef = NULL;
            this->attackf = NULL;
            this->actionf = NULL;
            this->collisionCallbackf = NULL;
            if(this->initf) {
                this->initf(this);
            }
        }
        Thing::~Thing() {
            if(this->uninitf) {
                this->uninitf(this);
            }
        }
        Thing* Thing::recreate(void* data, int datasize) {
            if(this->recreatef) {
                return this->recreatef(this, data, datasize);
            }
            return NULL;
        }
        void Thing::update(double delta) {
            Object::update(delta);
            this->vel.x *= 0.99;
            if(this->updatef) {
                this->updatef(this, delta);
            }
        }
        void Thing::render(Renderer* renderer) {
            Object::render(renderer);
            if(!this->checkFamily(renderer, "SDLGLRenderer", 3)) return;
            if(this->textureid) {
                glBindTexture(GL_TEXTURE_2D, this->textureid);
                glEnable(GL_TEXTURE_2D);
                    glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        glBegin(GL_TRIANGLES);
                            math::V2f pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x-(float)1/2, this->getPos().y-(float)1/2});
                            glTexCoord2f(0, 1);
                            glVertex3f(pos.x, pos.y, 1);
                            pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x+(float)1/2, this->getPos().y-(float)1/2});
                            glTexCoord2f(1, 1);
                            glVertex3f(pos.x, pos.y, 1);
                            pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x-(float)1/2, this->getPos().y+(float)1/2});
                            glTexCoord2f(0, 0);
                            glVertex3f(pos.x, pos.y, 0);

                            pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x+(float)1/2, this->getPos().y+(float)1/2});
                            glTexCoord2f(1, 0);
                            glVertex3f(pos.x, pos.y, 1);
                            pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x+(float)1/2, this->getPos().y-(float)1/2});
                            glTexCoord2f(1, 1);
                            glVertex3f(pos.x, pos.y, 1);
                            pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x-(float)1/2, this->getPos().y+(float)1/2});
                            glTexCoord2f(0, 0);
                            glVertex3f(pos.x, pos.y, 0);
                        glEnd();
                    glDisable(GL_BLEND);
                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            if(this->renderf) {
                this->renderf(this, renderer);
            }
        }
        void Thing::speak(Speaker* speaker) {
            Object::speak(speaker);
            if(this->speakf) {
                this->speakf(this, speaker);
            }
        }
        void Thing::use() {
            if(this->usef) {
                this->usef(this);
            }
        }
        void Thing::attack() {
            if(this->attackf) {
                this->attackf(this);
            }
        }
        void Thing::action(const char* action) {
            if(!strcmp(action, "use")) {
                this->use();
            }else if(!strcmp(action, "attack")) {
                this->attack();
            }
            if(this->actionf) {
                this->actionf(this, action);
            }
        }
        void Thing::collisionCallback(struct manifold* manifold) {
            Object::collisionCallback(manifold);
            if(this->collisionCallbackf) {
                this->collisionCallbackf(this, manifold);
            }
        }

        Game::Game() {
        }
        Game::~Game() {
        }
        int Game::addThing(Thing* thing) {
            if(thing) {
                return this->things.add(thing);
            }
            return -1;
        }
        Thing* Game::createThing(int x, void* data, int datasize) {
            return (*this->things.get(x))->recreate(data, datasize);
        }

        Textures::Textures() : Object(NULL) {
        }
         Textures::~Textures() {
            glDeleteTextures(textures.size(), textures.getArray());
        }
         void Textures::add(SDL_Surface* surface) {
            GLuint id=0;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            int color = surface->format->BytesPerPixel==4?GL_RGBA:GL_RGB;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexImage2D(GL_TEXTURE_2D, 0, color, surface->w, surface->h, 0, color, GL_UNSIGNED_BYTE, surface->pixels);
            this->textures.pushBack(id);
            SDL_FreeSurface(surface);
        }
         GLuint& Textures::operator[](unsigned int n) {
            return this->textures[n];
        }

        Tiles::Tiles(Textures* textures) : Object(NULL) {
            sizex = 50;
            sizey = 50;
            tiles = (unsigned int*)malloc(sizeof(unsigned int)*this->sizex*this->sizey);
            /*world = createWorld(this->sizex, this->sizey);
            srand(::time(NULL));
            resetWorld(world, {Wall, 0});
            createLinearMaze(world, 0, 0, {Path, 0}, {Exit, 0}, {Path, 0}, 3, ::time(NULL));
            x = 0;
            y = 0;
            heading = 0;
            last = {{Wall, 0}, {Wall, 0}, {Wall, 0}, 0};*/
            this->textures = textures;
            for(int x=0; x<this->sizex; x++) {
                for(int y=0; y<this->sizey; y++) {
                    this->tiles[this->sizex*y+x] = rand()%2?4:7;
                }
            }
        }
         Tiles::~Tiles() {
            //deleteWorld(world);
            free(this->tiles);
        }
         void Tiles::update(double delta) {
            Object::update(delta);
        }
         void Tiles::render(Renderer* renderer) {
            Object::render(renderer);
            if(!this->checkFamily(renderer, "SDLGLRenderer", 3)) return;
            /*if(solve(this->world, &this->x, &this->y, &this->heading, &this->last)) {
                srand(::time(NULL));
                resetWorld(world, {Wall, 0});
                createLinearMaze(world, 0, 0, {Path, 0}, {Exit, 0}, {Path, 0}, 3, 0);
                x = 0;
                y = 0;
                heading = 0;
                last = {{Wall, 0}, {Wall, 0}, {Wall, 0}, 0};
            }
            for(unsigned int x=0; x<this->sizex; x++) {
                for(unsigned int y=0; y<this->sizey; y++) {
                    switch(getTile(world, x, y)->type) {
                        case Wall:
                            this->tiles[this->sizex*y+x] = 1;
                            break;
                        case Path:
                            this->tiles[this->sizex*y+x] = 2;
                            break;
                        case Exit:
                            this->tiles[this->sizex*y+x] = 3;
                            break;
                        default:
                            this->tiles[this->sizex*y+x] = 4;
                            break;
                    }
                }
            }*/
            glEnable(GL_TEXTURE_2D);
                glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    math::V2f start = ((renderers::SDLGLRenderer*)renderer)->GLGetPos({-1, 1});
                    math::V2f end = ((renderers::SDLGLRenderer*)renderer)->GLGetPos({1, -1});
                    for(int y=fmax(this->sizey-start.y-(float)this->sizey/2, 0); y<fmin(this->sizey-end.y-(float)this->sizey/2, this->sizey); y++) {
                        for(int x=fmax(start.x+(float)this->sizex/2, 0); x<fmin(end.x+(float)this->sizex/2, this->sizex); x++) {
                            glBindTexture(GL_TEXTURE_2D, (*textures)[this->tiles[this->sizex*y+x]]);
                            glBegin(GL_TRIANGLES);
                                math::V2f pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({x-(float)this->sizex/2, this->sizey-1-y-(float)this->sizey/2});
                                glTexCoord2f(0, 1);
                                glVertex3f(pos.x, pos.y, 1);
                                pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({x+1-(float)this->sizex/2, this->sizey-1-y-(float)this->sizey/2});
                                glTexCoord2f(1, 1);
                                glVertex3f(pos.x, pos.y, 1);
                                pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({x-(float)this->sizex/2, this->sizey-1-y+1-(float)this->sizey/2});
                                glTexCoord2f(0, 0);
                                glVertex3f(pos.x, pos.y, 0);

                                pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({x+1-(float)this->sizex/2, this->sizey-1-y+1-(float)this->sizey/2});
                                glTexCoord2f(1, 0);
                                glVertex3f(pos.x, pos.y, 1);
                                pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({x+1-(float)this->sizex/2, this->sizey-1-y-(float)this->sizey/2});
                                glTexCoord2f(1, 1);
                                glVertex3f(pos.x, pos.y, 1);
                                pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({x-(float)this->sizex/2, this->sizey-1-y+1-(float)this->sizey/2});
                                glTexCoord2f(0, 0);
                                glVertex3f(pos.x, pos.y, 0);
                            glEnd();
                        }
                    }
                    /*glBindTexture(GL_TEXTURE_2D, textures[0]);
                    glBegin(GL_TRIANGLES);
                        math::V2f pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->x-(float)this->sizex/2, this->sizey-1-this->y-(float)this->sizey/2});
                        glTexCoord2f(0, 1);
                        glVertex3f(pos.x, pos.y, 1);
                        pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->x+1-(float)this->sizex/2, this->sizey-1-this->y-(float)this->sizey/2});
                        glTexCoord2f(1, 1);
                        glVertex3f(pos.x, pos.y, 1);
                        pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->x-(float)this->sizex/2, this->sizey-1-this->y+1-(float)this->sizey/2});
                        glTexCoord2f(0, 0);
                        glVertex3f(pos.x, pos.y, 0);

                        pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->x+1-(float)this->sizex/2, this->sizey-1-this->y+1-(float)this->sizey/2});
                        glTexCoord2f(1, 0);
                        glVertex3f(pos.x, pos.y, 1);
                        pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->x+1-(float)this->sizex/2, this->sizey-1-this->y-(float)this->sizey/2});
                        glTexCoord2f(1, 1);
                        glVertex3f(pos.x, pos.y, 1);
                        pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->x-(float)this->sizex/2, this->sizey-1-this->y+1-(float)this->sizey/2});
                        glTexCoord2f(0, 0);
                        glVertex3f(pos.x, pos.y, 0);
                    glEnd();*/
                glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        }

        Gun::Gun(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing), Shape* shape, float health, GLuint textureid) :
            Thing(data, datasize, recreatef, initf, uninitf, shape, health, 0, textureid) {
            this->family.pushBack("Gun");
        }
        Gun::~Gun() {
        }
        /*void Gun::use() {
            if(this->world && this->sample) {
                this->world->add(new Projectile(this->sample->data, this->sample->datasize, this->sample->initf, this->sample->uninitf, this->sample->shape));
            }
        }*/

        Projectile::Projectile(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing), Shape* shape, float health, float damage, float speed, GLuint textureid) :
        Thing(data, datasize, recreatef, initf, uninitf, shape, health, damage, textureid) {
            this->family.pushBack("Projectile");
            this->speed = speed;
        }
        Projectile::~Projectile() {
        }
        void Projectile::update(double delta) {
            Thing::update(delta);
        }
        void Projectile::render(Renderer* renderer) {
            Thing::render(renderer);
            glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(1, 1, 0, 1);
                glBegin(GL_LINES);
                    math::V2f pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x, this->getPos().y});
                    glTexCoord2f(0, 1);
                    glVertex3f(pos.x, pos.y, 1);
                    pos = ((renderers::SDLGLRenderer*)renderer)->GLMapPos({this->getPos().x, this->getPos().y});
                    glTexCoord2f(1, 1);
                    glVertex3f(pos.x, pos.y, 1);
                glEnd();
            glDisable(GL_BLEND);
        }
        void Projectile::collisionCallback(struct manifold* manifold) {
            Thing::collisionCallback(manifold);
            Object* hit = manifold->a==this?manifold->b:manifold->a;
            if(hit->checkFamily(hit, "Thing", 1)) {
                ((Thing*)hit)->health -= this->damage;
            }
            if(this->world) {
                this->world->remove(this->id);
            }
            if(!this->shared) {
                delete this;
            }
        }
    }
}
