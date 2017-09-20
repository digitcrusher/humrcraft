/*
 * utils.hpp
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
#ifndef UTILS_HPP
#define UTILS_HPP
#include <stdlib.h>
#include <string.h>

//TODO: add a sorting algorithm

#if defined(__linux__)
long getMS();
long getNS();
int getch();
#elif defined(_WIN32)
long getMS();
long getNS();
void hidecursor();
#endif

int indexof(const char* string, char search); //Search string for search
size_t intlen(int x); //How many digits in x?
size_t uintlen(unsigned int x); //How many digits in x?
size_t floatlen(float x); //How many digits in x?
bool stof(const char* str, float* ret); //Convert str to a float
bool stoi(const char* str, int* ret); //Convert str to an int
bool stoui(const char* str, unsigned int* ret); //Convert str to an unsigned int
char* itos(int x); //Convert int to string
char* uitos(unsigned int x); //Convert unsigned int to string
char* ftos(float x); //Convert float to string
int htoi(const char* hex); //Convert hex string to int
//char* itos(int input, char* msg); //Convert int to string and concatenate msg to the end !!!!!NOT USED!!!!!
char* encode(const char* str, unsigned int size, int pass);
char* decode(const char* str, unsigned int size, int pass);

#include <iostream>

template<typename T> class Vector {
    private:
        size_t vsize;
        size_t vcapacity;
        const size_t maxsize=(size_t)-1;
        T* array;
    public:
        Vector();
        ~Vector();
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
        virtual Vector<T>& operator=(const Vector<T>& rvalue);
};
template<typename T> Vector<T>::Vector() {
    this->vsize=0;
    this->vcapacity=this->vsize+1;
    this->array=(T*)malloc(sizeof(T)*this->vcapacity);
}
template<typename T> Vector<T>::~Vector() {
    free(this->array);
}
template<typename T> bool Vector<T>::pushBack(T t) {
    if(this->vsize >= this->vcapacity) {
        if(this->resize(this->vsize+1)) {
            return 1;
        }
    }
    this->array[this->vsize++] = t;
    return 0;
}
template<typename T> bool Vector<T>::popBack(T* t) {
    *t = this->array[--this->vsize];
    return this->resize(this->vsize+1);
}
template<typename T> bool Vector<T>::replace(unsigned int element, T t) {
    if(element>this->vsize-1)
        return 1;
    this->array[element] = t;
    return 0;
}
template<typename T> bool Vector<T>::resize(size_t newsize) {
    if(newsize > this->maxsize)
        return 1;
    this->vcapacity = newsize;
    this->array = (T*)realloc(this->array, sizeof(T)*this->vcapacity);
    return 0;
}
template<typename T> T* Vector<T>::getP(unsigned int element) {
    if(element > this->vsize-1)
        return NULL;
    return this->array+element;
}
template<typename T> bool Vector<T>::clear() {
    this->vsize=0;
    this->vcapacity=this->vsize+1;
    this->resize(this->vcapacity);
    return 0;
}
template<typename T> T* Vector<T>::getArray() {
    return this->array;
}
template<typename T> size_t Vector<T>::size() {
    return this->vsize;
}
template<typename T> size_t Vector<T>::capacity() {
    return this->vcapacity;
}
template<typename T> T& Vector<T>::operator[](unsigned int n) {
    return this->array[n];
}
template<typename T> Vector<T>& Vector<T>::operator=(const Vector<T>& rvalue) {
    this->vsize = rvalue.vsize;
    this->vcapacity = rvalue.vcapacity;
    free(this->array);
    this->array = (T*)malloc(sizeof(T)*this->vcapacity);
    memcpy(this->array, rvalue.array, sizeof(T)*this->vsize);
    return *this;
}

template<typename T> struct ListElem {
    T elem;
    bool free;
};
template<typename T> class List {
    public:
        Vector<ListElem<T>> elems;
        List();
        virtual ~List();
        virtual size_t size();
        virtual size_t capacity();
        virtual ListElem<T>* getArray();
        virtual int add(T elem);
        virtual bool remove(unsigned int x);
        virtual bool isFree(unsigned int x);
        virtual T* get(unsigned int x);
        virtual T operator[](const unsigned int& rvalue);
        virtual List<T>& operator=(const List<T>& rvalue);
};
template<typename T> List<T>::List() {
}
template<typename T> List<T>::~List() {
}
template<typename T> size_t List<T>::size() {
    return this->elems.size();
}
template<typename T> size_t List<T>::capacity() {
    return this->elems.capacity();
}
template<typename T> ListElem<T>* List<T>::getArray() {
    return this->elems.getArray();
}
template<typename T> int List<T>::add(T elem) {
    for(unsigned int i=0; i<this->elems.size(); i++) {
        if(this->elems.getArray()[i].free) {
            this->elems.getArray()[i].free = 0;
            this->elems.getArray()[i].elem = elem;
            return i;
        }
    }
    this->elems.pushBack({elem, 0});
    return this->elems.size()-1;
}
template<typename T> bool List<T>::remove(unsigned int x) {
    if(x >= 0 && x < this->elems.size()) {
        this->elems.getArray()[x].free = 1;
        return 0;
    }
    return 1;
}
template<typename T> bool List<T>::isFree(unsigned int x) {
    if(x >= 0 && x < this->elems.size()) {
        return this->elems.getArray()[x].free;
    }
    return 0;
}
template<typename T> T* List<T>::get(unsigned int x) {
    if(x >= 0 && x < this->elems.size()) {
        return &this->elems.getArray()[x].elem;
    }
    return NULL;
}
template<typename T> T List<T>::operator[](const unsigned int& rvalue) {
    return *this->get(rvalue);
}
template<typename T> List<T>& List<T>::operator=(const List<T>& rvalue) {
    this->elems = rvalue.elems;
    return *this;
}

#endif
