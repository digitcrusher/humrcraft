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
#include <vector>
#include <utils/utils.h>
#include "language.h"

type_t inttype = {"int", sizeof(int)};

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

Operator::Operator(const char* id, int priority, type_t lvalue, type_t rvalue
                  ,Variable* (*evalf)(Variable* lvalue, Variable* rvalue)) : Variable({"operator", sizeof(Operator)}, id) {
    this->postfix = 0;
    this->priority = priority;
    this->lvalue = lvalue;
    this->rvalue = rvalue;
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

Expression::Expression(Variable* lvalue, Operator* op, Variable* rvalue) : Variable({"expression", sizeof(Expression)}, "") {
    this->lvalue = lvalue;
    this->op = op;
    this->rvalue = rvalue;
    this->cache = NULL;
}
Expression::~Expression() {
    if(this->op && this->op->postfix) {
        this->cache = this->op->eval(this->lvalue->get(), this->rvalue->get());
    }
    delete this->lvalue;
    delete this->rvalue;
    if(this->cache) {
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
void Expression::clearCache() {
    if(this->cache) {
        delete this->cache;
        this->cache = NULL;
    }
}

bool varcmp(Variable* var, type_t type) {
    return !strcmp(var->get()->type.id, type.id) && var->get()->type.size == type.size;
}

Operator* ops[] = {new Operator("+", 1, inttype, inttype, [](Variable* lvalue, Variable* rvalue) {
    if(lvalue->get() && rvalue->get()) {
        if(varcmp(lvalue, inttype) && varcmp(rvalue, inttype)) {
            int* result = (int*)malloc(sizeof(int));
            *result =  *(int*)lvalue->get()->data + *(int*)rvalue->get()->data;
            Variable* var = new Variable(inttype, "", result);
            return var;
        }
    }
    return (Variable*)NULL;
}), new Operator("-", 1, inttype, inttype, [](Variable* lvalue, Variable* rvalue) {
    if(lvalue->get() && rvalue->get()) {
        if(varcmp(lvalue, inttype) && varcmp(rvalue, inttype)) {
            int* result = (int*)malloc(sizeof(int));
            *result =  *(int*)lvalue->get()->data - *(int*)rvalue->get()->data;
            Variable* var = new Variable(inttype, "", result);
            return var;
        }
    }
    return (Variable*)NULL;
}), new Operator("*", 2, inttype, inttype, [](Variable* lvalue, Variable* rvalue) {
    if(lvalue->get() && rvalue->get()) {
        if(varcmp(lvalue, inttype) && varcmp(rvalue, inttype)) {
            int* result = (int*)malloc(sizeof(int));
            *result =  *(int*)lvalue->get()->data * *(int*)rvalue->get()->data;
            Variable* var = new Variable(inttype, "", result);
            return var;
        }
    }
    return (Variable*)NULL;
}), new Operator("/", 2, inttype, inttype, [](Variable* lvalue, Variable* rvalue) {
    if(lvalue->get() && rvalue->get()) {
        if(varcmp(lvalue, inttype) && varcmp(rvalue, inttype)) {
            int* result = (int*)malloc(sizeof(int));
            *result =  *(int*)lvalue->get()->data / *(int*)rvalue->get()->data;
            Variable* var = new Variable(inttype, "", result);
            return var;
        }
    }
    return (Variable*)NULL;
})};
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
Variable* parse(const char** tokens, size_t size) {
    Variable* lvalue = NULL;
    Operator* op = NULL;
    Variable* rvalue = NULL;
    for(unsigned int i=0; i<size; i++) {
        int group = checkGroup(tokens[i][0]);
        if(group != 1) {
            if(group == 5) {
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
                lvalue = parse(tokens+start, i-start);
            }else {
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
                }
            }
        }else {
            for(int j=0; j<4; j++) {
                if(!strcmp(ops[j]->id, tokens[i])) {
                    op = ops[j];
                    break;
                }
                if(j == 4-1) {
                    std::cout<<"unidentified operator "<<tokens[i]<<'\n';
                    return NULL;
                }
            }
            if(op->uselvalue) {
                if(lvalue) {
                    if(strcmp(lvalue->get()->type.id, op->lvalue.id)) {
                        std::cout<<"operator "<<op->id<<" requires "<<op->lvalue.id<<" as lvalue\n";
                        return NULL;
                    }
                }else {
                    std::cout<<"operator "<<op->id<<" needs "<<op->lvalue.id<<" as lvalue\n";
                    return NULL;
                }
            }else {
                if(lvalue) {
                    if(strcmp(lvalue->get()->type.id, op->lvalue.id)) {
                        std::cout<<"operator "<<op->id<<" requires not a lvalue\n";
                        return NULL;
                    }
                }
            }
            int start = ++i;
            int priority = (unsigned int)~0>>1;
            while(i<size) {
                group = checkGroup(tokens[i][0]);
                if(group == 1) {
                    Operator* op2;
                    for(int j=0; j<4; j++) {
                        if(!strcmp(ops[j]->id, tokens[i])) {
                            op2 = ops[j];
                            break;
                        }
                        if(j == 4-1) {
                            std::cout<<"unidentified operator "<<tokens[i]<<'\n';
                            return NULL;
                        }
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
            rvalue = parse(tokens+start, i-start);
            i--;
            if(op->uservalue) {
                if(rvalue) {
                    if(strcmp(rvalue->get()->type.id, op->rvalue.id)) {
                        std::cout<<"operator "<<op->id<<" requires "<<op->rvalue.id<<" as rvalue\n";
                        return NULL;
                    }
                }else {
                    std::cout<<"operator "<<op->id<<" needs "<<op->rvalue.id<<" as rvalue\n";
                    return NULL;
                }
            }else {
                if(rvalue) {
                    if(strcmp(rvalue->get()->type.id, op->rvalue.id)) {
                        std::cout<<"operator "<<op->id<<" requires not a rvalue\n";
                        return NULL;
                    }
                }
            }
            lvalue = new Expression(lvalue, op, rvalue);
        }
    }
    return lvalue;
}
void execute(const char* text) {
    //Lexer
    std::vector<std::string*> tokens;
    int group=-1;
    for(unsigned int i=0, j = -1; i<strlen(text); i++) {
        if((group != checkGroup(text[i]) && !(group == 3 && checkGroup(text[i]) == 2)) || checkGroup(text[i]) == 5 || checkGroup(text[i]) == 6) {
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
    //Parser
    const char** tokenstemp = (const char**)malloc(sizeof(const char*)*tokens.size());
    for(unsigned int i=0; i<tokens.size(); i++) {
        tokenstemp[i] = tokens[i]->c_str();
    }
    Variable* expr = parse(tokenstemp, tokens.size());
    free(tokenstemp);
    for(unsigned int i=0; i<tokens.size(); i++) {
        delete tokens[i];
    }
    //Evaluator
    std::cout<<*(int*)expr->get()->data<<'\n';
    delete expr;
}
