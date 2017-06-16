/*
 * language.h C-like Interpreted Programming Language
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
#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <stdlib.h>
#include <string.h>
#include <vector>

class Operator;
class Variable;
class Expression;
class Scope;
struct type_t {
    const char* id;
    size_t size;
    bool number;
};
class Variable {
    public:
        Scope* scope;
        type_t type;
        const char* id;
        void* data;
        bool temp;
        Variable* cache;
        Variable(type_t type, const char* id, void* data);
        Variable(type_t type, const char* id);
        virtual ~Variable();
        virtual Variable* get();
        virtual void clearCache();
};
class Operator : public Variable {
    public:
        bool postfix;
        int priority;
        type_t lvalue, rvalue;
        bool uselvalue, uservalue;
        Variable* (*evalf)(Variable* lvalue, Variable* rvalue);
        Operator(const char* id, int priority, type_t lvalue, type_t rvalue, bool uselvalue, bool uservalue, Variable* (*evalf)(Variable* lvalue, Variable* rvalue));
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
class Scope {
    public:
        std::vector<type_t> types;
        std::vector<Operator*> ops;
        std::vector<Variable*> vars;
        Scope();
        virtual ~Scope();
        virtual void add(Variable* var);
        virtual Variable* parse(const char** tokens, size_t size);
        virtual void execute(const char* text);
};

extern type_t voidtype;
extern type_t inttype;
extern type_t pointertype;

#endif
