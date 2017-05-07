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
CC=g++
CCFLAGS=-Wall -Wno-write-strings -std=c++11 -ggdb -O0 -I. -I./karolslib/src
LD=$(CC)
#For linux:
LDFLAGS=
LDLIBS=-L./karolslib/build -lkarolslib -lSDL2 -lSDL2_image
#For windows:
#LDFLAGS=-pthread -mwindows -lgdi32
#LDLIBS=-L./karolslib/build -lkarolslib

all: clean textcraft

textcraft: game.o graphics.o main.o shapes.o
	$(LD) $(LDFLAGS) $(BD)/game.o $(BD)/graphics.o $(BD)/main.o $(BD)/shapes.o $(LDLIBS) -o $(BD)/textcraft

game.o:
	$(CC) $(CCFLAGS) $(SD)/game.cpp -c -o $(BD)/game.o

graphics.o:
	$(CC) $(CCFLAGS) $(SD)/graphics.cpp -c -o $(BD)/graphics.o

main.o:
	$(CC) $(CCFLAGS) $(SD)/main.cpp -c -o $(BD)/main.o

shapes.o:
	$(CC) $(CCFLAGS) $(SD)/shapes.cpp -c -o $(BD)/shapes.o

clean:
	rm -f $(BD)/textcraft $(BD)/game.o $(BD)/graphics.o $(BD)/main.o $(BD)/shapes.o
