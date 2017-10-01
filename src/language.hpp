/*
 * language.hpp C-like Interpreted Programming Language
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
#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP
#include <vector>

class Scope;
class Operator;
class Variable;
class Expression;
class Return;
struct type_t { //TODO: floating point numbers and make every operator don't consider that every variable is an int
    const char* id;
    char* opts; //TODO: fix this
    size_t size;
    int type; //0-everything else, 1-number, 2-anything, 3-return
};
class Scope { //TODO: change std::vector to Vector
    public:
        Scope* scope;
        std::vector<type_t> types;
        std::vector<Operator*> ops;
        std::vector<Variable*> vars;
        Scope();
        virtual ~Scope();
        virtual void addType(type_t type);
        virtual void addOp(Operator* op);
        virtual void addVar(Variable* var);
        virtual Variable* getVar(const char* id);
        virtual Variable* parse(const char** tokens, size_t size);
        virtual Variable* execute(const char* text); //TODO: change from cout to cerr
};
class Variable {
    public:
        Scope* scope;
        type_t type;
        const char* id;
        void* data;
        Variable* cache;
        Variable(type_t type, const char* id, void* data);
        Variable(type_t type, const char* id);
        virtual ~Variable();
        virtual Variable* get();
        virtual void clearCache();
};
class Operator : public Variable {
    public:
        int priority;
        type_t lvalue, rvalue;
        bool uselvalue, uservalue;
        bool suffix;
        Variable* (*evalf)(Variable* lvalue, Variable* rvalue);
        Operator(const char* id, int priority, type_t lvalue, type_t rvalue, bool uselvalue, bool uservalue, bool suffix, Variable* (*evalf)(Variable* lvalue, Variable* rvalue));
        virtual ~Operator();
        virtual Variable* eval(Variable* lvalue, Variable* rvalue);
};
class Expression : public Variable {
    public:
        Variable* lvalue;
        Operator* op;
        Variable* rvalue;
        Expression(Variable* lvalue, Operator* op, Variable* rvalue);
        virtual ~Expression();
        virtual Variable* get();
};
class Return : public Variable {
    public:
        Return(Variable* var);
        virtual ~Return();
        virtual Variable* get();
};

extern type_t returntype;
extern type_t anytype;
extern type_t voidtype;
extern type_t inttype;

#endif
