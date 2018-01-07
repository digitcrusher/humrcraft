/*
 * textsweeper.hpp
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
#ifndef TEXTSWEEPER_HPP
#define TEXTSWEEPER_HPP
#include "game.hpp"
#include "math.hpp"
#include "utils.hpp"

namespace textsweeper { //add class Textsweeper : public humrcraft::game::Game
    class Tile;
    class Minefield;
    class Player;

    class Tile : public humrcraft::game::Block {
        public:
            int sum;
            bool expose, mine, flag, wrong;
            bool pressed;
            Tile(void* data, int datasize, Thing* (*recreatef)(Thing* base, void* data, int datasize), void (*initf)(Thing* thing), void (*uninitf)(Thing* thing));
            virtual ~Tile();
            virtual void render(humrcraft::Renderer* renderer);
            static Thing* defaultRecreatef(Thing* base, void* data, int datasize);
    };
    class Minefield : public humrcraft::game::Level {
        public:
            /* Game States:
             * 0-Playing
             * 1-Win
             * 2-Lose
             */
            utils::timer timer;
            int turns;
            int flags, mines;
            Minefield(math::V2iPair size);
            virtual ~Minefield();
            virtual bool flag(math::V2i pos);
            virtual bool expose(math::V2i pos, math::V2i* neighbors, int neighborssize, bool recursion);
            virtual int getGameState();
            virtual void showAll();
            virtual void resetMinefield();
            virtual void plantMines(int seed, int mines, math::V2iPair safezone);
            virtual void calculateSums(math::V2i* neighbors, int neighborssize);
            virtual void generateMinefield(int seed, int mines, math::V2iPair safezone, math::V2i* neighbors, int neighborssize);
    };
    class Player {
        public:
            Minefield* board;
            void (*turnf)(Minefield* minefield);
            int score;
            Player(void (*turnf)(Minefield* minefield));
            virtual ~Player();
            virtual void turn();
            virtual void setMinefield(Minefield* board);
    };
    extern math::V2i normalneighbors[];
    extern int normalneighborssize;
    extern math::V2i knightneighbors[];
    extern int knightneighborssize;
    extern math::V2i orthogonalneighbors[];
    extern int orthogonalneighborssize;
    extern math::V2i diagonalneighbors[];
    extern int diagonalneighborssize;
}


#endif
