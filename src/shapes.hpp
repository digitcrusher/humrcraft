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

#include "renderers.hpp"
#include "graphics.hpp"

//TODO: add more shapes
//TODO: add intersection checking functions like Circle-Circle, Square-Circle etc
//TODO: add Plane and square related functions

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
/*class Polygon : public Shape {
    public:
        Vector<V2f> vertices;
        Polygon() : Shape() {
            this->family.pushBack("Polygon");
        }
        Polygon(material mat) : Shape(mat) {
            this->family.pushBack("Polygon");
        }
        Polygon(material mat, int r, int g, int b, int a) : Shape(mat, r, g, b, a) {
            this->family.pushBack("Polygon");
        }
        virtual ~Polygon() {
        }
        virtual void render(Renderer* renderer) {
            Shape::render(renderer);
            if(!this->checkFamily(renderer, "SDLRenderer", 2)) return;
            for(double i=0; i<M_PI*2; i+=M_PI/4/(M_PI*2*4)) {
                drawPixel(((SDLRenderer*)renderer)->buffer, ((SDLRenderer*)renderer)->SDLMapPos(this->getPos()).x+sin(i)*this->getRadius({0, i}).x, ((SDLRenderer*)renderer)->SDLMapPos(this->getPos()).y+cos(i)*this->getRadius({0, i}).x, (unsigned int)-1);
            }
            ((SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius(-this->getOri()).x,
                            this->getPos().y+(float)sin(this->getOri().y)*this->getRadius(-this->getOri()).x},
                            ((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
        }
        virtual float getVolume() {
            return 0;
        }
        virtual V2f getRadius(V2f angle) {
            if(this->vertices.size() == 0) {
                return angle;
            }
            unsigned int i;
            for(i=0; angle.y<this->vertices[i].y && i<this->vertices.size(); i++);
            V2f a = this->vertices[(i-1+this->vertices.size())&this->vertices.size()];
            V2f apos = {(float)cos(a.y)*a.x, (float)sin(a.y)*a.x};
            V2f b = this->vertices[i];
            V2f bpos = {(float)cos(b.y)*b.x, (float)sin(b.y)*b.x};
            float dx = bpos.x-apos.x;
            float dy = bpos.y-apos.y;
            float x;
            float y;
            if(apos.x < bpos.x) {
                x = 1;
                dx = bpos.x-apos.x;
            }else {
                x = -1;
                dx = apos.x-bpos.x;
            }
            if(apos.y < bpos.y) {
                y = 1;
                dy = bpos.y-apos.y;
            }else {
                y = -1;
                dy = apos.y-bpos.y;
            }
            V2f pos = {(float)fmin(apos.x, bpos.x)+(((float)fmax(apos.y, bpos.y)-(float)fmin(apos.y, bpos.y))/dy*dx)*x, (float)fmin(apos.y, bpos.y)+(((float)fmax(apos.x, bpos.x)-(float)fmin(apos.x, bpos.x))/dx*dy)*y};
            return {(float)sqrt(pos.x*pos.x+pos.y*pos.y), angle.y};
        }
        virtual V2f getNormal(V2f angle) {
            return {0, 0};
        }
        virtual void addVertex(V2f vertex) {
            this->vertices.pushBack(vertex);
            for(unsigned int i=0; i<this->vertices.size(); i++) { //TODO: replace this with a better sorting algorithm
                for(unsigned int j=i+1; j<this->vertices.size()-i; j++) {
                    if(this->vertices[j].y > this->vertices[j+1].y) {
                        V2f swap = this->vertices[j];
                        this->vertices[j] = this->vertices[j+1];
                        this->vertices[j+1] = swap;
                    }
                }
            }
        }
};*/
class Rectangle : public Shape {
    public:
        V2f start, end;
        Rectangle(V2f start, V2f end) : Shape() {
            this->family.pushBack("Rectangle");
            this->start = start;
            this->end = end;
        }
        Rectangle(V2f start, V2f end, material mat) : Shape(mat) {
            this->family.pushBack("Rectangle");
            this->start = start;
            this->end = end;
        }
        Rectangle(V2f start, V2f end, material mat, int r, int g, int b, int a) : Shape(mat, r, g, b, a) {
            this->family.pushBack("Rectangle");
            this->start = start;
            this->end = end;
        }
        virtual ~Rectangle() {
        }
        virtual void render(Renderer* renderer) {
            Shape::render(renderer);
            if(!this->checkFamily(renderer, "SDLRenderer", 2)) return;
            ((SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius(-this->getOri()).x,
                            this->getPos().y+(float)sin(this->getOri().y)*this->getRadius(-this->getOri()).x},
                            ((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
        }
        virtual float getVolume() {
            return abs(this->end.x-this->start.x)*abs(this->end.y-this->start.y);
        }
        virtual V2f getRadius(V2f angle) {
            return {0, 0};
        }
        virtual V2f getNormal(V2f angle) {
            return {0, 0};
        }
};

bool circleCircle(manifold* manifold, World* world);
bool rectangleRectangle(manifold* manifold, World* world);

#endif
