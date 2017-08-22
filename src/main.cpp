/*
 * total number of lines as of 12.08.2017: 4073
 * main.cpp
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
#include <stdlib.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "utils.hpp"
#include "world.hpp"
#include "shapes.hpp"
#include "renderers.hpp"
#include "graphics.hpp"

World* world = new World();
Thing* hero;
SDLGLRenderer* renderer;
Speaker* speaker;
float gmulti = 1;
bool pause = 1;
bool running = 1;
void stop(int status) {
    TTF_Quit();
    SDL_Quit();
    exit(status);
}
void update(double delta) {
    world->speak();
    world->update(delta);
    for(unsigned int i=0; i<world->objs.size(); i++) {
        if(!world->objs.isFree(i)) {
            Object* a = world->objs[i];
            if(a->shape) {
                for(unsigned int j=i+1; j<world->objs.size(); j++) {
                    if(!world->objs.isFree(j)) {
                        Object* b = world->objs[j];
                        if(b->shape) {
                            V2f pos1 = {(float)fmax(a->getPos().x, b->getPos().x), (float)fmax(a->getPos().y, b->getPos().y)};
                            V2f pos2 = {(float)fmin(a->getPos().x, b->getPos().x), (float)fmin(a->getPos().y, b->getPos().y)};
                            float force = (6.674*10/pow(10, 11))/((1/a->shape->getInvMass())*(1/b->shape->getInvMass())/
                                          pow(sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2)), 2));
                            a->applyImpulse({gmulti*force*(float)delta, fatp(a->getPos(), b->getPos())});
                            b->applyImpulse({gmulti*force*(float)delta, fatp(b->getPos(), a->getPos())});
                        }
                    }
                }
            }
        }
    }
}
class Textures {
    public:
        Vector<GLuint> textures;
        Textures() {
        }
        virtual ~Textures() {
            glDeleteTextures(textures.size(), textures.getArray());
        }
        virtual void add(SDL_Surface* surface) {
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
        GLuint& operator[](unsigned int n) {
            return this->textures[n];
        }
};
Textures textures;
class Tiles : public Object {
    public:
        size_t sizex;
        size_t sizey;
        unsigned int* tiles;
        Tiles() : Object(NULL) {
            sizex = 10;
            sizey = 10;
            tiles = (unsigned int*)malloc(sizeof(unsigned int)*this->sizex*this->sizey);
            for(unsigned int x=0; x<sizex; x++) {
                for(unsigned int y=0; y<sizey; y++) {
                    tiles[this->sizex*y+x] = 1;
                }
            }
        }
        virtual void render(Renderer* renderer) {
            Object::render(renderer);
            if(!this->checkFamily(renderer, "SDLGLRenderer", 2)) return;
            glEnable(GL_TEXTURE_2D);
                glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    for(unsigned int y=0; y<sizey; y++) {
                        for(unsigned int x=0; x<sizex; x++) {
                            glBindTexture(GL_TEXTURE_2D, textures[this->tiles[this->sizex*y+x]]);
                            glBegin(GL_TRIANGLES);
                                V2f pos = ((SDLGLRenderer*)renderer)->glMapPos({x-(float)1/2-(float)this->sizex/2, y-(float)1/2-(float)this->sizey/2});
                                glTexCoord2f(0, 1);
                                glVertex3f(pos.x, pos.y, 1);
                                pos = ((SDLGLRenderer*)renderer)->glMapPos({x+(float)1/2-(float)this->sizex/2, y-(float)1/2-(float)this->sizey/2});
                                glTexCoord2f(1, 1);
                                glVertex3f(pos.x, pos.y, 1);
                                pos = ((SDLGLRenderer*)renderer)->glMapPos({x-(float)1/2-(float)this->sizex/2, y+(float)1/2-(float)this->sizey/2});
                                glTexCoord2f(0, 0);
                                glVertex3f(pos.x, pos.y, 0);

                                pos = ((SDLGLRenderer*)renderer)->glMapPos({x+(float)1/2-(float)this->sizex/2, y+(float)1/2-(float)this->sizey/2});
                                glTexCoord2f(1, 0);
                                glVertex3f(pos.x, pos.y, 1);
                                pos = ((SDLGLRenderer*)renderer)->glMapPos({x+(float)1/2-(float)this->sizex/2, y-(float)1/2-(float)this->sizey/2});
                                glTexCoord2f(1, 1);
                                glVertex3f(pos.x, pos.y, 1);
                                pos = ((SDLGLRenderer*)renderer)->glMapPos({x-(float)1/2-(float)this->sizex/2, y+(float)1/2-(float)this->sizey/2});
                                glTexCoord2f(0, 0);
                                glVertex3f(pos.x, pos.y, 0);
                            glEnd();
                        }
                    }
                glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        }
};
void render() {
    world->render();
    renderer->pos = hero->getPos();
}
int main(int argc, char** argv) {
    std::cout<<"humrcraft 1.0 Copyright (C) 2017 Karol \"digitcrusher\" Łacina\n";
    std::cout<<"This program comes with ABSOLUTELY NO WARRANTY.\n";
    std::cout<<"This is free software, and you are welcome to redistribute it\n";
    std::cout<<"under certain conditions.\n";
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr<<"SDL_Init error: "<<SDL_GetError()<<'\n';
        stop(1);
    }
    renderer = new SDLGLRenderer("humrcraft 1.0", 800, 600, 2, 1);
    world->add(renderer);
    /*speaker = new Speaker();
    world->add(speaker);*/
    textures.add(IMG_Load("./gfx/hero.png"));
    textures.add(IMG_Load("./gfx/bricks.png"));
    world->add(new Tiles());
    hero = new Thing(new Square(1), 1, textures[0]);
    world->add(hero);
    srand(rand()*time(0));
    for(int i=0; i<50; i++) {
        Object* obj = new Object(new Circle(rand()%4+1, {1, 1}));
        int color = rand()%128;
        obj->shape->r = 255-color;
        obj->shape->g = 127;
        obj->shape->b = 127+color;
        obj->shape->a = 255;
        srand(rand());
        obj->pos = {(float)(rand()%1000-500), (float)(rand()%1000-500)};
        obj->vel = {1, (float)(rand()%314/100)};
        world->add(obj);
    }
    long lastUpdate = getMS();
    double msPerUpdate = (double)1000/120;
    double deltaUpdate = 0;
    long lastRender = getMS();
    double msPerRender = (double)1000/60;
    double deltaRender = 0;
    int frames=0, updates=0;
    long lastTime = getMS();
    running = 1;
    while(running) {
        long now = getMS();
        deltaUpdate += (now-lastUpdate)/msPerUpdate;
        lastUpdate = now;
        if(deltaUpdate >= 1) {
            SDL_Event event;
            while(renderer->getEvent(&event)) {
                switch(event.type) {
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {
                            case SDLK_w:
                                hero->applyImpulse({0.1, hero->getOri().y});
                                break;
                            case SDLK_a:
                                hero->applyImpulse({0.1, hero->getOri().y+(float)M_PI/2});
                                break;
                            case SDLK_s:
                                hero->applyImpulse({0.1, hero->getOri().y+(float)M_PI});
                                break;
                            case SDLK_d:
                                hero->applyImpulse({0.1, hero->getOri().y-(float)M_PI/2});
                                break;
                            case SDLK_z:
                                gmulti *= 10;
                                break;
                            case SDLK_x:
                                gmulti /= 10;
                                break;
                            case SDLK_SCROLLLOCK:
                                renderer->zoom = 1;
                                break;
                            case SDLK_HOME:
                                hero->pos = {0, 0};
                                break;
                            case SDLK_DELETE:
                                gmulti = 1;
                                break;
                            case SDLK_END:
                                hero->vel = {0, 0};
                                break;
                            case SDLK_ESCAPE:
                                pause = !pause;
                                break;
                        }
                        break;
                    case SDL_MOUSEWHEEL:
                        renderer->zoom *= powf(2, event.wheel.y);
                        std::cout<<renderer->zoom<<'\n';
                        break;
                    /*case SDL_MOUSEBUTTONUP:
                        switch(event.button.button) {
                            case SDL_BUTTON_LEFT: {
                                    int color = rand()%128;
                                    Object* obj = new Object(new Circle(rand()%4+1, 0.5, 1, 255-color, 127, 127+color, 255));
                                    srand(rand());
                                    obj->pos = renderer->getPos({event.button.x, event.button.y});
                                    obj->vel = hero->getVel();
                                    world->add(obj);
                                }break;
                            case SDL_BUTTON_RIGHT: {
                                    for(unsigned int i=0; i<world->objs.size(); i++) {
                                        if(!world->objs.isFree(i)) {
                                            Object* a = world->objs[i];
                                            V2f pos = renderer->getPos({event.button.x, event.button.y});
                                            V2f pos1 = {(float)fmax(a->getPos().x, pos.x), (float)fmax(a->getPos().y, pos.y)};
                                            V2f pos2 = {(float)fmin(a->getPos().x, pos.x), (float)fmin(a->getPos().y, pos.y)};
                                            //14318181818.181818182 - Little Boy atomic bomb power
                                            //4184 - 1 g of TNT
                                            V2f j = {-4184/(float)(sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2))+1), fatp(a->getPos(), pos)};
                                            a->applyImpulse(j);
                                        }
                                    }
                                }break;
                        }
                        break;*/
                    case SDL_QUIT:
                        running = 0;
                        break;
                    case SDL_MOUSEMOTION:
                        if(!pause) {
                            hero->ori.y = fatp(hero->pos, renderer->getPos({event.motion.x, event.motion.y}));
                        }
                        break;
                }
                //frame->processEvent(event);
            }
            if(!pause) update(msPerUpdate/1000);
            updates++;
            deltaUpdate -= 1;
        }
        now = getMS();
        deltaRender += (now-lastRender)/msPerRender;
        lastRender = now;
        if(deltaRender >= 1) {
            render();
            frames++;
            deltaRender -= 1;
        }
        if(getMS()-lastTime >= 1000) {
            lastTime += 1000;
            std::cout<<updates<<" UPS, "<<frames<<" FPS\n";
            frames = 0;
            updates = 0;
        }
    }
    stop(0);
    return 0;
}
