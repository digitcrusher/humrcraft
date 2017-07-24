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

type_t returntype = {"", "", sizeof(Variable*), 3};
type_t anytype = {"", "", 0, 2};
type_t voidtype = {"void", "", sizeof(void), 0};
type_t inttype = {"int", "", sizeof(int), 1};

bool typecmp(type_t type, type_t type2) {
    return (!strcmp(type.id, type2.id) && !strcmp(type.opts, type2.opts) && type.size == type2.size) || (type.type == 1 && type2.type == 1) || type.type == 2 || type2.type == 2;
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
     * 5 - opening parentheses
     * 6 - closing parentheses
     * 7 - opening curly brackets
     * 8 - closing curly brackets
     */
    if(c == 0 || c == 10 || c == 11 || c == 13 || c == 32) {
        return 0;
    }else if(c == 59) {
        return 4;
    }else if(c == 40) {
        return 5;
    }else if(c == 41) {
        return 6;
    }else if(c == 123) {
        return 7;
    }else if(c == 125) {
        return 8;
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
    this->scope = NULL;
    this->addType(anytype);
    this->addType(voidtype);
    this->addType(inttype);
    this->addOp(new Operator("=", 0, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                *(int*)lvalue->get()->data = *(int*)rvalue->get()->data;
            }
        }
        return lvalue->get();
    }));

    this->addOp(new Operator("||", 1, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data || *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("&&", 2, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data && *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("|", 3, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data | *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("^", 4, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data ^ *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("&", 5, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data & *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("==", 6, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data == *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("!=", 6, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data != *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator(">", 7, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data > *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator(">=", 7, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data >= *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("<", 7, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data < *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("<", 7, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data <= *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("<<", 8, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data << *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator(">>", 8, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data >> *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("+", 9, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data + *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("-", 9, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data - *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("*", 10, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data * *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("/", 10, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data / *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("%", 10, inttype, inttype, 1, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get() && rvalue->get()) {
            if(varcmp(lvalue->get(), inttype) && varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = *(int*)lvalue->get()->data % *(int*)rvalue->get()->data;
                Variable* var = new Variable(lvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("--", 11, voidtype, inttype, 0, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(rvalue->get()) {
            if(varcmp(rvalue->get(), inttype)) {
                --*(int*)rvalue->get()->data;
                if(rvalue->scope) {
                    return rvalue->get();
                }else {
                    Variable* var = new Variable(rvalue->get()->type, "");
                    memcpy(var->data, rvalue->get()->data, var->type.size);
                    return var;
                }
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("++", 11, voidtype, inttype, 0, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(rvalue->get()) {
            if(varcmp(rvalue->get(), inttype)) {
                ++*(int*)rvalue->get()->data;
                if(rvalue->scope) {
                    return rvalue->get();
                }else {
                    Variable* var = new Variable(rvalue->get()->type, "");
                    memcpy(var->data, rvalue->get()->data, var->type.size);
                    return var;
                }
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("-", 11, voidtype, inttype, 0, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(rvalue->get()) {
            if(varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = -*(int*)rvalue->get()->data;
                Variable* var = new Variable(rvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("+", 11, voidtype, inttype, 0, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(rvalue->get()) {
            if(varcmp(rvalue->get(), inttype)) {
                int* result = (int*)malloc(sizeof(int));
                *result = +*(int*)rvalue->get()->data;
                Variable* var = new Variable(rvalue->get()->type, "", result);
                return var;
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("*", 11, voidtype, anytype, 0, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(rvalue->get()) {
            type_t type = rvalue->get()->type;
            char* opts = (char*)malloc(sizeof(char)*(strlen(type.opts)+2));
            strcpy(opts, type.opts);
            opts[strlen(type.opts)] = '*';
            opts[strlen(type.opts)+1] = '\0';
            free(type.opts);
            type.opts = opts;
            type.type = 1;
            Variable* var = new Variable(type, "");
            *(void**)var->data = rvalue->get()->data;
            return var;
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("&", 11, voidtype, anytype, 0, 1, 0, [](Variable* lvalue, Variable* rvalue) {
        if(rvalue->get()) {
            type_t type = rvalue->get()->type;
            char* opts = (char*)malloc(sizeof(char)*(strlen(type.opts)+2));
            strcpy(opts, type.opts);
            opts[strlen(type.opts)] = '*';
            opts[strlen(type.opts)+1] = '\0';
            free(type.opts);
            type.opts = opts;
            type.type = 1;
            Variable* var = new Variable(type, "");
            *(void**)var->data = rvalue->get()->data;
            return var;
        }
        return (Variable*)NULL;
    }));

    this->addOp(new Operator("--", 12, inttype, voidtype, 1, 0, 1, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get()) {
            if(varcmp(lvalue->get(), inttype)) {
                --*(int*)lvalue->get()->data;
                if(lvalue->scope) {
                    return lvalue->get();
                }else {
                    Variable* var = new Variable(lvalue->get()->type, "");
                    memcpy(var->data, lvalue->get()->data, var->type.size);
                    return var;
                }
            }
        }
        return (Variable*)NULL;
    }));
    this->addOp(new Operator("++", 12, inttype, voidtype, 1, 0, 1, [](Variable* lvalue, Variable* rvalue) {
        if(lvalue->get()) {
            if(varcmp(lvalue->get(), inttype)) {
                ++*(int*)lvalue->get()->data;
                if(lvalue->scope) {
                    return lvalue->get();
                }else {
                    Variable* var = new Variable(lvalue->get()->type, "");
                    memcpy(var->data, lvalue->get()->data, var->type.size);
                    return var;
                }
            }
        }
        return (Variable*)NULL;
    }));
}
Scope::~Scope() {
    for(unsigned int i=0; i<this->vars.size(); i++) {
        delete this->vars[i];
    }
}
void Scope::addType(type_t type) {
    this->types.push_back(type);
}
void Scope::addOp(Operator* op) {
    this->ops.push_back(op);
    op->scope = this;
}
void Scope::addVar(Variable* var) {
    this->vars.push_back(var);
    var->scope = this;
}
Variable* Scope::getVar(const char* id) {
    for(unsigned int i=0; i<this->vars.size(); i++) {
        if(!strcmp(this->vars[i]->id, id)) {
            return this->vars[i];
            break;
        }
    }
    if(this->scope) {
        return this->scope->getVar(id);
    }
    return NULL;
}
Variable* Scope::parse(const char** tokens, size_t size) {
    auto parseParentheses = [&](const char** tokens, size_t size, unsigned int* offset)->Variable*{
        unsigned int i=0;
        if(checkGroup(tokens[i][0]) == 5) {
            int start = ++i;
            int parentheses = 1;
            while(i < size) {
                int group = checkGroup(tokens[i][0]);
                if(group == 5) {
                    parentheses++;
                }
                if(group == 6) {
                    parentheses--;
                }
                if(parentheses < 1) {
                    break;
                }
                i++;
            }
            if(i == size) {
                std::cout<<"expected "<<parentheses<<" closing parentheses before end of statement\n";
                return NULL;
            }
            if(offset) {
                *offset = i;
            }
            return this->parse(tokens+start, i-start);
        }
        return NULL;
    };
    auto parseCurlyBrackets = [&](const char** tokens, size_t size, unsigned int* offset)->Variable*{
        unsigned int i=0;
        if(checkGroup(tokens[i][0]) == 7) {
            std::string string;
            int brackets = 1;
            i++;
            while(i < size) {
                if(strlen(tokens[i]) == 1) {
                    int group = checkGroup(tokens[i][0]);
                    if(group == 7) {
                        brackets++;
                    }
                    if(group == 8) {
                        brackets--;
                    }
                    if(brackets < 1) {
                        break;
                    }
                }
                string.append(tokens[i]);
                i++;
            }
            if(i == size) {
                std::cout<<"expected "<<brackets<<" closing curly brackets before end of statement\n";
                return NULL;
            }
            if(offset) {
                *offset = i;
            }
            Scope scope;
            scope.scope = this;
            return scope.execute(string.c_str());
        }
        return NULL;
    };
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
                case 3:
                    if(!strcmp(tokens[i], "return")) {
                        int start = ++i;
                        while(i < size) {
                            i++;
                        }
                        Variable* value = this->parse(tokens+start, i-start);
                        if(!value) {
                            std::cout<<"return-statement with no value\n";
                            return NULL;
                        }
                        i += i-start;
                        lvalue = new Return(value);
                    }else if(!strcmp(tokens[i], "if")) {
                        i++;
                        Variable* condition = parseParentheses(tokens+i, size-i, &i);
                        if(condition) {
                            i++;
                        }else {
                            std::cout<<"expected an expression before end of statement\n";
                            return NULL;
                        }
                    }else {
                        type_t* type = NULL;
                        for(unsigned int j=0; j<this->types.size(); j++) {
                            if(!strcmp(this->types[j].id, tokens[i])) {
                                type = &this->types[j];
                                break;
                            }
                        }
                        if(type) {
                            std::string str;
                            for(i++; i < size; i++) {
                                if(checkGroup(tokens[i][0]) == 3) {
                                    break;
                                }
                                if(i == size-1) {
                                    std::cout<<"expected unqualified-id before end of statement\n";
                                    return NULL;
                                }
                                str.append(tokens[i]);
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
                            this->addVar(new Variable(*type, id));
                            lvalue = this->vars[this->vars.size()-1];
                            lvalue->type.opts = (char*)malloc(sizeof(char)*(str.size()+1));
                            strcpy(lvalue->type.opts, str.c_str());
                            if(lvalue->type.opts[strlen(lvalue->type.opts)-1] == '*') lvalue->type.type = 1;
                        }else {
                            lvalue = this->getVar(tokens[i]);
                            if(!lvalue) {
                                std::cout<<"unidentified variable "<<tokens[i]<<'\n';
                                return NULL;
                            }
                        }
                    }
                    break;
                case 5:
                    lvalue = parseParentheses(tokens+i, size-i, &i);
                    break;
                case 7:
                    lvalue = parseCurlyBrackets(tokens+i, size-i, &i);
                    break;
            }
        }else {
            for(unsigned int j=0; j<this->ops.size(); j++) {
                if(!strcmp(this->ops[j]->id, tokens[i]) && !(!this->ops[j]->uselvalue ^ !lvalue)) {
                    op = this->ops[j];
                    break;
                }
            }
            if(!op) {
                std::cout<<"unidentified operator "<<tokens[i]<<'\n';
                return NULL;
            }
            if(op->uselvalue) {
                if(lvalue) {
                    if(!typecmp(lvalue->get()->type, op->lvalue)) {
                        std::cout<<"operator "<<op->id<<" requires "<<op->lvalue.id<<" as lvalue\n";
                        return NULL;
                    }
                }else {
                    std::cout<<"operator "<<op->id<<" needs "<<op->lvalue.id<<" as lvalue\n";
                    return NULL;
                }
                if(lvalue->type.type == 3) {
                    return lvalue;
                }
            }else if(lvalue) {
                std::cout<<"operator "<<op->id<<" doesn't need a lvalue\n";
                return NULL;
            }
            if(op->uservalue) {
                int start = ++i;
                int priority = (unsigned int)~0>>1;
                while(i<size) {
                    group = checkGroup(tokens[i][0]);
                    if(group == 1) {
                        Operator* op2;
                        bool islvalue = checkGroup(tokens[i-1][0]) != 1;
                        for(unsigned int j=0; j<this->ops.size(); j++) {
                            if(!strcmp(this->ops[j]->id, tokens[i]) && !(!this->ops[j]->uselvalue ^ !islvalue)) {
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
                        int parentheses = 0;
                        while(i < size) {
                            group = checkGroup(tokens[i][0]);
                            if(group == 5) {
                                parentheses++;
                            }
                            if(group == 6) {
                                parentheses--;
                            }
                            if(parentheses < 1) {
                                break;
                            }
                            i++;
                        }
                        if(i == size) {
                            std::cout<<"expected "<<parentheses<<" closing parentheses before end of statement\n";
                            return NULL;
                        }
                    }
                    if(!(op->priority<priority) && op->uselvalue) {
                        break;
                    }
                    i++;
                }
                rvalue = this->parse(tokens+start, i-start);
                i--;
            }
            if(op->uservalue) {
                if(rvalue) {
                    if(!typecmp(rvalue->get()->type, op->rvalue)) {
                        std::cout<<"operator "<<op->id<<" requires "<<op->rvalue.id<<" as rvalue\n";
                        return NULL;
                    }
                }else {
                    std::cout<<"operator "<<op->id<<" needs "<<op->rvalue.id<<" as rvalue\n";
                    return NULL;
                }
            }else if(rvalue) {
                std::cout<<"operator "<<op->id<<" doesn't need a rvalue\n";
                return NULL;
            }
            lvalue = new Expression(lvalue, op, rvalue);
        }
    }
    return lvalue;
}
Variable* Scope::execute(const char* text) {
    for(unsigned int i=0; i<strlen(text); i++) {
        //Lexer
        std::vector<std::string*>* tokens = new std::vector<std::string*>;
        int group=-1;
        for(unsigned int j = -1; i<strlen(text); i++) {
            if(text[i] == '/' && i+1 < strlen(text) && text[i+1] == '/') {
                while(text[i] != '\n') i++;
                break;
            }
            if(text[i] == '/' && i+1 < strlen(text) && text[i+1] == '*') {
                char lastchar = '\0';
                while(lastchar != '*' || text[i] != '/') {
                    lastchar = text[i];
                    i++;
                }
                break;
            }
            if((group != checkGroup(text[i]) && !(group == 3 && checkGroup(text[i]) == 2)) || checkGroup(text[i]) == 5 || checkGroup(text[i]) == 6 ||
                checkGroup(text[i]) == 7 || checkGroup(text[i]) == 8) {
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
                if(group == 7) {
                    tokens->push_back(new std::string());
                    j++;
                    i++;
                    int brackets = 1;
                    while(i < strlen(text)) {
                        group = checkGroup(text[i]);
                        if(group == 7) {
                            brackets++;
                        }
                        if(group == 8) {
                            brackets--;
                        }
                        if(brackets < 1) {
                            break;
                        }
                        (*tokens)[j]->push_back(text[i]);
                        i++;
                    }
                    i--;
                }
            }
        }
        if(tokens->size() > 0) {
            for(int i=0; i<tokens->size(); i++) {
                std::cout<<(*tokens)[i]->c_str()<<',';
            }
            std::cout<<'\n';
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
            //Evaluator
            if(expr) {
                std::cout<<*(int*)expr->get()->data<<'\n';
                if(expr->type.type == 3) {
                    return expr;
                }
            }
            delete expr;
        }
        delete tokens;
    }
    return NULL;
}

Variable::Variable(type_t type, const char* id, void* data) {
    this->scope = NULL;
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

Operator::Operator(const char* id, int priority, type_t lvalue, type_t rvalue, bool uselvalue, bool uservalue, bool suffix
                  ,Variable* (*evalf)(Variable* lvalue, Variable* rvalue)) : Variable(voidtype, id) {
    this->suffix = suffix;
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
        return this->evalf(lvalue, rvalue);
    }
    return NULL;
}

Expression::Expression(Variable* lvalue, Operator* op, Variable* rvalue) : Variable(voidtype, "") {
    this->lvalue = lvalue;
    this->op = op;
    this->rvalue = rvalue;
}
Expression::~Expression() {
    if(this->op && this->op->suffix) {
        this->cache = this->op->eval(this->lvalue, this->rvalue);
    }
    if(this->lvalue && !this->lvalue->scope && this->lvalue != this->rvalue && this->lvalue != this->cache) {
        delete this->lvalue;
    }
    if(this->rvalue && !this->rvalue->scope && this->rvalue != this->lvalue && this->rvalue != this->cache) {
        delete this->rvalue;
    }
    if(this->cache && !this->cache->scope && this->cache != this->lvalue && this->cache != this->rvalue) {
        delete this->cache;
    }
}
Variable* Expression::get() {
    if(this->op && !this->cache) {
        if(!this->op->suffix) {
            this->cache = this->op->eval(this->lvalue, this->rvalue);
        }else {
            if(this->op->uselvalue) {
                this->cache = this->lvalue->get();
            }else {
                this->cache = this->rvalue->get();
            }
        }
    }
    return this->cache->get();
}

Return::Return(Variable* var) : Variable(returntype, "") {
    *(Variable**)this->data = var;
}
Return::~Return() {
    if(*(Variable**)this->data && !(*(Variable**)this->data)->scope) {
        delete *(Variable**)this->data;
    }
}
Variable* Return::get() {
    if(*(Variable**)this->data) {
        return (*(Variable**)this->data)->get();
    }
    return NULL;
}
