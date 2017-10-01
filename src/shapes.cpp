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

Circle::Circle(float radius) : Shape() {
    this->family.pushBack("Circle");
    this->radius = radius;
}
Circle::Circle(float radius, material mat) : Shape(mat) {
    this->family.pushBack("Circle");
    this->radius = radius;
}
Circle::Circle(float radius, material mat, int r, int g, int b, int a) : Shape(mat, r, g, b, a) {
    this->family.pushBack("Circle");
    this->radius = radius;
}
Circle::~Circle() {
}
void Circle::render(Renderer* renderer) {
    Shape::render(renderer);
    if(!this->checkFamily(renderer, "SDLRenderer", 2)) return;
    ((SDLRenderer*)renderer)->drawCircle(this->getPos(), this->radius, ((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
    ((SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius(this->getOri()).x,
                    this->getPos().y+(float)sin(this->getOri().y)*this->getRadius(this->getOri()).x},
                    ((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
}
float Circle::getVolume() {
    return M_PI*this->radius*this->radius;
}
V2f Circle::getRadius(V2f angle) {
    return {this->radius, angle.y};
}
V2f Circle::getNormal(V2f angle) {
    return {0, angle.y};
}

Square::Square(float sidelen) : Shape() {
    this->family.pushBack("Square");
    this->sidelen = sidelen;
}
Square::Square(float sidelen, material mat) : Shape(mat) {
    this->family.pushBack("Square");
    this->sidelen = sidelen;
}
Square::Square(float sidelen, material mat, int r, int g, int b, int a) : Shape(mat, r, g, b, a) {
    this->family.pushBack("Square");
    this->sidelen = sidelen;
}
Square::~Square() {
}
void Square::render(Renderer* renderer) {
    Shape::render(renderer);
    if(!this->checkFamily(renderer, "SDLRenderer", 2)) return;
    ((SDLRenderer*)renderer)->drawSquare(this->getPos(), this->getOri(), this->sidelen, ((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
    ((SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius(-this->getOri()).x,
                    this->getPos().y+(float)sin(this->getOri().y)*this->getRadius(-this->getOri()).x},
                    ((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
}
float Square::getVolume() {
    return this->sidelen*this->sidelen;
}
V2f Square::getRadius(V2f angle) {
    return {squarer(-(this->getOri().y+angle.y), this->sidelen), angle.y};
}
V2f Square::getNormal(V2f angle) {
    return {0, angle.y};
}

bool circleCircle(manifold* manifold, World* world) {
    if(!manifold->a->shape->checkFamily(manifold->a->shape, "Circle", 2) || !manifold->b->shape->checkFamily(manifold->b->shape, "Circle", 2)) {
        return 1;
    }
    manifold->ra = manifold->a->shape->getRadius(manifold->angle);
    manifold->rb = manifold->b->shape->getRadius(manifold->angle+(V2f){0, M_PI});
    V2f pos1 = {(float)fmax(manifold->a->shape->getPos().x, manifold->b->shape->getPos().x), (float)fmax(manifold->a->shape->getPos().y, manifold->b->shape->getPos().y)};
    V2f pos2 = {(float)fmin(manifold->a->shape->getPos().x, manifold->b->shape->getPos().x), (float)fmin(manifold->a->shape->getPos().y, manifold->b->shape->getPos().y)};
    manifold->penetration = manifold->ra.x+manifold->rb.x-sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2));
    return 0;
}
bool rectangleRectangle(manifold* manifold, World* world) {
    if(!manifold->a->shape->checkFamily(manifold->a->shape, "Rectangle", 2) || !manifold->b->shape->checkFamily(manifold->b->shape, "Rectangle", 2)) {
        return 1;
    }
    return 0;
}
