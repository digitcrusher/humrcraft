/*
 * language.cpp C-like Interpreted Programming Language
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
#include <utils/utils.h>
#include "language.h"

type_t voidtype = {"void", sizeof(void), 0};
type_t inttype = {"int", sizeof(int), 1};
type_t pointertype = {"pointer", sizeof(void*), 1};

Variable::Variable(type_t type, const char* id, void* data) {
    this->type = type;
    this->id = id;
    this->data = data;
    this->cache = NULL;
}
Variable::Variable(type_t type, const char* id) : Variable(type, id, malloc(type.size)) {
}
Variable::~Variable() {
    free(this->data);
}
Variable* Variable::get() {
    if(!this->cache) {
        this->cache = this;
    }
    return this->cache;
}
void Variable::clearCache() {
    this->cache = NULL;
}

Operator::Operator(const char* id, int priority, type_t lvalue, type_t rvalue, bool uselvalue, bool uservalue
                  ,Variable* (*evalf)(Variable* lvalue, Variable* rvalue)) : Variable({"operator", sizeof(Operator), 0}, id) {
    this->postfix = 0;
    this->priority = priority;
    this->lvalue = lvalue;
    this->rvalue = rvalue;
    this->uselvalue = uselvalue;
    this->uservalue = uservalue;
    this->evalf = evalf;
}
Operator::~Operator() {
}
Variable* Operator::eval(Variable* lvalue, Variable* rvalue) {
    if(this->evalf) {
        return this->evalf(lvalue->get(), rvalue->get());
    }
    return NULL;
}

Expression::Expression(Variable* lvalue, Operator* op, Variable* rvalue) : Variable({"expression", sizeof(Expression), 0}, "") {
    this->lvalue = lvalue;
    this->op = op;
    this->rvalue = rvalue;
}
Expression::~Expression() {
    if(this->op && this->op->postfix) {
        this->cache = this->op->eval(this->lvalue->get(), this->rvalue->get());
    }
    if(this->lvalue && !this->lvalue->scope) {
        delete this->lvalue;
    }
    if(this->rvalue && !this->rvalue->scope) {
        delete this->rvalue;
    }
    if(this->cache && !this->cache->scope) {
        delete this->cache;
    }
}
Variable* Expression::get() {
    if(this->op && !this->cache) {
        if(!this->op->postfix) {
            this->cache = this->op->eval(this->lvalue->get(), this->rvalue->get());
        }else {
            if(this->op->uselvalue) {
                this->cache = this->lvalue->get();
            }else {
                this->cache = this->rvalue->get();
            }
        }
    }
    return this->cache;
}

bool typecmp(type_t type, type_t type2) {
    return (!strcmp(type.id, type2.id) && type.size == type2.size) || (type.number && type2.number);
}
bool varcmp(Variable* var, type_t type) {
    return typecmp(var->type, type);
}
int checkGroup(char c) {
    /* 0 - space
     * 1 - op
     * 2 - num
     * 3 - text
     * 4 - eos
     * 5 - opening paranthesis
     * 6 - closing paranthesis
     */
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
}
Scope::Scope() {
    this->types.push_back(voidtype);
    this->types.push_back(inttype);
    this->types.push_back(pointertype);
    this->ops.push_back(new Operator("=", 0, inttype, inttype, 1, 1, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue, inttype) && varcmp(rvalue, inttype)) {
                *(int*)lvalue->get()->data = *(int*)rvalue->get()->data;
                return lvalue;
            }
        }
        return lvalue;
    }));
    this->ops.push_back(new Operator("+", 1, inttype, inttype, 1, 1, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue, inttype) && varcmp(rvalue, inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data + *(int*)rvalue->get()->data;
                Variable* var = new Variable(inttype, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->ops.push_back(new Operator("-", 1, inttype, inttype, 1, 1, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue, inttype) && varcmp(rvalue, inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data - *(int*)rvalue->get()->data;
                Variable* var = new Variable(inttype, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->ops.push_back(new Operator("*", 2, inttype, inttype, 1, 1, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue, inttype) && varcmp(rvalue, inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data * *(int*)rvalue->get()->data;
                Variable* var = new Variable(inttype, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->ops.push_back(new Operator("/", 2, inttype, inttype, 1, 1, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue, inttype) && varcmp(rvalue, inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data / *(int*)rvalue->get()->data;
                Variable* var = new Variable(inttype, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
}
Scope::~Scope() {
}
void Scope::add(Variable* var) {
    this->vars.push_back(var);
    var->scope = this;
}
Variable* Scope::parse(const char** tokens, size_t size) {
    Variable* lvalue = NULL;
    Operator* op = NULL;
    Variable* rvalue = NULL;
    for(unsigned int i=0; i<size; i++) {
        int group = checkGroup(tokens[i][0]);
        if(group != 1) {
            if(lvalue) {
                std::cout<<"expected an operator\n";
                return NULL;
            }
            switch(group) {
                case 2:
                    lvalue = new Variable(inttype, "");
                    if(stoi(tokens[i], (int*)lvalue->data)) {
                        std::cout<<"interpreter error\n";
                        return NULL;
                    }
                    break;
                case 3: {
                    type_t* type = NULL;
                    for(unsigned int j=0; j<this->types.size(); j++) {
                        if(!strcmp(this->types[j].id, tokens[i])) {
                            type = &this->types[j];
                            break;
                        }
                    }
                    if(type) {
                        for(i++; i < size; i++) {
                            if(checkGroup(tokens[i][0]) == 3) {
                                break;
                            }
                        }
                        for(unsigned int j=0; j<this->vars.size(); j++) {
                            if(!strcmp(this->vars[j]->id, tokens[i])) {
                                std::cout<<"redefinition of variable "<<this->vars[j]->id<<'\n';
                                return NULL;
                                break;
                            }
                        }
                        char* id = (char*)malloc(sizeof(char)*(strlen(tokens[i])+1));
                        strcpy(id, tokens[i]);
                        this->add(new Variable(*type, id));
                        lvalue = this->vars[this->vars.size()-1];
                    }else {
                        for(unsigned int j=0; j<this->vars.size(); j++) {
                            if(!strcmp(this->vars[j]->id, tokens[i])) {
                                lvalue = this->vars[j];
                                break;
                            }
                        }
                        if(!lvalue) {
                            std::cout<<"unidentified variable "<<tokens[i]<<'\n';
                            return NULL;
                        }
                    }
                    }break;
                case 5:
                    int start = ++i;
                    int paranthesis = 1;
                    while(i < size) {
                        group = checkGroup(tokens[i][0]);
                        if(group == 5) {
                            paranthesis++;
                        }
                        if(group == 6) {
                            paranthesis--;
                        }
                        if(paranthesis < 1) {
                            break;
                        }
                        i++;
                    }
                    if(i == size) {
                        std::cout<<"expected "<<paranthesis<<" closing paranthesis before end of statement\n";
                        return NULL;
                    }
                    lvalue = this->parse(tokens+start, i-start);
                    break;
            }
        }else {
            for(unsigned int j=0; j<this->ops.size(); j++) {
                if(!strcmp(this->ops[j]->id, tokens[i])) {
                    op = this->ops[j];
                    break;
                }
            }
            if(!op) {
                std::cout<<"unidentified operator "<<tokens[i]<<'\n';
                return NULL;
            }
            if(op->uselvalue) {
                if(lvalue->get()) {
                    if(!typecmp(lvalue->get()->type, op->lvalue)) {
                        std::cout<<"operator "<<op->id<<" requires "<<op->lvalue.id<<" as lvalue\n";
                        return NULL;
                    }
                }else {
                    std::cout<<"operator "<<op->id<<" needs "<<op->lvalue.id<<" as lvalue\n";
                    return NULL;
                }
            }else if(lvalue->get()) {
                std::cout<<"operator "<<op->id<<" doesn't need a lvalue\n";
                return NULL;
            }
            int start = ++i;
            int priority = (unsigned int)~0>>1;
            while(i<size) {
                group = checkGroup(tokens[i][0]);
                if(group == 1) {
                    Operator* op2;
                    for(unsigned int j=0; j<this->ops.size(); j++) {
                        if(!strcmp(this->ops[j]->id, tokens[i])) {
                            op2 = this->ops[j];
                            break;
                        }
                    }
                    if(!op2) {
                        std::cout<<"unidentified operator "<<tokens[i]<<'\n';
                        return NULL;
                    }
                    priority = op2->priority;
                }else if(group == 5) {
                    int paranthesis = 0;
                    while(i < size) {
                        group = checkGroup(tokens[i][0]);
                        if(group == 5) {
                            paranthesis++;
                        }
                        if(group == 6) {
                            paranthesis--;
                        }
                        if(paranthesis < 1) {
                            break;
                        }
                        i++;
                    }
                    if(i == size) {
                        std::cout<<"expected "<<paranthesis<<" closing paranthesis before end of statement\n";
                        return NULL;
                    }
                }
                if(!(op->priority<priority)) {
                    break;
                }
                i++;
            }
            rvalue = this->parse(tokens+start, i-start);
            i--;
            if(op->uservalue) {
                if(rvalue->get()) {
                    if(!typecmp(rvalue->get()->type, op->rvalue)) {
                        std::cout<<"operator "<<op->id<<" requires "<<op->rvalue.id<<" as rvalue\n";
                        return NULL;
                    }
                }else {
                    std::cout<<"operator "<<op->id<<" needs "<<op->rvalue.id<<" as rvalue\n";
                    return NULL;
                }
            }else if(rvalue->get()) {
                std::cout<<"operator "<<op->id<<" doesn't need a rvalue\n";
                return NULL;
            }
            lvalue = new Expression(lvalue, op, rvalue);
        }
    }
    return lvalue;
}
void Scope::execute(const char* text) {
    for(unsigned int i=0; i<strlen(text); i++) {
        //Lexer
        std::vector<std::string*>* tokens = new std::vector<std::string*>;
        int group=-1;
        for(unsigned int j = -1; i<strlen(text); i++) {
            if((group != checkGroup(text[i]) && !(group == 3 && checkGroup(text[i]) == 2)) || checkGroup(text[i]) == 5 || checkGroup(text[i]) == 6) {
                if(checkGroup(text[i]) == 4) {
                    break;
                }
                if(checkGroup(text[i]) != 0) {
                    tokens->push_back(new std::string());
                    j++;
                }
                group = checkGroup(text[i]);
            }
            if(group != 0) {
                (*tokens)[j]->push_back(text[i]);
            }
        }
        //Parser
        const char** tokenstemp = (const char**)malloc(sizeof(const char*)*tokens->size());
        for(unsigned int j=0; j<tokens->size(); j++) {
            tokenstemp[j] = (*tokens)[j]->c_str();
        }
        Variable* expr = this->parse(tokenstemp, tokens->size());
        free(tokenstemp);
        for(unsigned int j=0; j<tokens->size(); j++) {
            delete (*tokens)[j];
        }
        delete tokens;
        //Evaluator
        std::cout<<*(int*)expr->get()->data<<'\n';
        delete expr;
    }
}
