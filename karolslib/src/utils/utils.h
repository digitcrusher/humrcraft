/*
 * utils.h
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
#ifndef KAROLSLIB_UTILS_H
#define KAROLSLIB_UTILS_H
#include <stdlib.h>
#include <string.h>

#if defined(__linux__)
long KL_getMS();
long KL_getNS();
int getch();
#elif defined(_WIN32)
long KL_getMS();
long KL_getNS();
void hidecursor();
#endif

int indexof(const char* string, char search); //Search string for search
size_t intlen(int x); //How many digits in x?
size_t uintlen(unsigned int x); //How many digits in x?
size_t floatlen(float x); //How many digits in x?
bool stof(char* str, float* ret); //Convert str to a float
bool stoi(char* str, int* ret); //Convert str to an int
bool stoui(char* str, unsigned int* ret); //Convert str to an unsigned int
char* itos(int x); //Convert int to string
char* uitos(unsigned int x); //Convert unsigned int to string
char* ftos(float x); //Convert float to string
int htoi(char* hex); //Convert hex string to int
//char* itos(int input, char* msg); //Convert int to string and concatenate msg to the end !!!!!NOT USED!!!!!

template<typename T> class KL_Vector {
    private:
        size_t vsize;
        size_t vcapacity;
        const size_t maxsize=2147483647;
        T* array;
    public:
        KL_Vector();
        ~KL_Vector();
        virtual bool pushBack(T t);
        virtual bool popBack(T* t);
        virtual bool replace(unsigned int element, T t);
        virtual bool resize(size_t newsize);
        virtual T* getP(unsigned int element);
        virtual bool clear();
        virtual T* getArray();
        virtual size_t size();
        virtual size_t capacity();
        virtual T& operator[](unsigned int n);
        virtual KL_Vector<T>& operator=(const KL_Vector<T>& rvalue);
};
template<typename T> KL_Vector<T>::KL_Vector() {
    this->vsize=0;
    this->vcapacity=this->vsize+1;
    this->array=(T*)malloc(sizeof(T)*this->vcapacity);
}
template<typename T> KL_Vector<T>::~KL_Vector() {
    free(this->array);
}
template<typename T> bool KL_Vector<T>::pushBack(T t) {
    if(this->vsize >= this->vcapacity) {
        if(this->resize(this->vsize+1)) {
            return 1;
        }
    }
    this->array[this->vsize++] = t;
    return 0;
}
template<typename T> bool KL_Vector<T>::popBack(T* t) {
    *t = this->array[--this->vsize];
    return this->resize(this->vsize+1);
}
template<typename T> bool KL_Vector<T>::replace(unsigned int element, T t) {
    if(element>this->vsize-1)
        return 1;
    this->array[element] = t;
    return 0;
}
template<typename T> bool KL_Vector<T>::resize(size_t newsize) {
    if(newsize > this->maxsize)
        return 1;
    this->vcapacity = newsize;
    this->array = (T*)realloc((void*)this->array, sizeof(T)*this->vcapacity);
    return 0;
}
template<typename T> T* KL_Vector<T>::getP(unsigned int element) {
    if(element > this->vsize-1)
        return NULL;
    return this->array+element;
}
template<typename T> bool KL_Vector<T>::clear() {
    this->vsize=0;
    this->vcapacity=this->vsize+1;
    this->resize(this->vcapacity);
    return 0;
}
template<typename T> T* KL_Vector<T>::getArray() {
    return this->array;
}
template<typename T> size_t KL_Vector<T>::size() {
    return this->vsize;
}
template<typename T> size_t KL_Vector<T>::capacity() {
    return this->vcapacity;
}
template<typename T> T& KL_Vector<T>::operator[](unsigned int n) {
    return this->array[n];
}
template<typename T> KL_Vector<T>& KL_Vector<T>::operator=(const KL_Vector<T>& rvalue) {
    this->vsize = rvalue.vsize;
    this->vcapacity = rvalue.vcapacity;
    free(this->array);
    this->array = (T*)malloc(sizeof(T)*this->vcapacity);
    memcpy(this->array, rvalue.array, sizeof(T)*this->vsize);
    return *this;
}

#endif
