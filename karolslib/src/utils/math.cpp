/*
 * math.cpp
 * karolslib Source Code
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
#include <math.h>
#include <src/utils/math.h>

V2i operator+(const V2i& lvalue, const V2i& rvalue) {
    return {lvalue.x+rvalue.x, lvalue.y+rvalue.y};
}
V2f operator+(const V2f& lvalue, const V2f& rvalue) {
    return {lvalue.x+rvalue.x, lvalue.y+rvalue.y};
}
V2d operator+(const V2d& lvalue, const V2d& rvalue) {
    return {lvalue.x+rvalue.x, lvalue.y+rvalue.y};
}
V3i operator+(const V3i& lvalue, const V3i& rvalue) {
    return {lvalue.x+rvalue.x, lvalue.y+rvalue.y, lvalue.z+rvalue.z};
}
V3f operator+(const V3f& lvalue, const V3f& rvalue) {
    return {lvalue.x+rvalue.x, lvalue.y+rvalue.y, lvalue.z+rvalue.z};
}
V3d operator+(const V3d& lvalue, const V3d& rvalue) {
    return {lvalue.x+rvalue.x, lvalue.y+rvalue.y, lvalue.z+rvalue.z};
}
V2i operator-(const V2i& lvalue, const V2i& rvalue) {
    return {lvalue.x-rvalue.x, lvalue.y-rvalue.y};
}
V2f operator-(const V2f& lvalue, const V2f& rvalue) {
    return {lvalue.x-rvalue.x, lvalue.y-rvalue.y};
}
V2d operator-(const V2d& lvalue, const V2d& rvalue) {
    return {lvalue.x-rvalue.x, lvalue.y-rvalue.y};
}
V3i operator-(const V3i& lvalue, const V3i& rvalue) {
    return {lvalue.x-rvalue.x, lvalue.y-rvalue.y, lvalue.z-rvalue.z};
}
V3f operator-(const V3f& lvalue, const V3f& rvalue) {
    return {lvalue.x-rvalue.x, lvalue.y-rvalue.y, lvalue.z-rvalue.z};
}
V3d operator-(const V3d& lvalue, const V3d& rvalue) {
    return {lvalue.x-rvalue.x, lvalue.y-rvalue.y, lvalue.z-rvalue.z};
}
bool operator==(const V2i& lvalue, const V2i& rvalue) {
    return lvalue.x==rvalue.x && lvalue.y==rvalue.y;
}
bool operator==(const V2f& lvalue, const V2f& rvalue) {
    return lvalue.x==rvalue.x && lvalue.y==rvalue.y;
}
bool operator==(const V2d& lvalue, const V2d& rvalue) {
    return lvalue.x==rvalue.x && lvalue.y==rvalue.y;
}
bool operator==(const V3i& lvalue, const V3i& rvalue) {
    return lvalue.x==rvalue.x && lvalue.y==rvalue.y && lvalue.z==rvalue.z;
}
bool operator==(const V3f& lvalue, const V3f& rvalue) {
    return lvalue.x==rvalue.x && lvalue.y==rvalue.y && lvalue.z==rvalue.z;
}
bool operator==(const V3d& lvalue, const V3d& rvalue) {
    return lvalue.x==rvalue.x && lvalue.y==rvalue.y && lvalue.z==rvalue.z;
}

V2f carteToPolar(V2f carte) {
    return {(float)sqrt(pow(carte.x, 2)+pow(carte.y, 2)), fatp({0, 0}, carte)};
}
V2f polarToCarte(V2f polar) {
    return {polar.x*(float)cos(polar.y), polar.x*(float)sin(polar.y)};
}
bool squareInSquare(square sqr1, square sqr2) {
    return (sqr2.x1 >= sqr1.x1 && sqr2.x1 <= sqr1.x2 && sqr2.y1 >= sqr1.y1 && sqr2.y1 <= sqr1.y2) ||
           (sqr2.x2 >= sqr1.x1 && sqr2.x2 <= sqr1.x2 && sqr2.y2 >= sqr1.y1 && sqr2.y2 <= sqr1.y2) ||
           (sqr2.x1 >= sqr1.x1 && sqr2.x1 <= sqr1.x2 && sqr2.y2 >= sqr1.y1 && sqr2.y2 <= sqr1.y2) ||
           (sqr2.x2 >= sqr1.x1 && sqr2.x2 <= sqr1.x2 && sqr2.y1 >= sqr1.y1 && sqr2.y1 <= sqr1.y2);
}
bool squareInSquareCart(square sqr1, square sqr2) {
    return (sqr2.x1 >= sqr1.x1 && sqr2.x1 <= sqr1.x2 && sqr2.y1 <= sqr1.y1 && sqr2.y1 >= sqr1.y2) ||
           (sqr2.x1 >= sqr1.x1 && sqr2.x1 <= sqr1.x2 && sqr2.y1 <= sqr1.y1 && sqr2.y1 >= sqr1.y2) ||
           (sqr2.x2 >= sqr1.x1 && sqr2.x2 <= sqr1.x2 && sqr2.y1 <= sqr1.y1 && sqr2.y1 >= sqr1.y2) ||
           (sqr2.x2 >= sqr1.x1 && sqr2.x2 <= sqr1.x2 && sqr2.y2 <= sqr1.y1 && sqr2.y2 >= sqr1.y1);;
}
bool fpil(line line, V point) { //float point in line
    float a=line.x2-line.x1,b=line.y2-line.y1;
    if(line.x1<line.x2) {
        if(line.y1<line.y2) {
            if(!squareInSquare({line.x1,line.y1,line.x2,line.y2}, {point.x,point.y,point.x,point.y}))
                return 0;
        }else {
            if(!squareInSquare({line.x1,line.y2,line.x2,line.y1}, {point.x,point.y,point.x,point.y}))
                return 0;
        }
    }else {
        if(line.y1<line.y2) {
            if(!squareInSquare({line.x2,line.y1,line.x1,line.y2}, {point.x,point.y,point.x,point.y}))
                return 0;
        }else {
            if(!squareInSquare({line.x2,line.y2,line.x1,line.y1}, {point.x,point.y,point.x,point.y}))
                return 0;
        }
    }
    if(a>=b) {
        if(!a==0) {
            if((line.y1+b*(point.x-line.x1)/a)==point.y)
                return 1;
            else
                return 0;
        }else {
            if(line.y1 == point.y)
                return 1;
            else
                return 0;
        }
    }else {
        if(!b==0) {
            if((line.x1+a*(point.y-line.y1)/b)==point.x) {
                return 1;
            }else {
                return 0;
            }
        }else {
            if(line.x1 == point.x) {
                return 1;
            }else {
                return 0;
            }
        }
    }
    return 0;
}
bool fpilu(line line, V point, float unprec) { //float point in line with unprecision
    float a=line.x2-line.x1,b=line.y2-line.y1;
    if(line.x1<line.x2) {
        if(line.y1<line.y2) {
            if(!squareInSquare({line.x1-unprec,line.y1-unprec,line.x2+unprec,line.y2+unprec}, {point.x,point.y,point.x,point.y}))
                return 0;
        }else {
            if(!squareInSquare({line.x1-unprec,line.y2-unprec,line.x2+unprec,line.y1+unprec}, {point.x,point.y,point.x,point.y}))
                return 0;
        }
    }else {
        if(line.y1<line.y2) {
            if(!squareInSquare({line.x2-unprec,line.y1-unprec,line.x1+unprec,line.y2+unprec}, {point.x,point.y,point.x,point.y}))
                return 0;
        }else {
            if(!squareInSquare({line.x2-unprec,line.y2-unprec,line.x1+unprec,line.y1+unprec}, {point.x,point.y,point.x,point.y}))
                return 0;
        }
    }
    if(a>=b) {
        if(!a==0) {
            if(((line.y1+b*(point.x-line.x1)/a)<=point.y+unprec) && ((line.y1+b*(point.x-line.x1)/a)>=point.y-unprec))
                return 1;
            else
                return 0;
        }else {
            if((line.y1 <= point.y+unprec) && (line.y1 >= point.y-unprec))
                return 1;
            else
                return 0;
        }
    }else {
        if(!b==0) {
            if(((line.x1+a*(point.y-line.y1)/b)<=point.x+unprec) && ((line.x1+a*(point.y-line.y1)/b)>=point.x-unprec))
                return 1;
            else
                return 0;
        }else {
            if((line.x1 <= point.x+unprec) && (line.x1 >= point.x-unprec))
                return 1;
            else
                return 0;
        }
    }
    return 0;
}
float fatp(V p1, V p2) { //float angle to point
    float quarter=0,a,b,angle;
    if((p1 == (V){0, 0} && p2 == (V){0, 0}) || p1 == p2) {
        return 0;
    }
    if(p2.x > p1.x) {
        a=p2.x-p1.x;
    }else {
        a=p1.x-p2.x;
        quarter += 1;
    }
    if(p2.y >= p1.y) {
        b=p2.y-p1.y;
    }else {
        b=p1.y-p2.y;
        quarter += 1;
    }
    if(p2.y < p1.y && p2.x > p1.x)
        quarter = 3;
    angle = asin(b/sqrt(pow(b,2)+pow(a,2)));
    angle = (((M_PI/2)+(M_PI/2)*quarter-angle)*!(floor(quarter/2)==quarter/2)) +
            ((angle+(M_PI/2)*quarter)   * (floor(quarter/2)==quarter/2));
    return angle;
}
