/*
 * shapes.h
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
#ifndef SHAPES_H
#define SHAPES_H
#include "world.h"

float triangle(float i);
float squarer(float angle, float sidelen);

class Circle : public Shape {
    public:
        float radius;
        Circle(float radius);
        Circle(float radius, float restitution, float mass);
        Circle(float radius, float restitution, float mass, int r, int g, int b, int a);
        Circle(float radius, float restitution, float mass, SDL_Surface* texture);
        virtual ~Circle();
        virtual void render(Renderer* renderer);
        virtual float getRadius(V2f angle);
        virtual V2f getNormal(V2f angle);
};
class Square : public Shape {
    public:
        float sidelen;
        Square(float sidelen);
        Square(float sidelen, float restitution, float mass);
        Square(float sidelen, float restitution, float mass, int r, int g, int b, int a);
        Square(float sidelen, float restitution, float mass, SDL_Surface* texture);
        virtual ~Square();
        virtual void render(Renderer* renderer);
        virtual float getRadius(V2f angle);
        virtual V2f getNormal(V2f angle);
};

#endif
