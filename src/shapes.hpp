/*
 * shapes.hpp
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
#ifndef SHAPES_HPP
#define SHAPES_HPP
#include "math.h"
#include "world.hpp"

//TODO: add more shapes

class Circle : public Shape {
    public:
        float radius;
        Circle(float radius);
        Circle(float radius, material mat);
        Circle(float radius, material mat, int r, int g, int b, int a);
        virtual ~Circle();
        virtual void render(Renderer* renderer);
        virtual float getVolume();
        virtual V2f getRadius(V2f angle);
        virtual V2f getNormal(V2f angle);
};
class Square : public Shape {
    public:
        float sidelen;
        Square(float sidelen);
        Square(float sidelen, material mat);
        Square(float sidelen, material mat, int r, int g, int b, int a);
        virtual ~Square();
        virtual void render(Renderer* renderer);
        virtual float getVolume();
        virtual V2f getRadius(V2f angle);
        virtual V2f getNormal(V2f angle);
};

#endif