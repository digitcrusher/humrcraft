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
#include <fstream>
#include <utils/utils.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "world.h"
#include "shapes.h"
#include "renderers.h"
#include "graphics.h"
#include "gui.h"
#include "language.h"
#include "nbp.h"
#define RENDERER 0 //0 - SDLRenderer, 1 - Raycaster

/*GUIProgressBar* progressbar;
GUIFrame* frame;*/
World* world = new World();
Object* hero;
#if RENDERER == 1
Raycaster* renderer;
#else
SDLRenderer* renderer;
#endif
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
                            float force = (6.674*10/pow(10, 11))/((1/a->shape->invmass)*(1/b->shape->invmass)/
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
void render() {
#if RENDERER == 1
    SDL_SetRelativeMouseMode((SDL_bool)!pause);
#endif
    world->render();
    //frame->render(SDL_GetWindowSurface(renderer->window));
    //SDL_UpdateWindowSurface(renderer->window);
}
int main(int argc, char** argv) {
    std::cout<<"humrcraft 1.0 Copyright (C) 2017 Karol \"digitcrusher\" Łacina\n";
    std::cout<<"This program comes with ABSOLUTELY NO WARRANTY.\n";
    std::cout<<"This is free software, and you are welcome to redistribute it\n";
    std::cout<<"under certain conditions.\n";
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cout<<"SDL_Init error: "<<SDL_GetError()<<'\n';
        stop(1);
    }
    if(TTF_Init()) {
        std::cout<<"TTF_Init error: "<<TTF_GetError()<<'\n';
        stop(1);
    }
    /*NBP_node* start = NBP_createNode(0, 0);
    NBP_node* mid0 = NBP_createNode(2, 2);
    NBP_node* mid1 = NBP_createNode(0, 5);
    NBP_node* goal = NBP_createNode(5, 5);
    NBP_connect(start, mid0);
    NBP_connect(mid0, goal);
    NBP_connect(start, mid1);
    NBP_connect(mid1, goal);
    NBP_path* path = NBP_TreePathFind(start, goal);
    NBP_destroyNode(start);
    NBP_destroyNode(mid0);
    NBP_destroyNode(mid1);
    NBP_destroyNode(goal);
    NBP_destroyPath(path);*/
    Scope scope;
    std::ifstream file("cfg.cipl");
    file.seekg(0, file.end);
    int size = file.tellg();
    file.seekg(0, file.beg);
    char* buff = (char*)malloc(sizeof(char)*(size+1));
    file.readsome(buff, size);
    buff[size] = '\0';
    scope.execute(buff);
    free(buff);
    file.close();
    /*TTF_Font* font = TTF_OpenFont("gfx/fonts/FSEX300.ttf", 16);
    frame = (GUIFrame*)(new GUIFrame())->setBGColor({191, 191, 191, 255})->setBounds(0, 0, 100, 100);
    progressbar = (GUIProgressBar*)(new GUIProgressBar())->setBounds(0, 50, 100, 50)->setBGColor({191, 191, 191, 255})->setFGColor({0, 255, 0, 255});
    GUIButton* widget = (GUIButton*)(new GUIButton())->setBGColor({191, 191, 191, 255})->setBounds(0, 0, 100, 50)->setEventListener(
    [](int state) {
        if(state == Release) {
            progressbar->setProgress(progressbar->progress+0.1);
        }
    });
    widget->add((new GUILabel(font, "GUIButton"))->setBounds(0, 0, 100, 50));
    frame->add(progressbar);
    frame->add(widget);*/
#if RENDERER == 1
    renderer = new Raycaster("humrcraft 1.0", 800/2, 600/2);
    std::ifstream cfg("cfg.dat"); //TODO: make a better parser
    int lines=0;
    char c;
    for(int i=0; cfg.get(c); i++) {
        if(c == '\n') {
            lines++;
        }
    }
    cfg.close();
    KL_Vector<SDL_Surface> textures;
    cfg.open("cfg.dat");
    for(int i=0; i<lines; i++) {
        std::string data;
        cfg>>data;
        std::cout<<data<<'\n';
        if(data[0] != '#') {
            SDL_Surface* tex = IMG_Load(data.c_str());
            if(tex) {
                textures.pushBack(*tex);
            }
        }
    }
    cfg.close();
#else
    renderer = new SDLRenderer("humrcraft 1.0", 800, 600);
#endif
    world->add(renderer);
    hero = renderer;
    hero->ori = {0, M_PI/2};
    speaker = new Speaker();
    world->add(speaker);
    srand(rand()*time(0));
    for(int i=0; i<50; i++) {
        Object* obj = new Object(new Circle(rand()%4+1, 1, rand()%9+1));
    #if RENDERER == 1
        obj->shape->texture = &textures[rand()%textures.size()];
        obj->shape->texmode = 1;
    #endif
        int color = rand()%128;
        obj->shape->r = 255-color;
        obj->shape->g = 127;
        obj->shape->b = 127+color;
        obj->shape->a = 255;
        srand(rand());
        obj->pos = {(float)(rand()%500-250), (float)(rand()%500-250)};
        obj->vel = {1, (float)(rand()%314/100)};
        world->add(obj);
    }
    long lastUpdate = KL_getMS();
    double msPerUpdate = (double)1000/120;
    double deltaUpdate = 0;
    long lastRender = KL_getMS();
    double msPerRender = (double)1000/60;
    double deltaRender = 0;
    int frames=0, updates=0;
    long lastTime = KL_getMS();
    running = 1;
    while(running) {
        long now = KL_getMS();
        deltaUpdate += (now-lastUpdate)/msPerUpdate;
        lastUpdate = now;
        if(deltaUpdate >= 1) {
            SDL_Event event;
            while(renderer->getEvent(&event)) {
                switch(event.type) {
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {
                            case SDLK_w:
                                hero->applyImpulse({1, hero->getOri().y});
                                break;
                            case SDLK_a:
                                hero->applyImpulse({1, hero->getOri().y+(float)M_PI/2});
                                break;
                            case SDLK_s:
                                hero->applyImpulse({1, hero->getOri().y+(float)M_PI});
                                break;
                            case SDLK_d:
                                hero->applyImpulse({1, hero->getOri().y-(float)M_PI/2});
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
                        renderer->zoom += event.wheel.y;
                        if(renderer->zoom == 0) {
                            renderer->zoom += 1;
                        }
                        break;
                    case SDL_MOUSEBUTTONUP:
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
                        break;
#if RENDERER == 1
                    case SDL_MOUSEMOTION:
                        if(!pause) {
                            hero->ori.y -= renderer->fov/renderer->buffer->w*event.motion.xrel;
                            SDL_WarpMouseInWindow(renderer->window, renderer->buffer->w/2, renderer->buffer->h/2);
                        }
                        break;
#endif
                    case SDL_QUIT:
                        running = 0;
                        break;
                }
                //frame->processEvent(event);
            }
            if(!pause) update(deltaUpdate*msPerUpdate/1000);
            updates++;
            deltaUpdate -= 1;
        }
        now = KL_getMS();
        deltaRender += (now-lastRender)/msPerRender;
        lastRender = now;
        if(deltaRender >= 1) {
            render();
            frames++;
            deltaRender -= 1;
        }
        if(KL_getMS()-lastTime >= 1000) {
            lastTime += 1000;
            std::cout<<updates<<" UPS, "<<frames<<" FPS\n";
            frames = 0;
            updates = 0;
        }
    }
    stop(0);
    return 0;
}
