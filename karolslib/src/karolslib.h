/*
 * karolslib.h
 * karolslib Source Code
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
#ifndef KAROLSLIB_H
#define KAROLSLIB_H
#define KAROLSLIB_MAJOR_VERSION 0
#define KAROLSLIB_MINOR_VERSION 2

#define main KL_user_main
int KL_user_main(int argc, char** argv);
#if defined(_WIN32)
#include <windows.h>
extern HINSTANCE KL_hInstance;
extern HINSTANCE KL_hPrevInstance;
extern PSTR KL_szCmdLine;
extern int KL_iCmdShow;
#endif

bool KL_init();
void KL_deinit();

#endif
