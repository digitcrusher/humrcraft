/*
 * shapes.hpp
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
#ifndef SHAPES_HPP
#define SHAPES_HPP
#include "math.h"
#include "world.hpp"
#include "renderers.hpp"
#include "graphics.hpp"

//TODO: add more shapes
//TODO: add intersection checking functions like Circle-Circle, Square-Circle etc
//TODO: add Plane and square related functions

namespace humrcraft {
    namespace shapes {
        class Circle : public Shape {
            public:
                float radius;
                Circle(float radius);
                Circle(float radius, struct material material);
                Circle(float radius, struct material material, int r, int g, int b, int a);
                virtual ~Circle();
                virtual void render(Renderer* renderer);
                virtual float getVolume();
                virtual math::V2f getRadius(math::V2f angle);
                virtual math::V2f getNormal(math::V2f angle);
        };
        class Square : public Shape {
            public:
                float sidelen;
                Square(float sidelen);
                Square(float sidelen, struct material material);
                Square(float sidelen, struct material material, int r, int g, int b, int a);
                virtual ~Square();
                virtual void render(Renderer* renderer);
                virtual float getVolume();
                virtual math::V2f getRadius(math::V2f angle);
                virtual math::V2f getNormal(math::V2f angle);
        };
        class Polygon : public Shape {
            public:
                utils::Vector<math::V2f> vertices;
                Polygon() : Shape() {
                    this->family.pushBack("Polygon");
                }
                Polygon(struct material material) : Shape(material) {
                    this->family.pushBack("Polygon");
                }
                Polygon(struct material material, int r, int g, int b, int a) : Shape(material, r, g, b, a) {
                    this->family.pushBack("Polygon");
                }
                virtual ~Polygon() {
                }
                virtual void render(Renderer* renderer) {
                    Shape::render(renderer);
                    if(!this->checkFamily(renderer, "SDLRenderer", 2)) return;
                    for(float i=0; i<math::pi*2; i+=math::pi/4/(math::pi*2*4)) {
                        graphics::drawPixel(((humrcraft::renderers::SDLRenderer*)renderer)->buffer, ((humrcraft::renderers::SDLRenderer*)renderer)->SDLMapPos(this->getPos()).x+sin(i)*this->getRadius({0, i}).x, ((humrcraft::renderers::SDLRenderer*)renderer)->SDLMapPos(this->getPos()).y+cos(i)*this->getRadius({0, i}).x, (int)-1);
                    }
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius(-this->getOri()).x,
                        this->getPos().y+(float)sin(this->getOri().y)*this->getRadius(-this->getOri()).x},
                        ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
                }
                virtual float getVolume() {
                    return 0;
                }
                virtual math::V2f getRadius(math::V2f angle) {
                    if(this->vertices.size() == 0) {
                        return angle;
                    }
                    int i;
                    for(i=0; angle.y<this->vertices[i].y && i<this->vertices.size(); i++);
                    math::V2f a = this->vertices[(i-1+this->vertices.size())&this->vertices.size()];
                    math::V2f apos = {(float)cos(a.y)*a.x, (float)sin(a.y)*a.x};
                    math::V2f b = this->vertices[i];
                    math::V2f bpos = {(float)cos(b.y)*b.x, (float)sin(b.y)*b.x};
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
                    math::V2f pos = {(float)fmin(apos.x, bpos.x)+(((float)fmax(apos.y, bpos.y)-(float)fmin(apos.y, bpos.y))/dy*dx)*x, (float)fmin(apos.y, bpos.y)+(((float)fmax(apos.x, bpos.x)-(float)fmin(apos.x, bpos.x))/dx*dy)*y};
                    return {(float)sqrt(pos.x*pos.x+pos.y*pos.y), angle.y};
                }
                virtual math::V2f getNormal(math::V2f angle) {
                    return {0, 0};
                }
                virtual void addVertex(math::V2f vertex) {
                    this->vertices.pushBack(vertex);
                    for(int i=0; i<this->vertices.size(); i++) { //TODO: replace this with a better sorting algorithm
                        for(int j=i+1; j<this->vertices.size()-i; j++) {
                            if(this->vertices[j].y > this->vertices[j+1].y) {
                                math::V2f swap = this->vertices[j];
                                this->vertices[j] = this->vertices[j+1];
                                this->vertices[j+1] = swap;
                            }
                        }
                    }
                }
        };
        class Rectangle : public Shape {
            public:
                math::V2fPair rectangle; //Relative to the center of the shape
                Rectangle(math::V2fPair rectangle) : Shape() {
                    this->family.pushBack("Rectangle");
                    this->rectangle = rectangle;
                }
                Rectangle(math::V2fPair rectangle, struct material material) : Shape(material) {
                    this->family.pushBack("Rectangle");
                    this->rectangle = rectangle;
                }
                Rectangle(math::V2fPair rectangle, struct material material, int r, int g, int b, int a) : Shape(material, r, g, b, a) {
                    this->family.pushBack("Rectangle");
                    this->rectangle = rectangle;
                }
                virtual ~Rectangle() {
                }
                virtual void render(Renderer* renderer) {
                    Shape::render(renderer);
                    if(!this->checkFamily(renderer, "SDLRenderer", 2)) return;
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius(-this->getOri()).x,
                        this->getPos().y+(float)sin(this->getOri().y)*this->getRadius(-this->getOri()).x},
                        ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawRectangle(this->getPos()+this->rectangle.v1, this->getPos()+this->rectangle.v2,
                        ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawLine(this->getPos(), (math::V2f){this->rectangle.v1.x, this->rectangle.v1.y}+this->getPos(),
                        ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawLine(this->getPos(), (math::V2f){this->rectangle.v2.x, this->rectangle.v2.y}+this->getPos(),
                        ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawLine(this->getPos(), (math::V2f){this->rectangle.v1.x, this->rectangle.v2.y}+this->getPos(),
                        ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
                    ((humrcraft::renderers::SDLRenderer*)renderer)->drawLine(this->getPos(), (math::V2f){this->rectangle.v2.x, this->rectangle.v1.y}+this->getPos(),
                        ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
                }
                virtual float getVolume() {
                    return (this->rectangle.v2.x+-this->rectangle.v1.x)*(this->rectangle.v2.y+-this->rectangle.v1.y);
                }
                virtual math::V2f getRadius(math::V2f angle) {
                    return angle;
                }
                virtual math::V2f getNormal(math::V2f angle) {
                    angle = {angle.x, (float)(angle.y < 0 ? math::pi*2-fmod(angle.y, math::pi*2) : fmod(angle.y, math::pi*2))};
                    if(angle.y > this->rectangle.v2.x/(this->rectangle.v2.x+-this->rectangle.v1.y)*math::pi/2+math::pi/2*3) {
                        return {angle.x, (float)math::pi*2};
                    }else if(angle.y > -this->rectangle.v1.y/(-this->rectangle.v1.y+-this->rectangle.v1.x)*math::pi/2+math::pi) {
                        return {angle.x, (float)math::pi/2*3};
                    }else if(angle.y > -this->rectangle.v1.x/(-this->rectangle.v1.x+this->rectangle.v2.y)*math::pi/2+math::pi/2) {
                        return {angle.x, (float)math::pi};
                    }else if(angle.y > this->rectangle.v2.y/(this->rectangle.v2.y+this->rectangle.v2.x)*math::pi/2) {
                        return {angle.x, (float)math::pi/2};
                    }else {
                        return {angle.x, 0};
                    }
                    return angle;
                }
        };

        bool circleCircle(struct manifold* manifold, World* world);
        bool rectangleRectangle(struct manifold* manifold, World* world);
    }
}

#endif
