/*
 * main.cpp
 * textcraft Source Code
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
#define RENDERER 1 //0 - SDLRenderer, 1 - Raycaster, 2 - ClassicRenderer

/*GUIProgressBar* progressbar;
GUIFrame* frame;*/
World* game = new World();
Object* hero;
#if RENDERER == 2
#include <karolslib.h>
#include <window/terminal.h>
ClassicRenderer* renderer;
void generate() {
    choose:
    int seed;
    KL_swrite(KL_stdterm, "Enter seed ");
    if(stoi(KL_sread(KL_stdterm), &seed)) {
        goto choose;
    }
    game->generate(seed, {-100, -100}, {100, 100});
}
void help() {
    KL_swrite(KL_stdterm, "textcraft 1.0 Copyright (C) 2017 Karol \"digitcrusher\" Łacina\n");
    KL_swrite(KL_stdterm, "This program comes with ABSOLUTELY NO WARRANTY.\n");
    KL_swrite(KL_stdterm, "This is free software, and you are welcome to redistribute it\n");
    KL_swrite(KL_stdterm, "under certain conditions.\n");
    KL_swrite(KL_stdterm, "Controls\n");
    KL_swrite(KL_stdterm, "Q - Drop a thing\n");
    KL_swrite(KL_stdterm, "E - Pick up a thing\n");
    KL_swrite(KL_stdterm, "W - Move forward\n");
    KL_swrite(KL_stdterm, "A - Move left\n");
    KL_swrite(KL_stdterm, "S - Move down\n");
    KL_swrite(KL_stdterm, "D - Move right\n");
    KL_swrite(KL_stdterm, "Z - Generate a world\n");
    KL_swrite(KL_stdterm, "X - Get some help\n");
    KL_swrite(KL_stdterm, "C - Settings\n");
    KL_swrite(KL_stdterm, "Press any key to continue...\n");
    KL_cread(KL_stdterm);
    KL_swrite(KL_stdterm, "Characters\n");
    KL_swrite(KL_stdterm, "@ - Hero\n");
    KL_swrite(KL_stdterm, "Y - Tree\n");
    KL_swrite(KL_stdterm, "n - Rock\n");
    KL_swrite(KL_stdterm, "* - Flower\n");
    KL_swrite(KL_stdterm, "m - Wolf\n");
    KL_swrite(KL_stdterm, "# - Lava\n");
    KL_swrite(KL_stdterm, "~ - Water\n");
    KL_swrite(KL_stdterm, "# - Milk\n");
    KL_swrite(KL_stdterm, "\" - Grass\n");
    KL_swrite(KL_stdterm, "M - Cow\n");
    KL_swrite(KL_stdterm, "o - Hamster\n");
    KL_swrite(KL_stdterm, "Press any key to continue...\n");
    KL_cread(KL_stdterm);
}
#elif RENDERER == 1
Raycaster* renderer;
#else
SDLRenderer* renderer;
#endif
float gmulti = 1;
bool pause = 1;
bool running = 1;
void stop(int status) {
    TTF_Quit();
    SDL_Quit();
    exit(status);
}
void update(double delta) {
    game->update(delta);
    for(unsigned int i=0; i<game->objs.size(); i++) {
        if(!game->objs.isFree(i)) {
            Object* a = game->objs[i];
            if(a->shape) {
                for(unsigned int j=i+1; j<game->objs.size(); j++) {
                    if(!game->objs.isFree(j)) {
                        Object* b = game->objs[j];
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
    game->render();
    //frame->render(SDL_GetWindowSurface(renderer->window));
    //SDL_UpdateWindowSurface(renderer->window);
}
int main(int argc, char** argv) {
    std::cout<<"textcraft 1.0 Copyright (C) 2017 Karol \"digitcrusher\" Łacina\n";
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
    /*game->registerThing(new Thing(8, 1.5, 0, {1, (float)1/64}, NULL, Thing::defaultRenderf)); //Tree
    game->registerThing(new Thing(4, 1, 0, {1, (float)1/512}, NULL, Thing::defaultRenderf)); //Log
    game->registerThing(new Thing(1, 0.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Stick
    game->registerThing(new Thing(0.5, 0.5, 0, {1, (float)1/64}, NULL, Thing::defaultRenderf)); //Flower
    game->registerThing(new Thing(0.5, 0.5, 0, {1, (float)1/16}, NULL, Thing::defaultRenderf)); //Grass
    game->registerThing(new Thing(0.5, 0.5, 0, {1, (float)1/1024}, NULL, Thing::defaultRenderf)); //Wheat
    game->registerThing(new Thing(0.25, 0.5, 0, {0, 0}, NULL, Thing::defaultRenderf)); //Seeds

    game->registerThing(new Thing(5, 2.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Flint
    game->registerThing(new Thing(5, 2.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Stone

    game->registerThing(new Thing(0, 0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //TODO: Water

    game->registerThing(new Thing(80, 5, 0.5, {1, (float)1/4096}, NULL, Thing::defaultRenderf)); //Human
    game->registerThing(new Thing(120, 5, 1, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Pig
    game->registerThing(new Thing(450, 5, 1, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Cow
    game->registerThing(new Thing(0.1, 5, 0.25, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Hamster

    game->registerThing(new Thing(1, 0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //Bread
    game->registerThing(new Thing(1, 0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //RawMeat
    game->registerThing(new Thing(1, 0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //CookedMeat*/
    game->registerThing(new Thing(1.5, 0, {1, (float)1/64}, NULL, Thing::defaultRenderf)); //Tree
    game->registerThing(new Thing(1, 0, {1, (float)1/512}, NULL, Thing::defaultRenderf)); //Log
    game->registerThing(new Thing(0.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Stick
    game->registerThing(new Thing(0.5, 0, {1, (float)1/64}, NULL, Thing::defaultRenderf)); //Flower
    game->registerThing(new Thing(0.5, 0, {1, (float)1/16}, NULL, Thing::defaultRenderf)); //Grass
    game->registerThing(new Thing(0.5, 0, {1, (float)1/1024}, NULL, Thing::defaultRenderf)); //Wheat
    game->registerThing(new Thing(0.5, 0, {0, 0}, NULL, Thing::defaultRenderf)); //Seeds

    game->registerThing(new Thing(2.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Flint
    game->registerThing(new Thing(2.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Stone

    game->registerThing(new Thing(0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //TODO: Water

    game->registerThing(new Thing(5, 0.5, {1, (float)1/4096}, NULL, Thing::defaultRenderf)); //Human
    game->registerThing(new Thing(5, 1, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Pig
    game->registerThing(new Thing(5, 1, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Cow
    game->registerThing(new Thing(5, 0.25, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Hamster

    game->registerThing(new Thing(0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //Bread
    game->registerThing(new Thing(0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //RawMeat
    game->registerThing(new Thing(0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //CookedMeat
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
#if RENDERER == 2
    if(KL_init()) {
        std::cout<<"KL_init error"<<'\n';
        stop(1);
    }
    renderer = new ClassicRenderer(20, 20);
#elif RENDERER == 1
    renderer = new Raycaster("textcraft 1.0", 800/2, 600/2);
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
    renderer = new SDLRenderer("textcraft 1.0", 800, 600);
#endif
    game->add(renderer);
    hero = renderer;
    hero->ori = {0, M_PI/2};
#if RENDERER == 2
    game->generate(0, {-50, -50}, {50, 50});
#else
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
        obj->pos = {(float)(rand()%100-50), (float)(rand()%100-50)};
        obj->vel = {1, (float)(rand()%314/100)};
        game->add(obj);
    }
#endif
    long lastTime = KL_getMS();
    long lastUpdate = KL_getMS();
    double ticksPerS = 60;
    double msPerTick = 1000/ticksPerS;
    long lastTimer = KL_getMS();
    double delta = 0;
    int ticks=0, frames=0;
    running = 1;
    while(running) {
        long now = KL_getMS();
        delta += (now-lastTime)/msPerTick;
        lastTime = now;
        bool shouldRender = 0;
        while(delta >= 1) {
#if RENDERER != 2
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
                                    game->add(obj);
                                }break;
                            case SDL_BUTTON_RIGHT: {
                                    for(unsigned int i=0; i<game->objs.size(); i++) {
                                        if(!game->objs.isFree(i)) {
                                            Object* a = game->objs[i];
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
#endif
            now = KL_getMS();
            if(!pause) update((double)(now-lastUpdate)/1000);
            lastUpdate = now;
            ticks++; //Increment by 1
            delta -= 1;
            shouldRender = 1;
        }
        if(shouldRender) {
            render();
            frames++; //Increment by 1
        }
        if(KL_getMS()-lastTimer >= 1000) {
            lastTimer += 1000;
#if RENDERER == 2
            ((ClassicRenderer*)renderer)->ticks = ticks;
            ((ClassicRenderer*)renderer)->frames = frames;
#endif
            std::cout<<frames<<' '<<ticks<<'\n';
            frames = 0; //Reset FPS counter
            ticks = 0; //Reset TPS counter
        }
    }
    stop(0);
    return 0;
}
