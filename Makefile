# Makefile
# textcraft Source Code
# Available on Github
#
# Copyright (C) 2017 Karol "digitcrusher" Łacina
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
SD=./src
BD=./build
CC=gcc
CCFLAGS=-Wall -Wextra -Wpedantic -ggdb -O0
CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-write-strings -std=c++17 -ggdb -O0 -I. -I./karolslib/src
LD=$(CXX)
#For linux:
LDFLAGS=
LDLIBS=-L./karolslib/build -lkarolslib -L/usr/lib/X11R6/lib -lX11 -lSDL2 -lSDL2_image -lSDL2_ttf
#For windows:
#LDFLAGS=-pthread -mwindows -lgdi32
#LDLIBS=-L./karolslib/build -lkarolslib

all: clean textcraft

textcraft: brainfuck.o graphics.o gui.o language.o main.o nbp.o network.o procedural.o renderers.o shapes.o world.o
	$(LD) $(LDFLAGS) $(BD)/brainfuck.o $(BD)/graphics.o $(BD)/gui.o $(BD)/language.o $(BD)/main.o $(BD)/nbp.o $(BD)/network.o $(BD)/procedural.o $(BD)/renderers.o $(BD)/shapes.o $(BD)/world.o $(LDLIBS) -o $(BD)/textcraft

brainfuck.o:
	$(CXX) $(CXXFLAGS) $(SD)/brainfuck.cpp -c -o $(BD)/brainfuck.o

graphics.o:
	$(CXX) $(CXXFLAGS) $(SD)/graphics.cpp -c -o $(BD)/graphics.o

gui.o:
	$(CXX) $(CXXFLAGS) $(SD)/gui.cpp -c -o $(BD)/gui.o

language.o:
	$(CXX) $(CXXFLAGS) $(SD)/language.cpp -c -o $(BD)/language.o

main.o:
	$(CXX) $(CXXFLAGS) $(SD)/main.cpp -c -o $(BD)/main.o

nbp.o:
	$(CC) $(CCFLAGS) $(SD)/nbp.c -c -o $(BD)/nbp.o

network.o:
	$(CXX) $(CXXFLAGS) $(SD)/network.cpp -c -o $(BD)/network.o

procedural.o:
	$(CXX) $(CXXFLAGS) $(SD)/procedural.cpp -c -o $(BD)/procedural.o

renderers.o:
	$(CXX) $(CXXFLAGS) $(SD)/renderers.cpp -c -o $(BD)/renderers.o

shapes.o:
	$(CXX) $(CXXFLAGS) $(SD)/shapes.cpp -c -o $(BD)/shapes.o

world.o:
	$(CXX) $(CXXFLAGS) $(SD)/world.cpp -c -o $(BD)/world.o

clean:
	rm -f $(BD)/textcraft $(BD)/brainfuck.o $(BD)/graphics.o $(BD)/gui.o $(BD)/language.o $(BD)/main.o $(BD)/nbp.o $(BD)/network.o $(BD)/procedural.o $(BD)/renderers.o $(BD)/shapes.o $(BD)/world.o
