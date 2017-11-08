/*
 * total number of lines as of 12.08.2017: 4073
 * total number of lines as of 1.10.2017: 6451
 * main.cpp
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
#if defined(_WIN32)
#define SDL_MAIN_HANDLED
#endif
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "utils.hpp"
#include "game.hpp"
#include "shapes.hpp"
#include "renderers.hpp"
#include "graphics.hpp"
#include "game.hpp"
#include "language.hpp"

class BigNumber {
    utils::Vector<char> data;
    bool sign;
    int size;
    int mantissa;
    BigNumber() {
    }
    virtual ~BigNumber() {
    }
    virtual BigNumber& operator+(const BigNumber& rvalue) {
        unsigned char carry;
        for(int i=this->size-1; i>=0; i--) {

        }
    }
    virtual BigNumber& operator-(const BigNumber& rvalue) {
    }
    virtual BigNumber& operator*(const BigNumber& rvalue) {
    }
    virtual BigNumber& operator/(const BigNumber& rvalue) {
    }
};

humrcraft::game::Game* game = new humrcraft::game::Game();
humrcraft::renderers::SDLGLRenderer* renderer;
humrcraft::Speaker* speaker;
humrcraft::game::Textures* textures;
humrcraft::game::Thing* hero;
float gmulti = 1;
bool pause = 1;
bool running = 1;
void stop(int status) {
    game->destroyAll();
    delete game;
    TTF_Quit();
    SDL_Quit();
    exit(status);
}
void update(double delta) {
    game->update(delta);
    game->speak();
    game->interface();
    for(int i=0; i<game->objs.size(); i++) {
        if(!game->objs.isFree(i)) {
            humrcraft::Object* a = game->objs[i];
            if(a->shape) {
                for(int j=i+1; j<game->objs.size(); j++) {
                    if(!game->objs.isFree(j)) {
                        humrcraft::Object* b = game->objs[j];
                        if(b->shape) {
                            float force = (6.674*10/pow(10, 11))/((1/a->shape->getInvMass())*(1/b->shape->getInvMass())/
                                          pow(math::distance(a->getPos(), b->getPos()), 2));
                            a->applyImpulse((math::V2f){gmulti*force*(float)delta, 0}+fatp(a->getPos(), b->getPos()));
                            b->applyImpulse((math::V2f){gmulti*force*(float)delta, 0}+fatp(b->getPos(), b->getPos()));
                        }
                    }
                }
            }
        }
    }
}
void render() {
    renderer->pos = hero->getPos();
    game->render();
}
int main(int argc, char** argv) {
    std::cout<<"humrcraft 1.0 Copyright (C) 2017 Karol \"digitcrusher\" Łacina\n";
    std::cout<<"This program comes with ABSOLUTELY NO WARRANTY.\n";
    std::cout<<"This is free software, and you are welcome to redistribute it\n";
    std::cout<<"under certain conditions.\n";
    /*int i=0;
    int bignum=0;
    int bigdiv=0;
    while(i<(int)-1>>1) {
        int div=0;
        for(int j=1; j<i; j++) {
            if((float)i/j==(int)(float)i/j) {
                div++;
            }else {
                break;
            }
        }
        if(div > bigdiv) {
            bignum = i;
            bigdiv = div;
        }
        if(i%1000==0) {
            std::cout<<(float)i/2147483647*100<<"%\n";
            std::cout<<"#"<<bignum<<": "<<bigdiv<<"\n";
        }
        i++;
    }
    std::cout<<"#"<<bignum<<": "<<bigdiv<<"\n";*/
    int primes[50];
    primes[0] = 1;
    for(int i=1, num=2; i<50; num++) {
        int div=2;
        for(; div<num; div++) {
            if((float)num/div==num/div) {
                break;
            }
        }
        if(div == num) {
            primes[i] = num;
            i++;
        }
    }
    for(int i=2; i<50; i++) {
        std::cout<<primes[i]<<": ";
        int j=primes[i];
        for(int k=i-1; k>=0 && j; k--) {
            if(j>=primes[k]) {
                j-=primes[k];
                std::cout<<primes[k]<<", ";
            }
        }
        std::cout<<'\n';
    }
    CIPL::Scope scope;
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
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr<<"SDL_Init error: "<<SDL_GetError()<<'\n';
        stop(1);
    }
    renderer = new humrcraft::renderers::SDLGLRenderer("humrcraft 1.0", 800, 600, 2, 1);
    game->add(renderer);
    /*speaker = new Speaker();
    game->add(speaker);*/
    textures = new humrcraft::game::Textures();
    game->add(textures);
    SDL_Surface* texturesheet = IMG_Load("./gfx/texturesheet.png");
    textures->add(IMG_Load("./gfx/humr.png"));
    textures->add(IMG_Load("./gfx/human.png"));
    textures->add(IMG_Load("./gfx/matemat0.png"));
    textures->add(IMG_Load("./gfx/bricks.png"));
    textures->add(IMG_Load("./gfx/grass.png"));
    textures->add(IMG_Load("./gfx/papaver_orientale.png"));
    textures->add(IMG_Load("./gfx/orror.png"));
    textures->add(IMG_Load("./gfx/water.png"));
    SDL_FreeSurface(texturesheet);
    humrcraft::game::Tiles* background = new humrcraft::game::Tiles(textures);
    //humrcraft::game::Tiles* foreground = new humrcraft::game::Tiles(textures);
    game->add(background);
    //game->add(foreground);
    /*Thing* humr = new Thing(NULL, 0, NULL, NULL, new humrcraft::shapes::Rectangle((math::V2fPair){{-0.5, -0.375}, {0.5, 0.3125}}), 1, 0, (*textures)[0]);
    humr->pos = {0, 0};
    game->add(humr);*/
    game->addThing(new humrcraft::game::Thing(NULL, 0, [&](humrcraft::game::Thing* base, void* data, int datasize) {
        return new humrcraft::game::Thing(data, datasize, base->recreatef, base->initf, base->uninitf, new humrcraft::shapes::Rectangle((math::V2fPair){{-0.34375, -0.5}, {0.34375, 0.5}}),
            1, 0, (*textures)[1]);
    }, NULL, NULL, new humrcraft::shapes::Rectangle((math::V2fPair){{-0.34375, -0.5}, {0.34375, 0.5}}), 1, 0, (*textures)[1])); //Human
    game->addThing(new humrcraft::game::Thing(NULL, 0, [&](humrcraft::game::Thing* base, void* data, int datasize) {
        return new humrcraft::game::Thing(data, datasize, base->recreatef, base->initf, base->uninitf, new humrcraft::shapes::Rectangle((math::V2fPair){{-0.5, -0.5}, {0.5, 0.3125}}),
            1, 0, (*textures)[2]);
    }, NULL, NULL, new humrcraft::shapes::Rectangle((math::V2fPair){{-0.5, -0.5}, {0.5, 0.3125}}), 1, 0, (*textures)[2])); //Matemat
    humrcraft::game::Thing* human = game->createThing(0, NULL, 0);
    human->pos = {0, 0};
    game->add(human);
    humrcraft::game::Thing* matemat = game->createThing(1, NULL, 0);
    matemat->pos = {2, 2};
    game->add(matemat);
    hero = human;
    /*Polygon* polygon = new Polygon();
    std::cout<<"polygon"<<'\n';
    polygon->addVertex({50, 5.9});
    polygon->addVertex({25, 3.8});
    polygon->addVertex({50, 0.3});
    polygon->addVertex({25, 1.2});*/
    /*srand(rand()*time(0));
    for(int i=0; i<50; i++) {
        humrcraft::Object* obj = new humrcraft::Object(new humrcraft::shapes::Circle(rand()%4+1, {1, 1}));
        //humrcraft::Object* obj = new humrcraft::Object(polygon);
        int color = rand()%128;
        obj->shape->r = 255-color;
        obj->shape->g = 127;
        obj->shape->b = 127+color;
        obj->shape->a = 255;
        srand(rand());
        obj->pos = {(float)(rand()%1000-500), (float)(rand()%1000-500)};
        obj->vel = {1, (float)(rand()%314/100)};
        game->add(obj);
    }*/
    long lastUpdate = utils::getMS();
    double msPerUpdate = (double)1000/120;
    double deltaUpdate = 0;
    long lastRender = utils::getMS();
    double msPerRender = (double)1000/60;
    double deltaRender = 0;
    int frames=0, updates=0;
    long lastTime = utils::getMS();
    running = 1;
    struct utils::timer timer;
    utils::setLoopTimer(&timer, 0, 0);
    utils::resetTimer(&timer);
    while(running) {
        long now = utils::getMS();
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
                                hero->applyImpulse({1, hero->getOri().y+(float)math::pi/2});
                                break;
                            case SDLK_s:
                                hero->applyImpulse({1, hero->getOri().y+(float)math::pi});
                                break;
                            case SDLK_d:
                                hero->applyImpulse({1, hero->getOri().y-(float)math::pi/2});
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
                                    humrcraft::Object* obj = new humrcraft::Object(new Circle(rand()%4+1, 0.5, 1, 255-color, 127, 127+color, 255));
                                    srand(rand());
                                    obj->pos = renderer->getPos({event.button.x, event.button.y});
                                    obj->vel = hero->getVel();
                                    game->add(obj);
                                }break;
                            case SDL_BUTTON_RIGHT: {
                                    for(int i=0; i<game->objs.size(); i++) {
                                        if(!game->objs.isFree(i)) {
                                            humrcraft::Object* a = game->objs[i];
                                            math::V2f pos = renderer->getPos({event.button.x, event.button.y});
                                            math::V2f pos1 = {(float)fmax(a->getPos().x, pos.x), (float)fmax(a->getPos().y, pos.y)};
                                            math::V2f pos2 = {(float)fmin(a->getPos().x, pos.x), (float)fmin(a->getPos().y, pos.y)};
                                            //14318181818.181818182 - Little Boy atomic bomb power
                                            //4184 - 1 g of TNT
                                            math::V2f j = {-4184/(float)(sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2))+1), fatp(a->getPos(), pos)};
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
                            hero->ori = math::fatp(hero->pos, renderer->SDLGetPos({event.motion.x, event.motion.y}));
                        }
                        break;
                }
                //frame->processEvent(event);
            }
            if(!pause) {
                update(msPerUpdate/1000);
            }
            updates++;
            deltaUpdate -= 1;
        }
        now = utils::getMS();
        deltaRender += (now-lastRender)/msPerRender;
        lastRender = now;
        if(deltaRender >= 1) {
            render();
            frames++;
            deltaRender -= 1;
        }
        if(utils::getMS()-lastTime >= 1000) {
            lastTime += 1000;
            std::cout<<updates<<" UPS, "<<frames<<" FPS, "<<utils::getElapsedTimer(timer)<<" SEC\n";
            frames = 0;
            updates = 0;
        }
    }
    stop(0);
    return 0;
}
