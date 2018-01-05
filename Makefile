# Makefile
# humrcraft Source Code
# Available on Github
#
# Copyright (c) 2017 Karol "digitcrusher" Łacina
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
SD=./src
BD=./build
CC=gcc#i686-w64-mingw32-gcc
CCFLAGS=-Wall -Wextra -Wpedantic -std=c11 -ggdb -O0
CXX=g++#i686-w64-mingw32-g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-write-strings -std=c++17 -ggdb -O0#-I./mingw32/SDL2/i686-w64-mingw32/include
LD=$(CXX)
#For linux:
LDFLAGS=
LDLIBS=-L/usr/lib/X11R6/lib -lX11 -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU
#For windows:
#LDFLAGS=-m32 -mwindows
#LDLIBS=-L./mingw32/SDL2/i686-w64-mingw32/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lopengl32 -lglu32

all: clean humrcraft

humrcraft: brainfuck.o game.o graphics.o gui.o language.o main.o math.o maze.o module.o nbp.o procedural.o renderers.o shapes.o terminal.o textsweeper.o utils.o wad.o world.o
	$(LD) $(LDFLAGS) $(BD)/brainfuck.o $(BD)/game.o $(BD)/graphics.o $(BD)/gui.o $(BD)/language.o $(BD)/main.o $(BD)/math.o $(BD)/maze.o $(BD)/module.o $(BD)/nbp.o $(BD)/procedural.o $(BD)/renderers.o $(BD)/shapes.o $(BD)/terminal.o $(BD)/textsweeper.o $(BD)/utils.o $(BD)/wad.o $(BD)/world.o $(LDLIBS) -o $(BD)/humrcraft

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

terminal.o:
	$(CXX) $(CXXFLAGS) $(SD)/terminal.cpp -c -o $(BD)/terminal.o

textsweeper.o:
	$(CXX) $(CXXFLAGS) $(SD)/textsweeper.cpp -c -o $(BD)/textsweeper.o

utils.o:
	$(CXX) $(CXXFLAGS) $(SD)/utils.cpp -c -o $(BD)/utils.o

wad.o:
	$(CC) $(CCFLAGS) $(SD)/wad.c -c -o $(BD)/wad.o

world.o:
	$(CXX) $(CXXFLAGS) $(SD)/world.cpp -c -o $(BD)/world.o

clean:
	rm -f $(BD)/humrcraft $(BD)/brainfuck.o $(BD)/game.o $(BD)/graphics.o $(BD)/gui.o $(BD)/language.o $(BD)/main.o $(BD)/math.o $(BD)/maze.o $(BD)/module.o $(BD)/nbp.o $(BD)/procedural.o $(BD)/renderers.o $(BD)/shapes.o $(BD)/terminal.o $(BD)/textsweeper.o $(BD)/utils.o $(BD)/wad.o $(BD)/world.o
