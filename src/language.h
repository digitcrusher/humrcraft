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

class Scope;
class Operator;
class Variable;
class Expression;
class Return;
struct type_t {
    const char* id;
    char* opts;
    size_t size;
    int type; //0-everything else, 1-number, 2-anything, 3-return
};
class Scope {
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
        virtual Variable* execute(const char* text);
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
