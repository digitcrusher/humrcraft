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
        Thing::Thing(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing)) : Object(NULL) {
            this->family.pushBack("Thing");
            this->resources = NULL;
            this->textureid = 0;
            this->maxhealth = 0;
            this->health = this->maxhealth;
            this->damage = 0;
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
            this->interfacef = NULL;
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
        void Thing::interface(Interface* interface) {
            if(this->checkFamily(interface, "Resources", 2)) {
                this->resources = (Resources*)interface;
            }
            if(this->interfacef) {
                this->interfacef(this, interface);
            }
        }
        void Thing::render(Renderer* renderer) {
            Object::render(renderer);
            if(!this->checkFamily(renderer, "SDLRenderer", 3)) return;
            if(this->resources) {
                ((humrcraft::renderers::SDLRenderer*)renderer)->drawImage(this->getPos(), this->resources->getTexture(this->textureid));
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
        Thing* Thing::defaultRecreatef(Thing* base, void* data, int datasize) {
            Thing* thing = new Thing(data, datasize, base->recreatef, base->initf, base->uninitf);
            thing->updatef = base->updatef;
            thing->interfacef = base->interfacef;
            thing->renderf = base->renderf;
            thing->speakf = base->speakf;
            thing->usef = base->usef;
            thing->attackf = base->attackf;
            thing->actionf = base->actionf;
            thing->collisionCallbackf = base->collisionCallbackf;
            return thing;
        }

        Block::Block(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing)) : Thing(data, datasize, recreatef, initf, uninitf) {
            this->family.pushBack("Block");
        }
        Block::~Block() {
        }
        Thing* Block::defaultRecreatef(Thing* base, void* data, int datasize) {
            Block* block = new Block(data, datasize, base->recreatef, base->initf, base->uninitf);
            block->updatef = base->updatef;
            block->interfacef = base->interfacef;
            block->renderf = base->renderf;
            block->speakf = base->speakf;
            block->usef = base->usef;
            block->attackf = base->attackf;
            block->actionf = base->actionf;
            block->collisionCallbackf = base->collisionCallbackf;
            return block;
        }

        Level::Level(math::V2iPair size, math::V2f blocksize) : Thing(NULL, 0, humrcraft::game::Thing::defaultRecreatef, [](humrcraft::game::Thing* thing) {
                thing->textureid = 0;
            }, NULL) {
            this->family.pushBack("Level");
            this->size = size;
            this->blocksize = blocksize;
            this->visibleblocks = this->size;
            this->blocks = (Block**)malloc(sizeof(Block*)*(this->getSize().x*this->getSize().y));
            for(int i=0; i<this->getSize().x*this->getSize().y; i++) {
                this->blocks[i] = NULL;
            }
        }
        Level::~Level() {
            for(int i=0; i<this->getSize().x*this->getSize().y; i++) {
                if(this->blocks[i] && !this->blocks[i]->shared) {
                    delete this->blocks[i];
                }
            }
        }
        void Level::update(double delta) {
            Thing::update(delta);
            for(int x=this->visibleblocks.v1.x; x<this->visibleblocks.v2.x; x++) {
                for(int y=this->visibleblocks.v1.y; y<this->visibleblocks.v2.y; y++) {
                    Block* block = (*this->getBlock((math::V2i){x, y}));
                    if(block) {
                        block->update(delta);
                    }
                }
            }
        }
        void Level::interface(Interface* interface) {
            Thing::interface(interface);
            for(int x=this->visibleblocks.v1.x; x<this->visibleblocks.v2.x; x++) {
                for(int y=this->visibleblocks.v1.y; y<this->visibleblocks.v2.y; y++) {
                    Block* block = (*this->getBlock((math::V2i){x, y}));
                    if(block) {
                        block->interface(interface);
                    }
                }
            }
        }
        void Level::render(Renderer* renderer) {
            Thing::render(renderer);
            if(!this->checkFamily(renderer, "SDLRenderer", 3)) return;
            math::V2f start = ((humrcraft::renderers::SDLRenderer*)renderer)->SDLGetPos({0, ((humrcraft::renderers::SDLRenderer*)renderer)->buffer->h});
            math::V2f end = ((humrcraft::renderers::SDLRenderer*)renderer)->SDLGetPos({((humrcraft::renderers::SDLRenderer*)renderer)->buffer->w, 0});
            this->visibleblocks = (math::V2iPair){{(int)fmax(floor(start.x/this->blocksize.x), this->size.v1.x),
                                                   (int)fmax(floor(start.y/this->blocksize.y), this->size.v1.y)},
                                                  {(int)fmin(ceil(end.x/this->blocksize.x), this->size.v2.x),
                                                   (int)fmin(ceil(end.y/this->blocksize.y), this->size.v2.y)}};
            for(int x=this->visibleblocks.v1.x; x<this->visibleblocks.v2.x; x++) {
                for(int y=this->visibleblocks.v1.y; y<this->visibleblocks.v2.y; y++) {
                    Block* block = *this->getBlock((math::V2i){x, y});
                    if(block) {
                        block->pos = {(float)x*this->blocksize.x+this->blocksize.x/2, (float)y*this->blocksize.y+this->blocksize.y/2};
                        block->render(renderer);
                    }
                }
            }
        }
        void Level::speak(Speaker* speaker) {
            Thing::speak(speaker);
            for(int x=this->visibleblocks.v1.x; x<this->visibleblocks.v2.x; x++) {
                for(int y=this->visibleblocks.v1.y; y<this->visibleblocks.v2.y; y++) {
                    Block* block = (*this->getBlock((math::V2i){x, y}));
                    if(block) {
                        block->speak(speaker);
                    }
                }
            }
        }
        math::V2i Level::getSize() {
            return {this->size.v2.x-this->size.v1.x, this->size.v2.y-this->size.v1.y};
        }
        Block** Level::getBlock(math::V2i pos) {
            return this->blocks+(pos.x-this->size.v1.x+this->getSize().x)%this->getSize().x+(pos.y-this->size.v1.y+this->getSize().y)%this->getSize().y*this->getSize().x;
        }
        Block** Level::getBlock(math::V2f pos) {
            return this->getBlock(this->BlocksMapPos(pos));
        }
        math::V2i Level::BlocksMapPos(math::V2f pos) {
            return {(int)floor((pos-this->getPos()).x/this->blocksize.x), (int)floor((pos-this->getPos()).y/this->blocksize.y)};
        }
        math::V2f Level::BlocksGetPos(math::V2i pos) {
            return (math::V2f){(float)pos.x*this->blocksize.x, (float)pos.y*this->blocksize.y}+this->getPos();
        }
        void Level::generateLevel() {
            if(!this->checkFamily(this->world, "Game", 2)) return;
            for(int x=this->size.v1.x; x<this->size.v2.x; x++) {
                for(int y=this->size.v1.y; y<this->size.v2.y; y++) {
                    Block** block = this->getBlock((math::V2i){x, y});
                    int blocktype = rand()%3;
                    switch(blocktype) {
                        case 0:
                            blocktype = 0;
                            break;
                        case 1:
                            blocktype = 1;
                            break;
                        case 2:
                            blocktype = 3;
                            break;
                    }
                    *block = ((Game*)this->world)->recreateBlock(blocktype, NULL, 0);
                }
            }
            /*for(int i=0; i<this->getSize().x*this->getSize().y; i++) {
                this->blocks[i] = ((humrcraft::game::Game*)this->world)->recreateBlock(0, NULL, 0);
            }*/
        }

        Game::Game() : World() {
            this->family.pushBack("Game");
        }
        Game::~Game() {
        }
        int Game::registerThing(Thing* thing) {
            if(thing) {
                return this->things.add(thing);
            }
            return -1;
        }
        int Game::registerBlock(Block* block) {
            if(block) {
                return this->blocks.add(block);
            }
            return -1;
        }
        Thing* Game::recreateThing(int x, void* data, int datasize) {
            return this->things[x]->recreate(data, datasize);
        }
        Block* Game::recreateBlock(int x, void* data, int datasize) {
            return (Block*)this->blocks[x]->recreate(data, datasize);
        }

        Resources::Resources() : Interface() {
            this->family.pushBack("Resources");
        }
        Resources::~Resources() {
            for(int i=0; i<this->textures.size(); i++) {
                SDL_FreeSurface(this->getTexture(i));
            }
        }
        void Resources::addTexture(SDL_Surface* surface) {
            this->textures.pushBack(surface);
        }
        SDL_Surface* Resources::getTexture(int n) {
            return this->textures[n];
        }

        /*Tiles::Tiles(Resources* resources) : Object(NULL) {
            this->family.pushBack("Tiles");
            sizex = 1024;
            sizey = 1024;
            tiles = (int*)malloc(sizeof(int)*this->sizex*this->sizey);
            *//*world = createWorld(this->sizex, this->sizey);
            srand(::time(NULL));
            resetWorld(world, {Wall, 0});
            createLinearMaze(world, 0, 0, {Path, 0}, {Exit, 0}, {Path, 0}, 3, ::time(NULL));
            x = 0;
            y = 0;
            heading = 0;
            last = {{Wall, 0}, {Wall, 0}, {Wall, 0}, 0};*//*
            this->resources = resources;
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
            if(!this->checkFamily(renderer, "SDLRenderer", 3)) return;
            *//*if(solve(this->world, &this->x, &this->y, &this->heading, &this->last)) {
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
            }*//*
            math::V2f start = ((humrcraft::renderers::SDLRenderer*)renderer)->SDLGetPos({0, ((humrcraft::renderers::SDLRenderer*)renderer)->buffer->h});
            math::V2f end = ((humrcraft::renderers::SDLRenderer*)renderer)->SDLGetPos({((humrcraft::renderers::SDLRenderer*)renderer)->buffer->w, 0});
            for(int x=fmax(floor(start.x/0.5+0.5+(float)this->sizex/2-0.5), 0); x<fmin(ceil(end.x/0.5+0.5+(float)this->sizex/2-0.5), this->sizex); x++) {
                for(int y=fmax(floor(start.y/0.5+0.5+(float)this->sizey/2-0.5), 0); y<fmin(ceil(end.y/0.5+0.5+(float)this->sizey/2-0.5), this->sizey); y++) {
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawImage({x*0.5-(float)this->sizex/2*0.5+0.5/2, y*0.5-(float)this->sizey/2*0.5+0.5/2}, this->resources->getTexture(this->tiles[this->sizex*y+x]));
                }
            }
            *//*glBindTexture(GL_TEXTURE_2D, textures[0]);
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
            glEnd();*//*
        }*/

        Gun::Gun(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing)) : Thing(data, datasize, recreatef, initf, uninitf) {
            this->family.pushBack("Gun");
        }
        Gun::~Gun() {
        }
        /*void Gun::use() {
            if(this->world && this->sample) {
                this->world->add(new Projectile(this->sample->data, this->sample->datasize, this->sample->initf, this->sample->uninitf, this->sample->shape));
            }
        }*/

        Projectile::Projectile(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing)) : Thing(data, datasize, recreatef, initf, uninitf) {
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
