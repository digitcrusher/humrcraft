/*
 * language.cpp
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
#include <iostream>
#include <vector>
#include "language.h"

Variable::Variable(type_t type, const char* id) {
    this->type = type;
    this->id = id;
    this->data = malloc(this->type.size);
}
Variable::Variable(type_t type, const char* id, void* data) {
    this->type = type;
    this->id = id;
    this->data = data;
}
Variable::~Variable() {
    free(this->data);
}
Variable* Variable::get() {
    return this;
}

Operator::Operator(const char* id, type_t* lvalue, type_t* rvalue, Variable* (*evalf)(Variable* lvalue, Variable* rvalue)) : Variable({"operator", sizeof(Operator)}, id) {
    this->lvalue = lvalue;
    this->rvalue = rvalue;
    this->evalf = evalf;
}
Operator::~Operator() {
    this->~Variable();
}
Variable* Operator::eval(Variable* lvalue, Variable* rvalue) {
    if(this->evalf) {
        return this->evalf(lvalue, rvalue);
    }
    return NULL;
}

Expression::Expression(Variable* lvalue, Operator* op, Variable* rvalue) : Variable({"expression", sizeof(Expression)}, "") {
    this->lvalue = lvalue;
    this->op = op;
    this->rvalue = rvalue;
}
Expression::~Expression() {
    this->~Variable();
    delete this->lvalue;
    delete this->rvalue;
}
Variable* Expression::get() {
    return this->op->eval(this->lvalue->get(), this->rvalue->get());
}

type_t types[] = {{"int", sizeof(int)}};
Operator* ops[] = {new Operator("+", &types[0], &types[0], [](Variable* lvalue, Variable* rvalue) {
    if(lvalue && rvalue) {
        if(!strcmp(lvalue->type.id, types[0].id) && !strcmp(rvalue->type.id, types[0].id) && lvalue->type.size == sizeof(int) && rvalue->type.size == sizeof(int)) {
            int* result = (int*)malloc(sizeof(int));
            *result =  *(int*)lvalue->data + *(int*)rvalue->data;
            Variable* var = new Variable(types[0], "", result);
            return var;
        }
    }
    return (Variable*)NULL;
}), new Operator("-", &types[0], &types[0], [](Variable* lvalue, Variable* rvalue) {
    if(lvalue && rvalue) {
        if(!strcmp(lvalue->type.id, types[0].id) && !strcmp(rvalue->type.id, types[0].id) && lvalue->type.size == sizeof(int) && rvalue->type.size == sizeof(int)) {
            int* result = (int*)malloc(sizeof(int));
            *result =  *(int*)lvalue->data - *(int*)rvalue->data;
            Variable* var = new Variable(types[0], "", result);
            return var;
        }
    }
    return (Variable*)NULL;
}), new Operator("*", &types[0], &types[0], [](Variable* lvalue, Variable* rvalue) {
    if(lvalue && rvalue) {
        if(!strcmp(lvalue->type.id, types[0].id) && !strcmp(rvalue->type.id, types[0].id) && lvalue->type.size == sizeof(int) && rvalue->type.size == sizeof(int)) {
            int* result = (int*)malloc(sizeof(int));
            *result =  *(int*)lvalue->data * *(int*)rvalue->data;
            Variable* var = new Variable(types[0], "", result);
            return var;
        }
    }
    return (Variable*)NULL;
}), new Operator("/", &types[0], &types[0], [](Variable* lvalue, Variable* rvalue) {
    if(lvalue && rvalue) {
        if(!strcmp(lvalue->type.id, types[0].id) && !strcmp(rvalue->type.id, types[0].id) && lvalue->type.size == sizeof(int) && rvalue->type.size == sizeof(int)) {
            int* result = (int*)malloc(sizeof(int));
            *result =  *(int*)lvalue->data / *(int*)rvalue->data;
            Variable* var = new Variable(types[0], "", result);
            return var;
        }
    }
    return (Variable*)NULL;
})};
void execute(const char* text) {
    //0 - space
    //1 - op
    //2 - num
    //3 - text
    //4 - eos
    //5 - po
    //6 - pe
    auto checkGroup = [](char c) {
        if(c == 0 || c == 10 || c == 11 || c == 13 || c == 32) {
            return 0;
        }else if(c == 59) {
            return 4;
        }else if(c == 40) {
            return 5;
        }else if(c == 41) {
            return 6;
        }else if((c>=33 && c<=47) || (c>=58 && c<=64) || (c>=91 && c<=96) || (c>=123 && c<=126)) {
            return 1;
        }else if(c>=48 && c<=57) {
            return 2;
        }else if((c>=65 && c<=90) || (c>=97 && c<=122)) {
            return 3;
        }
        return -1;
    };
    std::vector<std::string*> tokens;
    int group=-1;
    for(unsigned int i=0, j = -1; i<strlen(text); i++) {
        if(group != checkGroup(text[i]) && !(group == 3 && checkGroup(text[i]) == 2)) {
            if(checkGroup(text[i]) == 4) {
                break;
            }
            if(checkGroup(text[i]) != 0) {
                tokens.push_back(new std::string());
                j++;
            }
            group = checkGroup(text[i]);
        }
        if(group != 0) {
            tokens[j]->push_back(text[i]);
        }
    }
    group = -1;
    for(unsigned int i=0; i<tokens.size(); i++) {
        std::cout<<*tokens[i]<<'\n';
    }
    Variable* var1 = new Variable(types[0], "");
    Variable* var2 = new Variable(types[0], "");
    *(int*)var1->data = 5;
    *(int*)var2->data = 5;
    Expression* expr = new Expression(var1, ops[0], var2);
    std::cout<<*(int*)expr->get()->data<<'\n';
}
