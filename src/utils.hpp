/*
 * utils.hpp
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
#ifndef UTILS_HPP
#define UTILS_HPP
#include <stdlib.h>
#include <string.h>
#include <exception>

//TODO: add a sorting algorithm

namespace utils {
    struct timer {
        double starttime;
        double pausetime;
        double looplength;
        bool loopover;
        bool pause;
    };
    void resetTimer(struct timer* timer);
    void pauseTimer(struct timer* timer, bool pause);
    void setLoopTimer(struct timer* timer, bool loopover, double looplength);
    double getElapsedTimer(struct timer timer);

    template<typename T> class Vector { //TODO: add more functions
        private:
            int vsize;
            int vcapacity;
            const int maxsize=(unsigned int)-1>>1;
            T* array;
        public:
            Vector();
            virtual ~Vector();
            virtual void pushBack(T t);
            virtual void popBack(T* t);
            virtual void replace(int element, T t);
            virtual void resize(int newsize);
            virtual T* get(int element);
            virtual void clear();
            virtual T* getArray();
            virtual int size();
            virtual int capacity();
            virtual T operator[](int n);
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
    template<typename T> void Vector<T>::pushBack(T t) {
        if(this->vsize >= this->vcapacity) {
            try {
                this->resize(this->vsize+1);
            }catch(std::exception e) {
                throw e;
            }
        }
        this->array[this->vsize++] = t;
    }
    template<typename T> void Vector<T>::popBack(T* t) {
        *t = this->array[--this->vsize];
        try {
            this->resize(this->vsize+1);
        }catch(std::exception e) {
            throw e;
        }
    }
    template<typename T> void Vector<T>::replace(int element, T t) {
        if(element > this->vsize-1) {
            throw "Invalid element";
        }
        this->array[element] = t;
    }
    template<typename T> void Vector<T>::resize(int newsize) {
        if(newsize > this->maxsize) {
            throw "Invalid size";
        }
        this->vcapacity = newsize;
        this->array = (T*)realloc(this->array, sizeof(T)*this->vcapacity);
    }
    template<typename T> T* Vector<T>::get(int element) {
        if(element > this->vsize-1) {
            throw "Invalid element";
        }
        return this->array+element;
    }
    template<typename T> void Vector<T>::clear() {
        this->vsize=0;
        this->vcapacity=this->vsize+1;
        try {
            this->resize(this->vcapacity);
        }catch(std::exception e) {
            throw e;
        }
    }
    template<typename T> T* Vector<T>::getArray() {
        return this->array;
    }
    template<typename T> int Vector<T>::size() {
        return this->vsize;
    }
    template<typename T> int Vector<T>::capacity() {
        return this->vcapacity;
    }
    template<typename T> T Vector<T>::operator[](int n) {
        try {
            return *this->get(n);
        }catch(std::exception e) {
            throw e;
        }
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
            Vector<struct ListElem<T>> elems;
            List();
            virtual ~List();
            virtual int size();
            virtual int capacity();
            virtual struct ListElem<T>* getArray();
            virtual int add(T elem);
            virtual void remove(int x);
            virtual bool isFree(int x);
            virtual T* get(int x);
            virtual T operator[](int rvalue);
            virtual List<T>& operator=(const List<T>& rvalue);
    };
    template<typename T> List<T>::List() {
    }
    template<typename T> List<T>::~List() {
    }
    template<typename T> int List<T>::size() {
        return this->elems.size();
    }
    template<typename T> int List<T>::capacity() {
        return this->elems.capacity();
    }
    template<typename T> struct ListElem<T>* List<T>::getArray() {
        return this->elems.getArray();
    }
    template<typename T> int List<T>::add(T elem) {
        for(int i=0; i<this->elems.size(); i++) {
            if(this->elems.getArray()[i].free) {
                this->elems.getArray()[i].free = 0;
                this->elems.getArray()[i].elem = elem;
                return i;
            }
        }
        try {
            this->elems.pushBack({elem, 0});
        }catch(std::exception e) {
            throw e;
        }
        return this->elems.size()-1;
    }
    template<typename T> void List<T>::remove(int x) {
        if(x >= 0 && x < this->elems.size()) {
            this->elems.getArray()[x].free = 1;
        }
        throw;
    }
    template<typename T> bool List<T>::isFree(int x) {
        if(x >= 0 && x < this->elems.size()) {
            return this->elems.getArray()[x].free;
        }
        return 0;
    }
    template<typename T> T* List<T>::get(int x) {
        if(x >= 0 && x < this->elems.size()) {
            return &this->elems.getArray()[x].elem;
        }
        throw;
    }
    template<typename T> T List<T>::operator[](int rvalue) {
        return *this->get(rvalue);
    }
    template<typename T> List<T>& List<T>::operator=(const List<T>& rvalue) {
        this->elems = rvalue.elems;
        return *this;
    }

    #if defined(__linux__)
    long getMS();
    long getNS();
    int getch();
    #elif defined(_WIN32)
    long getMS();
    long getNS();
    void hidecursor();
    #endif
    unsigned int brr(unsigned int number, unsigned int bits); //Bitwise Right Rotate
    unsigned int blr(unsigned int number, unsigned int bits); //Bitwise Left Rotate
    int indexof(const char* string, char search); //Search string for search
    int intlen(int x); //How many digits in x?
    int uintlen(unsigned int x); //How many digits in x?
    int floatlen(float x); //How many digits in x?
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
}

#endif
