# Makefile
# humrcraft Source Code
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
CC=i686-w64-mingw32-gcc
CCFLAGS=-Wall -Wextra -Wpedantic -ggdb -O0
CXX=i686-w64-mingw32-g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-write-strings -std=c++17 -ggdb -O0 -I. -I./mingw32/SDL2/i686-w64-mingw32/include
LD=$(CXX)
#For linux:
#LDFLAGS=-m32
#LDLIBS=-L/usr/lib -L/usr/lib/X11R6/lib -lX11 -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU
#For windows:
LDFLAGS=-m32 -mwindows
LDLIBS=-L./mingw32/SDL2/i686-w64-mingw32/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lopengl32 -lglu32

all: clean humrcraft

humrcraft: brainfuck.o game.o graphics.o gui.o language.o main.o math.o maze.o module.o nbp.o procedural.o renderers.o shapes.o utils.o world.o
	$(LD) $(LDFLAGS) $(BD)/brainfuck.o $(BD)/game.o $(BD)/graphics.o $(BD)/gui.o $(BD)/language.o $(BD)/main.o $(BD)/math.o $(BD)/maze.o $(BD)/module.o $(BD)/nbp.o $(BD)/procedural.o $(BD)/renderers.o $(BD)/shapes.o $(BD)/utils.o $(BD)/world.o $(LDLIBS) -o $(BD)/humrcraft

brainfuck.o:
	$(CC) $(CCFLAGS) $(SD)/brainfuck.c -c -o $(BD)/brainfuck.o

game.o:
	$(CXX) $(CXXFLAGS) $(SD)/game.cpp -c -o $(BD)/game.o

graphics.o:
	$(CXX) $(CXXFLAGS) $(SD)/graphics.cpp -c -o $(BD)/graphics.o

gui.o:
	$(CXX) $(CXXFLAGS) $(SD)/gui.cpp -c -o $(BD)/gui.o

language.o:
	$(CXX) $(CXXFLAGS) $(SD)/language.cpp -c -o $(BD)/language.o

main.o:
	$(CXX) $(CXXFLAGS) $(SD)/main.cpp -c -o $(BD)/main.o

math.o:
	$(CXX) $(CXXFLAGS) $(SD)/math.cpp -c -o $(BD)/math.o

maze.o:
	$(CC) $(CCFLAGS) $(SD)/maze.c -c -o $(BD)/maze.o

module.o:
	$(CC) $(CCFLAGS) $(SD)/module.c -c -o $(BD)/module.o

nbp.o:
	$(CC) $(CCFLAGS) $(SD)/nbp.c -c -o $(BD)/nbp.o

procedural.o:
	$(CXX) $(CXXFLAGS) $(SD)/procedural.cpp -c -o $(BD)/procedural.o

renderers.o:
	$(CXX) $(CXXFLAGS) $(SD)/renderers.cpp -c -o $(BD)/renderers.o

shapes.o:
	$(CXX) $(CXXFLAGS) $(SD)/shapes.cpp -c -o $(BD)/shapes.o

utils.o:
	$(CXX) $(CXXFLAGS) $(SD)/utils.cpp -c -o $(BD)/utils.o

world.o:
	$(CXX) $(CXXFLAGS) $(SD)/world.cpp -c -o $(BD)/world.o

clean:
	rm -f $(BD)/humrcraft $(BD)/brainfuck.o $(BD)/game.o $(BD)/graphics.o $(BD)/gui.o $(BD)/language.o $(BD)/main.o $(BD)/math.o $(BD)/maze.o $(BD)/module.o $(BD)/nbp.o $(BD)/procedural.o $(BD)/renderers.o $(BD)/shapes.o $(BD)/utils.o $(BD)/world.o
