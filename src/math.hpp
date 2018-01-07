/*
 * math.hpp
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
#ifndef MATH_HPP
#define MATH_HPP
#include <math.h>

namespace math {
    const double pi=3.14159265358979323846;

    struct V2i {
        int x, y;
    };
    struct V2f {
        float x, y;
    };
    struct V2d {
        double x, y;
    };
    struct V3i {
        int x, y, z;
    };
    struct V3f {
        float x, y, z;
    };
    struct V3d {
        double x, y, z;
    };
    struct V2iPair {
        V2i v1, v2;
    };
    struct V2fPair {
        V2f v1, v2;
    };
    struct V2dPair {
        V2d v1, v2;
    };
    struct V3iPair {
        V3i v1, v2;
    };
    struct V3fPair {
        V3f v1, v2;
    };
    struct V3dPair {
        V3d v1, v2;
    };
    V2i operator-(const V2i& rvalue);
    V2f operator-(const V2f& rvalue);
    V2d operator-(const V2d& rvalue);
    V3i operator-(const V3i& rvalue);
    V3f operator-(const V3f& rvalue);
    V3d operator-(const V3d& rvalue);
    V2i operator+(const V2i& lvalue, const V2i& rvalue);
    V2f operator+(const V2f& lvalue, const V2f& rvalue);
    V2d operator+(const V2d& lvalue, const V2d& rvalue);
    V3i operator+(const V3i& lvalue, const V3i& rvalue);
    V3f operator+(const V3f& lvalue, const V3f& rvalue);
    V3d operator+(const V3d& lvalue, const V3d& rvalue);
    V2i operator-(const V2i& lvalue, const V2i& rvalue);
    V2f operator-(const V2f& lvalue, const V2f& rvalue);
    V2d operator-(const V2d& lvalue, const V2d& rvalue);
    V3i operator-(const V3i& lvalue, const V3i& rvalue);
    V3f operator-(const V3f& lvalue, const V3f& rvalue);
    V3d operator-(const V3d& lvalue, const V3d& rvalue);
    V2i operator*(const V2i& lvalue, const V2i& rvalue);
    V2f operator*(const V2f& lvalue, const V2f& rvalue);
    V2d operator*(const V2d& lvalue, const V2d& rvalue);
    V3i operator*(const V3i& lvalue, const V3i& rvalue);
    V3f operator*(const V3f& lvalue, const V3f& rvalue);
    V3d operator*(const V3d& lvalue, const V3d& rvalue);
    V2i operator/(const V2i& lvalue, const V2i& rvalue);
    V2f operator/(const V2f& lvalue, const V2f& rvalue);
    V2d operator/(const V2d& lvalue, const V2d& rvalue);
    V3i operator/(const V3i& lvalue, const V3i& rvalue);
    V3f operator/(const V3f& lvalue, const V3f& rvalue);
    V3d operator/(const V3d& lvalue, const V3d& rvalue);
    bool operator==(const V2i& lvalue, const V2i& rvalue);
    bool operator==(const V2f& lvalue, const V2f& rvalue);
    bool operator==(const V2d& lvalue, const V2d& rvalue);
    bool operator==(const V3i& lvalue, const V3i& rvalue);
    bool operator==(const V3f& lvalue, const V3f& rvalue);
    bool operator==(const V3d& lvalue, const V3d& rvalue);

    float trianglewave(float i);
    float squarer(float angle, float sidelen);
    float distance(V2f pos1, V2f pos2);
    V2f carteToPolar(V2f carte);
    V2f polarToCarte(V2f polar);
    bool pointInSquare(V2i pos, V2iPair sqr);
    bool pointInSquare(V2f pos, V2fPair sqr);
    bool squareInSquare(V2iPair sqr1, V2iPair sqr2);
    bool squareInSquare(V2fPair sqr1, V2fPair sqr2);
    bool fpil(V2fPair line, V2f point); //float point in line
    bool fpilu(V2fPair line, V2f point, float unprec); //float point in line with unprecision
    V2f fatp(V2f p1, V2f p2); //float angle to point
}

#endif
