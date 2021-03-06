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
#include "shapes.h"
#include "renderers.h"

float triangle(float i) {
    return fabs(1-fmod(fabs(i-M_PI/2)/M_PI, 2))*2-1;
}
float squarer(float angle, float sidelen) {
    float sides = 4;
    float low = sidelen/2; //Lowest radius possible (for squares)
    float high = sqrt(low*low+low*low); //Highest radius possible (for squares)
    float radius = 1-fabs(cos(angle*sides/2));
    return low+radius*(high-low);
}

Circle::Circle(float radius) : Shape() {
    this->family.pushBack("Circle");
    this->radius = radius;
}
Circle::Circle(float radius, float restitution, float mass) : Shape(restitution, mass) {
    this->family.pushBack("Circle");
    this->radius = radius;
}
Circle::Circle(float radius, float restitution, float mass, int r, int g, int b, int a) : Shape(restitution, mass, r, g, b, a) {
    this->family.pushBack("Circle");
    this->radius = radius;
}
Circle::Circle(float radius, float restitution, float mass, SDL_Surface* texture) : Shape(restitution, mass, texture) {
    this->family.pushBack("Circle");
    this->radius = radius;
}
Circle::~Circle() {
}
void Circle::render(Renderer* renderer) {
    Shape::render(renderer);
    if(!(2 < renderer->family.size() && !strcmp(renderer->family[2], "SDLRenderer"))) return;
    ((SDLRenderer*)renderer)->drawCircle(this->getPos(), this->radius, ((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
    ((SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius(this->getOri()).x
                    ,this->getPos().y+(float)sin(this->getOri().y)*this->getRadius(this->getOri()).x}
                    ,((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
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
Square::Square(float sidelen, float restitution, float mass) : Shape(restitution, mass) {
    this->family.pushBack("Square");
    this->sidelen = sidelen;
}
Square::Square(float sidelen, float restitution, float mass, int r, int g, int b, int a) : Shape(restitution, mass, r, g, b, a) {
    this->family.pushBack("Square");
    this->sidelen = sidelen;
}
Square::Square(float sidelen, float restitution, float mass, SDL_Surface* texture) : Shape(restitution, mass, texture) {
    this->family.pushBack("Square");
    this->sidelen = sidelen;
}
Square::~Square() {
}
void Square::render(Renderer* renderer) {
    Shape::render(renderer);
    if(!(2 < renderer->family.size() && !strcmp(renderer->family[2], "SDLRenderer"))) return;
    ((SDLRenderer*)renderer)->drawSquare(this->getPos(), this->getOri(), this->sidelen, ((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
    ((SDLRenderer*)renderer)->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getOri().y)*this->getRadius({-this->getOri().x, -this->getOri().y}).x
                    ,this->getPos().y+(float)sin(this->getOri().y)*this->getRadius({-this->getOri().x, -this->getOri().y}).x}
                    ,((SDLRenderer*)renderer)->mapRGBA(this->r, this->g, this->b, this->a));
}
V2f Square::getRadius(V2f angle) {
    return {squarer(-(this->getOri().y+angle.y), this->sidelen), angle.y};
}
V2f Square::getNormal(V2f angle) {
    return {0, angle.y};
}
