/*
 * shapes.cpp
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
#include "shapes.h"

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
    this->radius = radius;
}
Circle::Circle(float radius, float restitution, float mass) : Shape(restitution, mass) {
    this->radius = radius;
}
Circle::Circle(float radius, float restitution, float mass, int r, int g, int b) : Shape(restitution, mass, r, g, b) {
    this->radius = radius;
}
Circle::~Circle() {
}
void Circle::render(Renderer* render) {
    render->drawCircle(this->getPos(), this->radius, render->mapRGB(this->r, this->g, this->b));
    render->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getRot().y)*this->getRadius(this->getRot())
                    ,this->getPos().y+(float)sin(this->getRot().y)*this->getRadius(this->getRot())}, render->mapRGB(this->r, this->g, this->b));
}
float Circle::getRadius(V2f angle) {
    return this->radius;
}
V2f Circle::getNormal(V2f angle) {
    return {0, angle.y};
}

Square::Square(float sidelen) : Shape() {
    this->sidelen = sidelen;
}
Square::Square(float radius, float restitution, float mass) : Shape(restitution, mass) {
    this->sidelen = sidelen;
}
Square::Square(float radius, float restitution, float mass, int r, int g, int b) : Shape(restitution, mass, r, g, b) {
    this->sidelen = sidelen;
}
Square::~Square() {
}
void Square::render(Renderer* render) {
    render->drawSquare(this->getPos(), this->getRot(), this->sidelen, render->mapRGB(this->r, this->g, this->b));
    render->drawLine(this->getPos(), {this->getPos().x+(float)cos(this->getRot().y)*this->getRadius({-this->getRot().x, -this->getRot().y})
                    ,this->getPos().y+(float)sin(this->getRot().y)*this->getRadius({-this->getRot().x, -this->getRot().y})}
                    ,render->mapRGB(this->r, this->g, this->b));
}
float Square::getRadius(V2f angle) {
    return squarer(-(this->getRot().y+angle.y), this->sidelen);
}
V2f Square::getNormal(V2f angle) {
    return {0, angle.y};
}
