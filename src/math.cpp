/*
 * math.cpp
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
#include "math.hpp"

V2i operator-(const V2i& rvalue) {
    return {-rvalue.x, -rvalue.y};
}
V2f operator-(const V2f& rvalue) {
    return {-rvalue.x, -rvalue.y};
}
V2d operator-(const V2d& rvalue) {
    return {-rvalue.x, -rvalue.y};
}
V3i operator-(const V3i& rvalue) {
    return {-rvalue.x, -rvalue.y, -rvalue.z};
}
V3f operator-(const V3f& rvalue) {
    return {-rvalue.x, -rvalue.y, -rvalue.z};
}
V3d operator-(const V3d& rvalue) {
    return {-rvalue.x, -rvalue.y, -rvalue.z};
}
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
V2i operator*(const V2i& lvalue, const V2i& rvalue) {
    return {lvalue.x*rvalue.x, lvalue.y*rvalue.y};
}
V2f operator*(const V2f& lvalue, const V2f& rvalue) {
    return {lvalue.x*rvalue.x, lvalue.y*rvalue.y};
}
V2d operator*(const V2d& lvalue, const V2d& rvalue) {
    return {lvalue.x*rvalue.x, lvalue.y*rvalue.y};
}
V3i operator*(const V3i& lvalue, const V3i& rvalue) {
    return {lvalue.x*rvalue.x, lvalue.y*rvalue.y, lvalue.z*rvalue.z};
}
V3f operator*(const V3f& lvalue, const V3f& rvalue) {
    return {lvalue.x*rvalue.x, lvalue.y*rvalue.y, lvalue.z*rvalue.z};
}
V3d operator*(const V3d& lvalue, const V3d& rvalue) {
    return {lvalue.x*rvalue.x, lvalue.y*rvalue.y, lvalue.z*rvalue.z};
}
V2i operator/(const V2i& lvalue, const V2i& rvalue) {
    return {lvalue.x/rvalue.x, lvalue.y/rvalue.y};
}
V2f operator/(const V2f& lvalue, const V2f& rvalue) {
    return {lvalue.x/rvalue.x, lvalue.y/rvalue.y};
}
V2d operator/(const V2d& lvalue, const V2d& rvalue) {
    return {lvalue.x/rvalue.x, lvalue.y/rvalue.y};
}
V3i operator/(const V3i& lvalue, const V3i& rvalue) {
    return {lvalue.x/rvalue.x, lvalue.y/rvalue.y, lvalue.z/rvalue.z};
}
V3f operator/(const V3f& lvalue, const V3f& rvalue) {
    return {lvalue.x/rvalue.x, lvalue.y/rvalue.y, lvalue.z/rvalue.z};
}
V3d operator/(const V3d& lvalue, const V3d& rvalue) {
    return {lvalue.x/rvalue.x, lvalue.y/rvalue.y, lvalue.z/rvalue.z};
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

float trianglewave(float i) {
    return fabs(1-fmod(fabs(i-M_PI/2)/M_PI, 2))*2-1;
}
float squarer(float angle, float sidelen) {
    float sides = 4;
    float low = sidelen/2; //Lowest radius possible (for squares)
    float high = sqrt(low*low+low*low); //Highest radius possible (for squares)
    float x = 1-fmod(angle/(M_PI*2/sides)*2+1, 2);
    float a = (high-low);
    float c = low;
    return a*pow(x, 2)+c;
}
V2f carteToPolar(V2f carte) {
    return {(float)sqrt(pow(carte.x, 2)+pow(carte.y, 2)), fatp({0, 0}, carte).y};
}
V2f polarToCarte(V2f polar) {
    return {polar.x*(float)cos(polar.y), polar.x*(float)sin(polar.y)};
}
bool squareInSquare(square sqr1, square sqr2) {
    return (sqr2.pos1.x >= sqr1.pos1.x && sqr2.pos1.x <= sqr1.pos2.x && sqr2.pos1.y >= sqr1.pos1.y && sqr2.pos1.y <= sqr1.pos2.y) ||
           (sqr2.pos2.x >= sqr1.pos1.x && sqr2.pos2.x <= sqr1.pos2.x && sqr2.pos2.y >= sqr1.pos1.y && sqr2.pos2.y <= sqr1.pos2.y) ||
           (sqr2.pos1.x >= sqr1.pos1.x && sqr2.pos1.x <= sqr1.pos2.x && sqr2.pos2.y >= sqr1.pos1.y && sqr2.pos2.y <= sqr1.pos2.y) ||
           (sqr2.pos2.x >= sqr1.pos1.x && sqr2.pos2.x <= sqr1.pos2.x && sqr2.pos1.y >= sqr1.pos1.y && sqr2.pos1.y <= sqr1.pos2.y);
}
bool squareInSquareCart(square sqr1, square sqr2) {
    return (sqr2.pos1.x >= sqr1.pos1.x && sqr2.pos1.x <= sqr1.pos2.x && sqr2.pos1.y <= sqr1.pos1.y && sqr2.pos1.y >= sqr1.pos2.y) ||
           (sqr2.pos1.x >= sqr1.pos1.x && sqr2.pos1.x <= sqr1.pos2.x && sqr2.pos1.y <= sqr1.pos1.y && sqr2.pos1.y >= sqr1.pos2.y) ||
           (sqr2.pos2.x >= sqr1.pos1.x && sqr2.pos2.x <= sqr1.pos2.x && sqr2.pos1.y <= sqr1.pos1.y && sqr2.pos1.y >= sqr1.pos2.y) ||
           (sqr2.pos2.x >= sqr1.pos1.x && sqr2.pos2.x <= sqr1.pos2.x && sqr2.pos2.y <= sqr1.pos1.y && sqr2.pos2.y >= sqr1.pos1.y);
}
bool fpil(line line, V2f point) { //float point in line
    float a=line.pos2.x-line.pos1.x,b=line.pos2.y-line.pos1.y;
    if(line.pos1.x<line.pos2.x) {
        if(line.pos1.y<line.pos2.y) {
            if(!squareInSquare({line.pos1.x,line.pos1.y,line.pos2.x,line.pos2.y}, {point.x,point.y,point.x,point.y}))
                return 0;
        }else {
            if(!squareInSquare({line.pos1.x,line.pos2.y,line.pos2.x,line.pos1.y}, {point.x,point.y,point.x,point.y}))
                return 0;
        }
    }else {
        if(line.pos1.y<line.pos2.y) {
            if(!squareInSquare({line.pos2.x,line.pos1.y,line.pos1.x,line.pos2.y}, {point.x,point.y,point.x,point.y}))
                return 0;
        }else {
            if(!squareInSquare({line.pos2.x,line.pos2.y,line.pos1.x,line.pos1.y}, {point.x,point.y,point.x,point.y}))
                return 0;
        }
    }
    if(a>=b) {
        if(!a==0) {
            if((line.pos1.y+b*(point.x-line.pos1.x)/a)==point.y)
                return 1;
            else
                return 0;
        }else {
            if(line.pos1.y == point.y)
                return 1;
            else
                return 0;
        }
    }else {
        if(!b==0) {
            if((line.pos1.x+a*(point.y-line.pos1.y)/b)==point.x) {
                return 1;
            }else {
                return 0;
            }
        }else {
            if(line.pos1.x == point.x) {
                return 1;
            }else {
                return 0;
            }
        }
    }
    return 0;
}
bool fpilu(line line, V2f point, float unprec) { //float point in line with unprecision
    float a=line.pos2.x-line.pos1.x,b=line.pos2.y-line.pos1.y;
    if(line.pos1.x<line.pos2.x) {
        if(line.pos1.y<line.pos2.y) {
            if(!squareInSquare({line.pos1.x-unprec,line.pos1.y-unprec,line.pos2.x+unprec,line.pos2.y+unprec}, {point.x,point.y,point.x,point.y}))
                return 0;
        }else {
            if(!squareInSquare({line.pos1.x-unprec,line.pos2.y-unprec,line.pos2.x+unprec,line.pos1.y+unprec}, {point.x,point.y,point.x,point.y}))
                return 0;
        }
    }else {
        if(line.pos1.y<line.pos2.y) {
            if(!squareInSquare({line.pos2.x-unprec,line.pos1.y-unprec,line.pos1.x+unprec,line.pos2.y+unprec}, {point.x,point.y,point.x,point.y}))
                return 0;
        }else {
            if(!squareInSquare({line.pos2.x-unprec,line.pos2.y-unprec,line.pos1.x+unprec,line.pos1.y+unprec}, {point.x,point.y,point.x,point.y}))
                return 0;
        }
    }
    if(a>=b) {
        if(!a==0) {
            if(((line.pos1.y+b*(point.x-line.pos1.x)/a)<=point.y+unprec) && ((line.pos1.y+b*(point.x-line.pos1.x)/a)>=point.y-unprec))
                return 1;
            else
                return 0;
        }else {
            if((line.pos1.y <= point.y+unprec) && (line.pos1.y >= point.y-unprec))
                return 1;
            else
                return 0;
        }
    }else {
        if(!b==0) {
            if(((line.pos1.x+a*(point.y-line.pos1.y)/b)<=point.x+unprec) && ((line.pos1.x+a*(point.y-line.pos1.y)/b)>=point.x-unprec))
                return 1;
            else
                return 0;
        }else {
            if((line.pos1.x <= point.x+unprec) && (line.pos1.x >= point.x-unprec))
                return 1;
            else
                return 0;
        }
    }
    return 0;
}
V2f fatp(V2f p1, V2f p2) { //float angle to point
    float quarter=0,a,b,angle;
    if(p1 == p2) {
        return {0, 0};
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
    angle = (((M_PI/2)+(M_PI/2)*quarter-angle)*!(floor(quarter/2)==quarter/2))+((angle+(M_PI/2)*quarter)*(floor(quarter/2)==quarter/2));
    return {0, angle};
}
