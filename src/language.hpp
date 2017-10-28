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
#include "utils.hpp"

//TODO: retype struct type to class
//TODO: divide this file into smaller files

namespace CIPL {
    class Scope;
    class Operator;
    class Variable;
    class Expression;
    class Return;
    struct type { //TODO: floating point numbers and make every operator don't consider every variable an int //TODO: add variable id type
        const char* id;
        char* opts; //TODO: fix this
        int size;
        int mantissa;
        int type; //0-everything else, 1-number, 2-anything, 3-return
    };
    class Scope {
        public:
            Scope* scope;
            utils::Vector<struct type> types;
            utils::Vector<Operator*> ops;
            utils::Vector<Variable*> vars;
            Scope();
            virtual ~Scope();
            virtual void addType(struct type type);
            virtual void addOp(Operator* op);
            virtual void addVar(Variable* var);
            virtual Variable* getVar(const char* id);
            virtual Variable* parse(const char** tokens, int size);
            virtual Variable* execute(const char* text); //TODO: change from cout to cerr
    };
    class Variable {
        public:
            Scope* scope;
            struct type type;
            const char* id;
            void* data;
            Variable* cache;
            Variable(struct type type, const char* id, void* data);
            Variable(struct type type, const char* id);
            virtual ~Variable();
            virtual Variable* get();
            virtual void clearCache();
    };
    class Operator : public Variable {
        public:
            int priority;
            struct type lvalue, rvalue;
            bool uselvalue, uservalue;
            bool suffix;
            Variable* (*evalf)(Variable* lvalue, Variable* rvalue);
            Operator(const char* id, int priority, struct type lvalue, struct type rvalue, bool uselvalue, bool uservalue, bool suffix, Variable* (*evalf)(Variable* lvalue, Variable* rvalue));
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

    extern struct type returntype;
    extern struct type anytype;
    extern struct type voidtype;
    extern struct type inttype;
}

#endif
