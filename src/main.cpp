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
#include <karolslib.h>
#include <utils/utils.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "shapes.h"

Renderer* renderer;
Object* hero;
Game game;
float gmulti = 1;
bool pause = 1;
bool running = 1;
/*void generate() {
    choose:
    int seed;
    KL_swrite(KL_stdterm, "Enter seed ");
    if(stoi(KL_sread(KL_stdterm), &seed)) {
        goto choose;
    }
    game.generate(seed, {-100, -100}, {100, 100});
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
}*/
void stop(int status) {
    KL_deinit();
    SDL_Quit();
    exit(status);
}
char* KL_encode(char* str, unsigned int size, int pass) {
    char* result=(char*)malloc(sizeof(char)*size+1);
    for(unsigned int i=0; i<size; i++) {
        result[i] = (float)i/2==(int)i/2 ? str[i]+(i*pass) : str[i]-(i*pass);
    }
    result[size] = '\0';
    return result;
}
char* KL_decode(char* str, unsigned int size, int pass) {
    char* result=(char*)malloc(sizeof(char)*size+1);
    for(unsigned int i=0; i<size; i++) {
        result[i] = (float)i/2==(int)i/2 ? str[i]-(i*pass) : str[i]+(i*pass);
    }
    result[size] = '\0';
    return result;
}

void update(double delta) {
    game.update(delta);
    for(unsigned int i=0; i<game.objs.size(); i++) {
        Object* a = game.objs[i];
        if(a->shape) {
            for(unsigned int j=i+1; j<game.objs.size(); j++) {
                Object* b = game.objs[j];
                if(b->shape) {
                    V2f pos1 = {(float)fmax(a->pos.x, b->pos.x), (float)fmax(a->pos.y, b->pos.y)};
                    V2f pos2 = {(float)fmin(a->pos.x, b->pos.x), (float)fmin(a->pos.y, b->pos.y)};
                    float force = (6.674*10/pow(10, 11))/((1/a->shape->invmass)*(1/b->shape->invmass)/
                                  pow(sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2)), 2));
                    a->applyImpulse({gmulti*force*(float)delta, fatp(a->pos, b->pos)});
                    b->applyImpulse({gmulti*force*(float)delta, fatp(b->pos, a->pos)});
                }
            }
        }
    }
}
void render() {
    renderer->pos = hero->pos;
    game.render();
}
int main(int argc, char** argv) {
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cout<<"SDL_Init error: "<<SDL_GetError()<<'\n';
        stop(1);
    }
    if(KL_init()) {
        std::cout<<"KL_init error"<<'\n';
        stop(1);
    }
    //help();
    /*game.registerThing(new Thing(8, 1.5, 0, {1, (float)1/64}, NULL, Thing::defaultRenderf)); //Tree
    game.registerThing(new Thing(4, 1, 0, {1, (float)1/512}, NULL, Thing::defaultRenderf)); //Log
    game.registerThing(new Thing(1, 0.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Stick
    game.registerThing(new Thing(0.5, 0.5, 0, {1, (float)1/64}, NULL, Thing::defaultRenderf)); //Flower
    game.registerThing(new Thing(0.5, 0.5, 0, {1, (float)1/16}, NULL, Thing::defaultRenderf)); //Grass
    game.registerThing(new Thing(0.5, 0.5, 0, {1, (float)1/1024}, NULL, Thing::defaultRenderf)); //Wheat
    game.registerThing(new Thing(0.25, 0.5, 0, {0, 0}, NULL, Thing::defaultRenderf)); //Seeds

    game.registerThing(new Thing(5, 2.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Flint
    game.registerThing(new Thing(5, 2.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Stone

    game.registerThing(new Thing(0, 0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //TODO: Water

    game.registerThing(new Thing(80, 5, 0.5, {1, (float)1/4096}, NULL, Thing::defaultRenderf)); //Human
    game.registerThing(new Thing(120, 5, 1, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Pig
    game.registerThing(new Thing(450, 5, 1, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Cow
    game.registerThing(new Thing(0.1, 5, 0.25, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Hamster

    game.registerThing(new Thing(1, 0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //Bread
    game.registerThing(new Thing(1, 0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //RawMeat
    game.registerThing(new Thing(1, 0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //CookedMeat*/
    game.registerThing(new Thing(1.5, 0, {1, (float)1/64}, NULL, Thing::defaultRenderf)); //Tree
    game.registerThing(new Thing(1, 0, {1, (float)1/512}, NULL, Thing::defaultRenderf)); //Log
    game.registerThing(new Thing(0.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Stick
    game.registerThing(new Thing(0.5, 0, {1, (float)1/64}, NULL, Thing::defaultRenderf)); //Flower
    game.registerThing(new Thing(0.5, 0, {1, (float)1/16}, NULL, Thing::defaultRenderf)); //Grass
    game.registerThing(new Thing(0.5, 0, {1, (float)1/1024}, NULL, Thing::defaultRenderf)); //Wheat
    game.registerThing(new Thing(0.5, 0, {0, 0}, NULL, Thing::defaultRenderf)); //Seeds

    game.registerThing(new Thing(2.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Flint
    game.registerThing(new Thing(2.5, 0, {1, (float)1/256}, NULL, Thing::defaultRenderf)); //Stone

    game.registerThing(new Thing(0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //TODO: Water

    game.registerThing(new Thing(5, 0.5, {1, (float)1/4096}, NULL, Thing::defaultRenderf)); //Human
    game.registerThing(new Thing(5, 1, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Pig
    game.registerThing(new Thing(5, 1, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Cow
    game.registerThing(new Thing(5, 0.25, {1, (float)1/2048}, NULL, Thing::defaultRenderf)); //Hamster

    game.registerThing(new Thing(0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //Bread
    game.registerThing(new Thing(0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //RawMeat
    game.registerThing(new Thing(0, 0, {0, 0}, NULL, Thing::defaultRenderf)); //CookedMeat
    renderer = new Renderer("textcraft 1.0", 800, 600, 1);
    game.add(renderer);
    hero = (Object*) game.createThing(Human);
    game.add(hero);
    srand(rand()*time(0));
    for(int i=0; i<150; i++) {
        int color = rand()%128;
        Object* obj = new Object(new Circle(rand()%4+1, 0, 1, 255-color, 127, 127+color));
        srand(rand());
        obj->pos = {(float)(rand()%1000-500), (float)(rand()%1000-500)};
        obj->vel = {5, (float)(rand()%314/100)};
        game.add(obj);
    }
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
            SDL_Event event;
            while(renderer->getEvent(&event)) {
                switch(event.type) {
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {
                            case SDLK_w:
                                hero->applyImpulse({10, M_PI/2});
                                break;
                            case SDLK_a:
                                hero->applyImpulse({10, M_PI});
                                break;
                            case SDLK_s:
                                hero->applyImpulse({10, M_PI/2+M_PI});
                                break;
                            case SDLK_d:
                                hero->applyImpulse({10, 0});
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
                            case SDLK_INSERT: {
                                int color = rand()%128;
                                Object* obj = new Object(new Circle(rand()%4+1, 0.5, 1, 255-color, 127, 127+color));
                                srand(rand());
                                obj->pos = hero->pos;
                                obj->vel = {5, (float)(rand()%314/100)};
                                game.add(obj);
                                }break;
                            /*case 'z':
                                generate();
                                break;
                            case 'x':
                                help();
                                break;*/
                        }
                        break;
                    case SDL_MOUSEWHEEL:
                        renderer->zoom += event.wheel.y;
                        if(renderer->zoom == 0) {
                            renderer->zoom += 1;
                        }
                        break;
                    case SDL_QUIT:
                        running = 0;
                        break;
                }
            }
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
        if(KL_getNS()-lastTimer >= 1000) {
            lastTimer += 1000;
            frames = 0; //Reset FPS counter
            ticks = 0; //Reset TPS counter
        }
    }
    stop(0);
    return 0;
}
