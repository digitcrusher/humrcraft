/*
 * utils.cpp
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
#include <math.h>
#include "utils.hpp"

namespace utils {
    void resetTimer(struct timer* timer) {
        timer->starttime = (double)getMS()/1000;
    }
    void pauseTimer(struct timer* timer, bool pause) {
        if(pause) {
            if(!timer->pause) {
                timer->pausetime = (double)getMS()/1000;
            }
        }else {
            timer->starttime = (double)getMS()/1000-(timer->pausetime-timer->starttime);
        }
        timer->pause = pause;
    }
    void setLoopTimer(struct timer* timer, bool loopover, double looplength) {
        timer->loopover = loopover;
        timer->looplength = looplength;
    }
    double getElapsedTimer(struct timer timer) {
        double time = (double)getMS()/1000-timer.starttime;
        if(timer.pause) {
            time = timer.pausetime-timer.starttime;
        }
        return timer.loopover?fmod(time, timer.looplength):time;
    }
    #if defined(__linux__)
    #include <stdio.h>
    #include <termios.h>
    #include <unistd.h>
    #include <time.h>
    long getMS() {
        timespec ts;
        if(clock_gettime(CLOCK_MONOTONIC, &ts)) return 0;
        return ts.tv_sec*1000+ts.tv_nsec/1000000;
    }
    long getNS() {
        timespec ts;
        if(clock_gettime(CLOCK_MONOTONIC, &ts)) return 0;
        return ts.tv_sec*1000000000+ts.tv_nsec;
    }
    int getch() {
      struct termios oldt, newt;
      int ch;
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      ch = getchar();
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      return ch;
    }
    #elif defined(_WIN32)
    #include <windows.h>
    long getMS() { //Get milliseconds
        LARGE_INTEGER f,c; //Union which will store frequency and ticks
        if(!QueryPerformanceFrequency(&f)) return 0; //Try to get processor clock frequency
        if(!QueryPerformanceCounter(&c)) return 0; //Try to get processor clock ticks
        return c.QuadPart/(f.QuadPart/1000); //Return milliseconds (Ticks / (Frequency / msPerSecond))
    }
    long getNS() { //Get nanoseconds
        LARGE_INTEGER f,c; //Union which will store frequency and ticks
        if(!QueryPerformanceFrequency(&f)) return 0; //Try to get processor clock frequency
        if(!QueryPerformanceCounter(&c)) return 0; //Try to get processor clock ticks
        return c.QuadPart/(f.QuadPart/1000000000); //Return nanoseconds (Ticks / (Frequency / nsPerSecond))
    }
    void hidecursor() {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }
    #endif

    unsigned int brr(unsigned int number, unsigned int bits) {
        unsigned int result=number>>bits<<bits;
        unsigned int shift=1<<(bits-1);
        unsigned int shift1=1;
        for(unsigned int i=0; i<bits; i++) {
            if(number&shift) {
                result += shift1;
            }
            shift >>= 1;
            shift1 <<= 1;
        }
        return result;
    }
    unsigned int blr(unsigned int number, unsigned int bits) {
        unsigned int result=number<<bits>>bits;
        unsigned int shift=1<<(sizeof(int)*8-1); //There is a better method
        unsigned int shift1=1<<(sizeof(int)*8-bits);
        for(unsigned int i=0; i<bits; i++) {
            if(number&shift) {
                result += shift1;
            }
            shift >>= 1;
            shift1 <<= 1;
        }
        return result;
    }
    int indexof(const char* string, char search) {
        for(int i=0; i<(int)strlen(string); i++) {
            if(*(string+i) == search) {
                return i;
            }
        }
        return 1;
    }
    int intlen(int x) {
        int i=x;
        int len=1;
        if(i!=0) {
            for(len = 0; i!=0; len++) {
                i = i/10;
            }
        }
        return len+(x<0);
    }
    int uintlen(unsigned int x) {
        int i=x;
        int len=1;
        if(i!=0) {
            for(len = 0; i!=0; len++) {
                i = i/10;
            }
        }
        return len;
    }
    int floatlen(float x) {
        float i=x;
        int len=1;
        if((int)i!=0) {
            for(len = 0; (int)i!=0; len++) {
                i = i/10;
            }
        }
        i = x;
        if(i!=0) {
            for(; (int)i!=i; len++) {
                i = i*10;
            }
        }
        return len+(x<0)+((int)x!=x);
    }
    bool stof(const char* str, float* ret) {
        int len=strlen(str);
        bool neg=0;
        bool floatpoint=0;
        float num=0;
        char* whole=(char*)malloc(sizeof(char)*len);
        int wholelen=0;
        char* point=(char*)malloc(sizeof(char)*len);
        int floatlen=0;
        for(int i=0; i<len; i++) {
            if(i == 0 && *(str+i) == '-') {
                neg = 1;
            }else if(*(str+i) == '.' && !floatpoint) {
                floatpoint = 1;
            }else if(*(str+i)-'0' >= 0 && *(str+i)-'0' <= 9) {
                if(floatpoint) {
                    *(point+floatlen++) = *(str+i)-'0';
                }else {
                    *(whole+wholelen++) = *(str+i)-'0';
                }
            }else {
                return 1;
            }
        }
        for(int i=0; i<wholelen; i++) {
            if(i>=1) {
                num += pow(10,i)**(whole+wholelen-1-i);
            }else {
                num += *(whole+wholelen-1-i);
            }
        }
        for(int i=0; i<floatlen; i++) {
            num += *(point+i)/pow(10,i+1);
        }
        if(neg) {
            num = -(num);
        }
        free(whole);
        free(point);
        *ret = num;
        return 0;
    }
    bool stoi(const char* str, int* ret) {
        int len=strlen(str);
        bool neg=0;
        int num=0;
        char* whole=(char*)malloc(sizeof(char)*len);
        int wholelen=0;
        for(int i=0; i<len; i++) {
            if(i == 0 && *(str+i) == '-') {
                neg = 1;
            }else if(*(str+i)-'0' >= 0 && *(str+i)-'0' <= 9) {
                *(whole+wholelen++) = *(str+i)-'0';
            }else {
                return 1;
            }
        }
        for(int i=0; i<wholelen; i++) {
            if(i>0) {
                num += pow(10,i)**(whole+wholelen-1-i);
            }else {
                num += *(whole+wholelen-1-i);
            }
        }
        if(neg) {
            num = -(num);
        }
        free(whole);
        *ret = num;
        return 0;
    }
    bool stoui(const char* str, unsigned int* ret) {
        int len=strlen(str);
        unsigned int num=0;
        char* whole=(char*)malloc(sizeof(char)*len);
        int wholelen=0;
        for(int i=0; i<len; i++) {
            if(*(str+i)-'0' >= 0 && *(str+i)-'0' <= 9) {
                *(whole+wholelen++) = *(str+i)-'0';
            }else {
                return 1;
            }
        }
        for(int i=0; i<wholelen; i++) {
            if(i>0) {
                num += pow(10,i)**(whole+wholelen-1-i);
            }else {
                num += *(whole+wholelen-1-i);
            }
        }
        free(whole);
        *ret = num;
        return 0;
    }
    char* itos(int x) {
        int len=intlen(x);
        char* str=(char*)malloc(sizeof(char)*(len+1));
        int num=x;
        bool neg=0;
        int ptr=0;
        if(x<0) {
            neg = 1;
            num = 0-x;
            *(str+ptr++) = '-';
        }
        for(int i=0; i<len-neg; i++, ptr++) {
            *(str+len-1-i) = num%10+'0';
            num = num/10;
        }
        *(str+len) = '\0';
        return str;
    }
    char* uitos(unsigned int x) {
        int len=uintlen(x);
        char* str=(char*)malloc(sizeof(char)*(len+1));
        unsigned int num=x;
        int ptr=0;
        for(int i=0; i<len; i++, ptr++) {
            *(str+len-1-i) = num%10+'0';
            num = num/10;
        }
        *(str+len) = '\0';
        return str;
    }
    char* ftos(float x) {
        int len=floatlen(x);
        char* str=(char*)malloc(sizeof(char)*(len+1));
        float num=x;
        bool neg=0;
        bool exponent=0;
        int ptr=0;
        if(x<0) {
            neg = 1;
            num = 0-x;
            *(str+ptr++) = '-';
        }
        if((int)x!=x) {
            exponent = 1;
        }
        len = intlen((int)x);
        for(int i=0; i<len-neg; i++, ptr++) {
            *(str+len-1-i) = (int)num%10+'0';
            num = num/10;
        }
        num = neg ? 0-x : x;
        if(exponent) {
            *(str+ptr++) = '.';
            for(; (int)num!=num; ptr++) {
                num = num*10;
                *(str+ptr) = (int)num%10+'0';
            }
        }
        len = floatlen(x);
        *(str+len) = '\0';
        return str;
    }
    int htoi(const char* hex) {
        int dec=0;
        for(int i=strlen(hex); i>0; i--) {
            int num=0;
            switch(hex[i-1]) {
                case'0':
                    num=0;
                break;
                case'1':
                    num=1;
                break;
                case'2':
                    num=2;
                break;
                case'3':
                    num=3;
                break;
                case'4':
                    num=4;
                break;
                case'5':
                    num=5;
                break;
                case'6':
                    num=6;
                break;
                case'7':
                    num=7;
                break;
                case'8':
                    num=8;
                break;
                case'9':
                    num=9;
                break;
                case'A':
                    num=10;
                break;
                case'a':
                    num=10;
                break;
                case'B':
                    num=11;
                break;
                case'b':
                    num=11;
                break;
                case'C':
                    num=12;
                break;
                case'c':
                    num=12;
                break;
                case'D':
                    num=13;
                break;
                case'd':
                    num=13;
                break;
                case'E':
                    num=14;
                break;
                case'e':
                    num=14;
                break;
                case'F':
                    num=15;
                break;
                case'f':
                    num=15;
                break;
                default:
                    num=0;
                break;
            }
            if(!strlen(hex)-i==0) {
                dec+=pow(16,i)*num;
            }else {
                dec+=num;
            }
        }
        return dec;
    }
    /*char* itos(int input, char* msg) {
        int len = intlen(input);
        char* result = (char*)malloc(sizeof(char)*(len+strlen(msg)));
        int sub = 0;
        for(int i=0;i<len;i++) {
            int current = floor((input-sub)/pow(10,(len-i-1)));
            *(result+i) = current + '0';
            sub += pow(10,(len-i-1))*current;
        }
        strcpy(result+len, msg);
        return result;
    }*/
    char* encode(const char* str, unsigned int size, int pass) {
        char* result=(char*)malloc(sizeof(char)*size+1);
        for(unsigned int i=0; i<size; i++) {
            result[i] = (float)i/2==(int)i/2 ? str[i]+(i*pass) : str[i]-(i*pass);
        }
        result[size] = '\0';
        return result;
    }
    char* decode(const char* str, unsigned int size, int pass) {
        char* result=(char*)malloc(sizeof(char)*size+1);
        for(unsigned int i=0; i<size; i++) {
            result[i] = (float)i/2==(int)i/2 ? str[i]-(i*pass) : str[i]+(i*pass);
        }
        result[size] = '\0';
        return result;
    }
}
