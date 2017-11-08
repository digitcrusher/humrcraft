/*
 * shapes.cpp
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
#include "shapes.hpp"
#include "renderers.hpp"

#include <iostream>

namespace humrcraft {
    namespace shapes {
        Circle::Circle(float radius) : Shape() {
            this->family.pushBack("Circle");
            this->radius = radius;
        }
        Circle::Circle(float radius, struct material material) : Shape(material) {
            this->family.pushBack("Circle");
            this->radius = radius;
        }
        Circle::Circle(float radius, struct material material, int r, int g, int b, int a) : Shape(material, r, g, b, a) {
            this->family.pushBack("Circle");
            this->radius = radius;
        }
        Circle::~Circle() {
        }
        void Circle::render(Renderer* renderer) {
            Shape::render(renderer);
            if(!this->checkFamily(renderer, "SDLRenderer", 3)) return;
            ((humrcraft::renderers::SDLRenderer*)renderer)->drawCircle(this->getPos(), this->radius, ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
            ((humrcraft::renderers::SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius(this->getOri()).x,
                this->getPos().y+(float)sin(this->getOri().y)*this->getRadius(this->getOri()).x},
                ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
        }
        float Circle::getVolume() {
            return math::pi*this->radius*this->radius;
        }
        math::V2f Circle::getRadius(math::V2f angle) {
            return {this->radius, angle.y};
        }
        math::V2f Circle::getNormal(math::V2f angle) {
            return {0, angle.y};
        }

        Square::Square(float sidelen) : Shape() {
            this->family.pushBack("Square");
            this->sidelen = sidelen;
        }
        Square::Square(float sidelen, struct material material) : Shape(material) {
            this->family.pushBack("Square");
            this->sidelen = sidelen;
        }
        Square::Square(float sidelen, struct material material, int r, int g, int b, int a) : Shape(material, r, g, b, a) {
            this->family.pushBack("Square");
            this->sidelen = sidelen;
        }
        Square::~Square() {
        }
        void Square::render(Renderer* renderer) {
            Shape::render(renderer);
            if(!this->checkFamily(renderer, "SDLRenderer", 3)) return;
            ((humrcraft::renderers::SDLRenderer*)renderer)->drawSquare(this->getPos(), this->getOri(), this->sidelen, ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
            ((humrcraft::renderers::SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius(-this->getOri()).x,
                this->getPos().y+(float)sin(this->getOri().y)*this->getRadius(-this->getOri()).x},
                ((humrcraft::renderers::SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
        }
        float Square::getVolume() {
            return this->sidelen*this->sidelen;
        }
        math::V2f Square::getRadius(math::V2f angle) {
            return {math::squarer(-(this->getOri().y+angle.y), this->sidelen), angle.y};
        }
        math::V2f Square::getNormal(math::V2f angle) {
            return {0, angle.y};
        }

        Rectangle::Rectangle(math::V2fPair rectangle) : Shape() {
            this->family.pushBack("Rectangle");
            this->rectangle = rectangle;
        }
        Rectangle::Rectangle(math::V2fPair rectangle, struct material material) : Shape(material) {
            this->family.pushBack("Rectangle");
            this->rectangle = rectangle;
        }
        Rectangle::Rectangle(math::V2fPair rectangle, struct material material, int r, int g, int b, int a) : Shape(material, r, g, b, a) {
            this->family.pushBack("Rectangle");
            this->rectangle = rectangle;
        }
        Rectangle::~Rectangle() {
        }
        void Rectangle::render(Renderer* renderer) {
            Shape::render(renderer);
            if(!this->checkFamily(renderer, "SDLRenderer", 3)) return;
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
        float Rectangle::getVolume() {
            return (this->rectangle.v2.x+-this->rectangle.v1.x)*(this->rectangle.v2.y+-this->rectangle.v1.y);
        }
        math::V2f Rectangle::getRadius(math::V2f angle) {
            return angle;
        }
        math::V2f Rectangle::getNormal(math::V2f angle) {
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

        bool circleCircle(struct manifold* manifold, World* world) {
            if(!manifold || !manifold->a->shape->checkFamily(manifold->a->shape, "Circle", 2) || !manifold->b->shape->checkFamily(manifold->b->shape, "Circle", 2)) {
                return 1;
            }
            manifold->ra = manifold->a->shape->getRadius(manifold->angle);
            manifold->penetration = manifold->ra.x+manifold->rb.x-math::distance(manifold->a->shape->getPos(), manifold->b->shape->getPos());
            return 0;
        }
        bool rectangleRectangle(struct manifold* manifold, World* world) {
            if(!manifold || !manifold->a->shape->checkFamily(manifold->a->shape, "Rectangle", 2) || !manifold->b->shape->checkFamily(manifold->b->shape, "Rectangle", 2)) {
                return 1;
            }
            Rectangle* a = (Rectangle*)manifold->a->shape;
            Rectangle* b = (Rectangle*)manifold->b->shape;
            math::V2f diff = {(float)fabs(a->getPos().x-b->getPos().x), (float)fabs(a->getPos().y-b->getPos().y)};
            math::V2f rectside;
            if(a->getPos().x > b->getPos().x) {
                rectside.x = fabs(a->rectangle.v1.x)+fabs(b->rectangle.v2.x);
            }else {
                rectside.x = fabs(a->rectangle.v2.x)+fabs(b->rectangle.v1.x);
            }
            if(a->getPos().y > b->getPos().y) {
                rectside.y = fabs(a->rectangle.v1.y)+fabs(b->rectangle.v2.y);
            }else {
                rectside.y = fabs(a->rectangle.v2.y)+fabs(b->rectangle.v1.y);
            }
            diff = diff-rectside;
            if(diff.x < 0 && diff.y < 0) {
                manifold->penetration = (float)fabs(diff.x>diff.y ? diff.x : diff.y);
            }else {
                manifold->penetration = 0;
            }
            manifold->size = sizeof(math::V2f)*2;
            manifold->data = malloc(manifold->size);
            *(math::V2f*)((char*)manifold->data+sizeof(math::V2f)*0) = diff;
            *(math::V2f*)((char*)manifold->data+sizeof(math::V2f)*1) = rectside;
            return 0;
        }
    }
}
