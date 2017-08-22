/*
 * shapes.cpp
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
    ((SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius({-this->getOri().x, -this->getOri().y}).x,
                    this->getPos().y+(float)sin(this->getOri().y)*this->getRadius({-this->getOri().x, -this->getOri().y}).x},
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
