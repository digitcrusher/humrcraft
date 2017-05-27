/*
 * language.h
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

struct type_t {
    const char* id;
    int size;
};
struct variable_t {
    const char* id;
    void* data;
    type_t type;
};
struct scope_t {
    type_t* types;
    int typessize;
    variable_t* vars;
    int varssize;
};
void execute(scope_t* scope, const char* text);

#endif
