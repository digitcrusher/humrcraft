/*
 * textsweeper.cpp
 * textsweeper Source Code
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
#include "textsweeper.hpp"
#include <iostream>

namespace textsweeper {
    Tile::Tile(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing)) : Block(data, datasize, recreatef, initf, uninitf) {
        this->family.pushBack("Tile");
        this->sum = 0;
        this->expose = 0;
        this->mine = 0;
        this->flag = 0;
        this->wrong = 0;
        this->pressed = 0;
    }
    Tile::~Tile() {
    }
    void Tile::render(humrcraft::Renderer* renderer) {
        if(this->pressed) {
            this->textureid = 8+1;
        }else if(this->expose) {
            this->textureid = 8+1;
        }else {
            this->textureid = 8+0;
        }
        humrcraft::game::Block::render(renderer);
        if(this->resources) {
            if(this->expose) {
                if(this->mine) {
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawImage(this->pos, this->resources->getTexture(8+3));
                }else if(this->sum) {
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawImage(this->pos, this->resources->getTexture(8+5+this->sum%10));
                }
            }
            if(this->flag) {
                ((humrcraft::renderers::SDLRenderer*)renderer)->drawImage(this->pos, this->resources->getTexture(8+2));
            }
            if(this->wrong) {
                ((humrcraft::renderers::SDLRenderer*)renderer)->drawImage(this->pos, this->resources->getTexture(8+4));
            }
        }
    }
    humrcraft::game::Thing* Tile::defaultRecreatef(Thing* base, void* data, int datasize) {
        Tile* tile = new Tile(data, datasize, base->recreatef, base->initf, base->uninitf);
        tile->updatef = base->updatef;
        tile->interfacef = base->interfacef;
        tile->renderf = base->renderf;
        tile->speakf = base->speakf;
        tile->usef = base->usef;
        tile->attackf = base->attackf;
        tile->actionf = base->actionf;
        tile->collisionCallbackf = base->collisionCallbackf;
        return tile;
    }

    Minefield::Minefield(math::V2iPair size) : Level(size, {0.5, 0.5}) {
        this->family.pushBack("Minefield");
        this->turns = 0;
        this->flags = 0;
        this->mines = 0;
    }
    Minefield::~Minefield() {
    }
    bool Minefield::flag(math::V2i pos) {
        Tile* tile = (Tile*)*this->getBlock((math::V2i)pos);
        if(!tile->expose) {
            if(tile->flag) {
                tile->flag = 0;
                this->flags++;
            }else if(this->flags > 0) {
                tile->flag = 1;
                this->flags--;
            }
        }
        return tile->flag;
    }
    bool Minefield::expose(math::V2i pos, math::V2i* neighbors, int neighborssize, bool recursion) {
        Tile* tile = (Tile*)*this->getBlock(pos);
        if(tile->flag) {
            return 0;
        }
        tile->expose = 1;
        this->turns++;
        if(tile->mine) {
            tile->wrong = 1;
            return 1;
        }
        if(!recursion && !tile->sum) {
            for(int i=0; i<neighborssize; i++) {
                math::V2i pos2 = pos+neighbors[i];
                if(pointInSquare(pos2, {this->size.v1, this->size.v2-(math::V2i){1, 1}})) {
                    Tile* tile2 = (Tile*)*this->getBlock(pos2);
                    if(!tile2->expose) {
                        if(this->expose(pos2, neighbors, neighborssize, tile2->sum)) {
                            return 1;
                        }
                    }
                }
            }
        }
        return 0;
    }
    int Minefield::getGameState() {
        bool clear=1;
        int flagged=0;
        bool fail=0;
        if(this->turns == 0) {
            return 0;
        }
        for(int i=0; i<this->getSize().x*this->getSize().y; i++) {
            Tile* tile = (Tile*)this->blocks[i];
            if(tile->mine && tile->expose) {
                return 2;
            }
            if(!tile->mine && !tile->expose) {
                clear = 0;
            }
            if(tile->mine && tile->flag) {
                flagged++;
            }else if(tile->mine && !tile->flag) {
                flagged = 0;
            }
        }
        if(fail) {
            return 2;
        }
        if(flagged == this->mines || clear) {
            return 1;
        }
        return 0;
    }
    void Minefield::showAll() {
        for(int i=0; i<this->getSize().x*this->getSize().y; i++) {
            Tile* tile = (Tile*)this->blocks[i];
            if(tile->flag && !tile->mine) {
                tile->wrong = 1;
            }
            tile->expose = 1;
        }
    }
    void Minefield::resetMinefield() {
        this->turns = 0;
        this->flags = 0;
        this->mines = 0;
        utils::setLoopTimer(&this->timer, 0, 0);
        utils::pauseTimer(&this->timer, 0);
        utils::resetTimer(&this->timer);
        for(int i=0; i<this->getSize().x*this->getSize().y; i++) {
            Tile** tile = (Tile**)(this->blocks+i);
            if(*tile) {
                delete *tile;
            }
            *tile = (Tile*)((humrcraft::game::Game*)this->world)->recreateBlock(4, NULL, 0);
        }
    }
    void Minefield::plantMines(int seed, int mines, math::V2iPair safezone) {
        srand(seed);
        std::cout<<"Planting Mines...\n";
        std::cout<<"Safezone: "<<safezone.v1.x<<", "<<safezone.v1.y<<", "<<safezone.v2.x<<", "<<safezone.v2.y<<"\n";
        for(int i=0; i<mines; i++) {
            randomize:
            int x = (int)rand()%(int)this->getSize().x+this->size.v1.x;
            int y = (int)rand()%(int)this->getSize().y+this->size.v1.y;
            Tile* tile = (Tile*)*this->getBlock((math::V2i){x, y});
            if(tile->mine || pointInSquare((math::V2i){x, y}, safezone)) {
                std::cout<<"Unfair Mine: "<<x<<", "<<y<<"\n";
                goto randomize;
            }
            std::cout<<"Fair Mine: "<<x<<", "<<y<<"\n";
            tile->mine = 1;
        }
    }
    void Minefield::calculateSums(math::V2i* neighbors, int neighborssize) {
        for(int x=this->size.v1.x; x<this->size.v2.x; x++) {
            for(int y=this->size.v1.y; y<this->size.v2.y; y++) {
                Tile* tile = (Tile*)*this->getBlock((math::V2i){x, y});
                int sum=0;
                for(int i=0; i<neighborssize; i++) {
                    math::V2i pos = (math::V2i){x, y}+neighbors[i];
                    if(pointInSquare(pos, {this->size.v1, this->size.v2-(math::V2i){1, 1}})) {
                        Tile* tile2 = (Tile*)*this->getBlock(pos);
                        if(tile2->mine) {
                            sum += 1;
                        }
                    }
                }
                tile->sum = sum;
            }
        }
    }
    void Minefield::generateMinefield(int seed, int mines, math::V2iPair safezone, math::V2i* neighbors, int neighborssize) {
        this->resetMinefield();
        this->mines = mines;
        this->flags = this->mines;
        this->plantMines(seed, this->mines, safezone);
        this->calculateSums(neighbors, neighborssize);
    }

    Player::Player(void (*turnf)(Minefield* minefield)) {
        this->turnf = turnf;
        this->score = 0;
    }
    Player::~Player() {
    }
    void Player::turn() {
        if(!this->board) {
            return;
        }
        this->turnf(this->board);
        /*board pbrd;
        createMinefield(&pbrd, brd->size.x, brd->size.y, {0, 1, 1, 0, 0, 0});
        resetMinefield(&pbrd, pbrd.nulltemp);
        //Calculate possibilities
        for(int x=0; x<pbrd.size.x; x++) {
            for(int y=0; y<pbrd.size.y; y++) {
                for(int i=0; i<(int)(sizeof(neighbors)/sizeof(neighbors[0])); i++) {
                    V p1 = {x+neighbors[i].x, y+neighbors[i].y};
                    tile* t1 = getTile(brd, p1.x, p1.y);
                    if(t1->show) {
                        int sum=t1->sum;
                        for(int j=0; j<(int)(sizeof(neighbors)/sizeof(neighbors[0])); j++) {
                            V p2 = {p1.x+neighbors[j].x, p1.y+neighbors[j].y};
                            tile* t2 = getTile(brd, p2.x, p2.y);
                            sum -= t2->flag;
                        }
                        getTile(&pbrd, x, y)->sum += sum;
                        getTile(&pbrd, x, y)->show = !getTile(brd, x, y)->show;
                    }
                }
            }
        }
        //Copy numbers
        tile** tiles=(tile**)malloc(sizeof(tile*)*pbrd.size.x*pbrd.size.y);
        for(int x=0; x<pbrd.size.x; x++) {
            for(int y=0; y<pbrd.size.y; y++) {
                tiles[x+y*(int)pbrd.size.x] = getTile(&pbrd, x, y);
            }
        }
        //Sort numbers
        for(int x=0; x<pbrd.size.x*pbrd.size.y; x++) {
            for(int y=0; y<pbrd.size.x*pbrd.size.y-1; y++) {
                if(tiles[y]->sum > tiles[y+1]->sum) {
                    tile* temp = tiles[y+1];
                    tiles[y+1] = tiles[y];
                    tiles[y] = temp;
                }
            }
        }
        //Flags, show etc.
        if(tiles[0]->sum < 0) {
            int i=0;
            int x=(tiles[i]-pbrd.map)%(int)pbrd.size.x;
            int y=(tiles[i]-pbrd.map)/(int)pbrd.size.x;
            for(; !getTile(brd, x, y)->flag; i++) {
                x = (tiles[i]-pbrd.map)%(int)pbrd.size.x;
                y = (tiles[i]-pbrd.map)/(int)pbrd.size.x;
            }
            if(tiles[i]->sum < 0) {
                flag(brd, x, y);
                goto end;
            }
        }
        if(tiles[(int)pbrd.size.x*(int)pbrd.size.y-1]->sum > 0) {
            int i=pbrd.size.x*pbrd.size.y-1;
            int x=(tiles[i]-pbrd.map)%(int)pbrd.size.x;
            int y=(tiles[i]-pbrd.map)/(int)pbrd.size.x;
            for(; getTile(brd, x, y)->flag || getTile(brd, x, y)->show; i--) {
                x = (tiles[i]-pbrd.map)%(int)pbrd.size.x;
                y = (tiles[i]-pbrd.map)/(int)pbrd.size.x;
            }
            if(tiles[i]->sum > 0) {
                flag(brd, x, y);
                goto end;
            }
        }
        //Didn't find anything so guess a tile
        for(int x=0; x<pbrd.size.x; x++) {
            for(int y=0; y<pbrd.size.y; y++) {
                if(!getTile(brd, x, y)->show) {
                    show(brd, x, y, 0);
                    goto end;
                }
            }
        }
        end:
        destroyMinefield(&pbrd);*/
    }
    void Player::setMinefield(Minefield* board) {
        this->board = board;
    }

    math::V2i normalneighbors[] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
    int normalneighborssize = sizeof(normalneighbors)/sizeof(*normalneighbors);
    math::V2i knightneighbors[] = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
    int knightneighborssize = sizeof(knightneighbors)/sizeof(*knightneighbors);
    math::V2i orthogonalneighbors[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int orthogonalneighborssize = sizeof(orthogonalneighbors)/sizeof(*orthogonalneighbors);
    math::V2i diagonalneighbors[] = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
    int diagonalneighborssize = sizeof(diagonalneighbors)/sizeof(*diagonalneighbors);
}

/*
//main.cpp
#include <stdlib.h>
#include <time.h>
#include <karolslib.h>
#include <utils/utils.h>
#include <utils/module.h>
#include <window/terminal.h>
#include "board.h"

int curx, cury;
board brd;
bool running=1;
bool playing=1;
bool win=0;
KL_module options;
void help() {
   KL_swrite(KL_stdterm, "    How to play\n");
   KL_swrite(KL_stdterm, "Use arrow keys to navigate the cursor.\n");
   KL_swrite(KL_stdterm, "The highlighted tile is where the cursor is.\n");
   KL_swrite(KL_stdterm, "Press 1 to flag the highlighted tile.\n");
   KL_swrite(KL_stdterm, "Press 2 to show the highlighted tile.\n");
   KL_swrite(KL_stdterm, "Every game you get to choose the number of mines\n");
   KL_swrite(KL_stdterm, "and the size of the board.\n");
   KL_swrite(KL_stdterm, "Press any key to continue...\n");
   KL_cread(KL_stdterm);
}
void flag() {
   if(flag(&brd, curx, cury)) {
       KL_swrite(KL_stdterm, "You don't have enough flags!\n");
       KL_cread(KL_stdterm);
   }
}
void show() {
   show(&brd, curx, cury, 0);
}
void solve() {
   solveMinefield(&brd);
}
void restart() {
   playing = 0;
   win = 0;
}
void settings() {
   choose:
   char* str;
   KL_swrite(KL_stdterm, "Settings\n");
   KL_swrite(KL_stdterm, "Upper margin ");
   str = KL_sread(KL_stdterm);
   if(stoi(str, &KL_stdterm->margintop)) {
       goto choose;
   }
   KL_swrite(KL_stdterm, "Right margin ");
   str = KL_sread(KL_stdterm);
   if(stoi(str, &KL_stdterm->marginright)) {
       goto choose;
   }
   KL_swrite(KL_stdterm, "Bottom margin ");
   str = KL_sread(KL_stdterm);
   if(stoi(str, &KL_stdterm->marginbottom)) {
       goto choose;
   }
   KL_swrite(KL_stdterm, "Left margin ");
   str = KL_sread(KL_stdterm);
   if(stoi(str, &KL_stdterm->marginleft)) {
       goto choose;
   }
}
void stop() {
   KL_deinit();
   exit(0);
}
void termclose(KL_terminal* term) {
   stop();
}
void addOpt(KL_routine opt) {
   KL_addRtn(opt, &options);
}
board* getMinefield() {
   return &brd;
}
int main(int argc, char** argv) {
   KL_init();
   KL_stdterm->close = termclose;
   options = *KL_createModule("options", NULL);
   KL_addRtn(*KL_createRoutine("addOpt", (void (*)())addOpt), KL_getMod("main", KL_stdmodbrd));
   KL_addRtn(*KL_createRoutine("getMinefield", (void (*)())getMinefield), KL_getMod("main", KL_stdmodbrd));
   addOpt(*KL_createRoutine("Help", (void (*)())help));
   addOpt(*KL_createRoutine("Flag", (void (*)())flag));
   addOpt(*KL_createRoutine("Show", (void (*)())show));
   addOpt(*KL_createRoutine("Solve", (void (*)())solve));
   addOpt(*KL_createRoutine("Restart", (void (*)())restart));
   addOpt(*KL_createRoutine("Settings", (void (*)())settings));
   addOpt(*KL_createRoutine("Quit", (void (*)())stop));
   KL_swrite(KL_stdterm, "textsweeper 1.1 Copyright (C) 2017 Karol \"digitcrusher\" Łacina\n");
   KL_swrite(KL_stdterm, "This program comes with ABSOLUTELY NO WARRANTY.\n");
   KL_swrite(KL_stdterm, "This is free software, and you are welcome to redistribute it\n");
   KL_swrite(KL_stdterm, "under certain conditions.\n");
   help();
   while(running) {
       {
           //Get input
           choose:
           unsigned int x, y, mines;
           char* str;
           KL_swrite(KL_stdterm, "Enter the size of the board:\n");
           KL_swrite(KL_stdterm, "Column ");
           str = KL_sread(KL_stdterm);
           if(stoui(str, &x)) {
               goto choose;
           }
           KL_swrite(KL_stdterm, "Row ");
           str = KL_sread(KL_stdterm);
           if(stoui(str, &y)) {
               goto choose;
           }
           KL_swrite(KL_stdterm, "Mines ");
           str = KL_sread(KL_stdterm);
           bool error = stoui(str, &mines);
           if(error || mines > x*y) {
               goto choose;
           }
           KL_swrite(KL_stdterm, "Initializing board...\n");
           createMinefield(&brd, x, y, {0, 0, 0, 0, 0, 0});
           KL_swrite(KL_stdterm, "Generating board...\n");
           generateMinefield(&brd, mines);
           brd.nulltemp = {0, 1, 1, 0, 0, 0};
       }
       int starttime=time(0);
       playing = 1;
       while(playing) {
           //Check if won
           bool clear=1;
           int flag=0;
           bool fail=0;
           for(int x=0; x<brd.size.x; x++) {
               for(int y=0; y<brd.size.y; y++) {
                   tile* tile = getTile(&brd, x, y);
                   if(tile->mine && tile->show) {
                       fail = 1;
                   }
                   if(!tile->mine && !tile->show) {
                       clear = 0;
                   }
                   if(tile->mine && tile->flag) {
                       flag++;
                   }else if(tile->mine && !tile->flag) {
                       flag = 0;
                   }
               }
           }
           if(fail) {
               playing = 0;
               win = 0;
               break;
           }
           if(flag == brd.mines || clear) {
               playing = 0;
               win = 1;
               break;
           }
           //Input loop
           while(1) {
               //Print TUI
               KL_flush(KL_stdterm, TERMINAL_OUTPUT);
               printMinefield(&brd);
               KL_swritef(KL_stdterm, "You have %d flags.\n", brd.flags);
               for(int i=0; i<options.size; i++) {
                   if(*KL_getRtn(i, &options)->name != '\0') {
                       KL_swritef(KL_stdterm, "%d-%s\n", i, KL_getRtn(i, &options)->name);
                   }
               }
               KL_swrite(KL_stdterm, "Choose an option.\n");
               //Get input
               int wcurx=KL_stdterm->ocurx, wcury=KL_stdterm->ocury;
               KL_gotoxy(KL_stdterm, curx, cury);
               KL_stdterm->flags = TERMINAL_MOVE_OCUR | TERMINAL_CURSOR;
               char c = KL_cread(KL_stdterm);
               int opt = c-'0';
               curx = KL_stdterm->ocurx;
               cury = KL_stdterm->ocury;
               KL_gotoxy(KL_stdterm, wcurx, wcury);
               KL_stdterm->flags = TERMINAL_DEFAULT_FLAGS;
               //Parse the input
               if(c < '0' || c > '9') {
                   KL_swrite(KL_stdterm, "Please enter a number.\n");
                   KL_cread(KL_stdterm);
                   continue;
               }
               KL_routine* rtn;
               if(!(rtn = KL_getRtn(opt, &options))){
                   KL_swrite(KL_stdterm, "Please enter a correct option.\n");
                   KL_cread(KL_stdterm);
                   continue;
               }
               if(rtn->func) {
                   rtn->func();
               }else {
                   continue;
               }
               break;
           }
       }
       //Show statistics
       showAll(&brd);
       KL_flush(KL_stdterm, TERMINAL_OUTPUT);
       printMinefield(&brd);
       KL_swrite(KL_stdterm, "Game Over\n");
       if(win) {
           KL_swrite(KL_stdterm, "You win! Yay!\n");
       }else {
           KL_swrite(KL_stdterm, "You lose! Hahaha!\n");
       }
       KL_swritef(KL_stdterm, "Time: %ds\n", time(0)-starttime);
       KL_swrite(KL_stdterm, "Press any key to continue...\n");
       KL_cread(KL_stdterm);
       destroyMinefield(&brd);
   }
   stop();
   return 0;
}*/
