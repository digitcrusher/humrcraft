
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <math.h>
#include <conio.h>
#define M_PI 3.14159265358979323846
using namespace std;
long UF_GetMS() { //Get milliseconds
    LARGE_INTEGER f,c; //Union which will store frequency and ticks
    if(!QueryPerformanceFrequency(&f)) return 0; //Try to get processor clock frequency
    if(!QueryPerformanceCounter(&c)) return 0; //Try to get processor clock ticks
    return c.QuadPart/(f.QuadPart/1000); //Return milliseconds ( Ticks/( Frequency/MSperSECOND ) )
}
void hidecursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
/*
 * Author: Karol "digitcrusher" Łacina
 */
enum {
    Wall,
    Path,
    Footprint,
    Exit
};
enum {
    Up,
    Right,
    Down,
    Left
};
struct V {int x,y;};
bool operator== (const V p1, const V p2) {
    return p1.x == p2.x &&
           p1.y == p2.y;
}
struct square {int x1,y1,x2,y2;};
bool operator== (const square s1, const square s2) {
    return s1.x1 == s2.x1 &&
           s1.y1 == s2.y1 &&
           s1.x2 == s2.x2 &&
           s1.y2 == s2.y2;
}
struct tile {int type,direction;};
bool operator== (const tile t1, const tile t2) {
    return t1.type == t2.direction &&
           t1.direction == t2.type;
}
struct lfr {tile *l,*f,*r;int h;};
bool operator== (const lfr t1, const lfr t2) {
    return t1.l == t2.l &&
           t1.f == t2.f &&
           t1.r == t2.r &&
           t1.h == t2.h;
}
struct world {
    int w,h;
    tile* map;
    tile* null;
};
bool operator== (const world t1, const world t2) {
    return t1.w == t2.w &&
           t1.h == t2.h &&
           t1.map == t2.map &&
           t1.null == t2.null;
}
void createMap(int w, int h, world* world, tile null) {
    world->w = w;
    world->h = h;
    world->map = (tile*)malloc(sizeof(tile)*w*h);
    world->null = (tile*)malloc(sizeof(tile));
    *world->null = null;
}
void deleteMap(world* world) {
    free(world->map);
}
void resetMap(tile to, world* map) {
    for(int i=0;i<map->w*map->h;i++) {
        map->map[i] = to;
    }
}
bool sqrInSqr(square sqr1, square sqr2) {
    return (sqr2.x1 >= sqr1.x1 && sqr2.x1 <= sqr1.x2 && sqr2.y1 >= sqr1.y1 && sqr2.y1 <= sqr1.y2) ||
           (sqr2.x2 >= sqr1.x1 && sqr2.x2 <= sqr1.x2 && sqr2.y2 >= sqr1.y1 && sqr2.y2 <= sqr1.y2) ||
           (sqr2.x1 >= sqr1.x1 && sqr2.x1 <= sqr1.x2 && sqr2.y2 >= sqr1.y1 && sqr2.y2 <= sqr1.y2) ||
           (sqr2.x2 >= sqr1.x1 && sqr2.x2 <= sqr1.x2 && sqr2.y1 >= sqr1.y1 && sqr2.y1 <= sqr1.y2);
}
tile* getTile(int x, int y, world* map) {
    if(sqrInSqr({0,0,map->w-1,map->h-1}, {x,y,x,y}))
        return map->map+x+y*map->w;
    return map->null;
}
lfr generateLFR(int xs, int ys, int rot, world* map) {
    int i = -1;
    lfr lfr;
    lfr.h = rot;
    get:
    int x=0,y=0;
    switch((rot+i+4) % 4) {
        case Up:
            y=-1;
            break;
        case Left:
            x=-1;
            break;
        case Down:
            y=1;
            break;
        case Right:
            x=1;
            break;
    }
    switch(i) {
        case -1:
            lfr.l = getTile(xs+x, ys+y, map);
            break;
        case 0:
            lfr.f = getTile(xs+x, ys+y, map);
            break;
        case 1:
            lfr.r = getTile(xs+x, ys+y, map);
            break;
    }
    if(i!=1) {
        i++;
        goto get;
    }
    return lfr;
}
bool compareLFR(lfr lfr1, lfr lfr2) {
    return *lfr1.l==*lfr2.l &&
           *lfr1.f==*lfr2.f &&
           *lfr1.r==*lfr2.r;
}
bool createLinearMaze(V startpos, tile start, tile end, tile fillwith, int corridorlen, world* map) {
    if(!sqrInSqr({0,0,map->w-1,map->h-1}, {startpos.x,startpos.y,startpos.x,startpos.y,}))
        return 1;
    V point = startpos;
    int heading=Right;
    int random;
    for(int i=0;i<map->w*map->h/corridorlen;i++) {
        randomize:
        srand(UF_GetMS()*sqrt(rand()));
        random = rand() % 3 - 1;
        heading = (heading+random+4)%4;
        int x=0,y=0;
        switch(heading) {
            case Up:
                y=-1;
                break;
            case Left:
                x=-1;
                break;
            case Down:
                y=1;
                break;
            case Right:
                x=1;
                break;
        }
        if(!sqrInSqr({0,0,map->w-1,map->h-1}, {point.x+x*(corridorlen-1),point.y+y*(corridorlen-1),point.x+x*(corridorlen-1),point.y+y*(corridorlen-1)}))
            goto randomize;
        for(int i=0;i<corridorlen;i++) {
            *getTile(x*i+point.x,y*i+point.y,map) = fillwith;
        }
        point.x+=x*(corridorlen-1);
        point.y+=y*(corridorlen-1);
    }
    *getTile(startpos.x, startpos.y, map) = start;
    *getTile(point.x, point.y, map) = end;
}
int main() {
    hidecursor();
    world map;
    createMap(25,13,&map,{Wall,0});
    int lvlscomplete=0;
    
    play:
    resetMap({Wall,0}, &map);
    createLinearMaze({0,0},{Path,0},{Exit,0},{Path,0},3,&map);
    *map.null = {Wall, 0};
    bool complete = 0;
    int heading = Right;
    int random;
    int x=0,y=0;
    lfr curr,last = {map.null, map.null, map.null, 0};
    V point = {0,0};
    while(!complete) {
        //if key pressed restart
        if(_kbhit()) {
            _getch();
            complete=1;
        }
        
        //if player is on exit restart
        if((*getTile(point.x, point.y, &map)).type == Exit) {
            complete = 1;
            lvlscomplete++;
            break;
        }
        curr = generateLFR(point.x, point.y, heading, &map);
        *map.null = {Wall, 0};
        if(compareLFR(curr,{map.null, map.null, map.null, 0})) {
            heading = (heading+2)%4;
            last=curr;
        } else
        if(!compareLFR(curr, last)) {
            bool again=1;
            choose:
            srand(UF_GetMS()*sqrt(rand()));
            random = rand() % 3 - 1;
            x=0,y=0;
            switch((heading+random+4)%4) {
                case Up:
                    y=-1;
                    break;
                case Left:
                    x=-1;
                    break;
                case Down:
                    y=1;
                    break;
                case Right:
                    x=1;
                    break;
            }
            //check if there is a path with no footprint on it
            bool path=(*(curr.l)).type == Path ||
                      (*(curr.f)).type == Path ||
                      (*(curr.r)).type == Path;
            //if move is out of boundaries choose again
            if(!sqrInSqr({0,0,map.w-1,map.h-1}, {point.x+x,point.y+y,point.x+x,point.y+y}))
                goto choose;
            //check if there is a wall
            if((*getTile(point.x+x, point.y+y, &map)).type == Wall)
                goto choose;
            if(again && path) {
                again=!again;
                goto choose;
            }
            heading = (heading+random+4)%4;
            last=curr;
        }
        x=0,y=0;
        switch(heading) {
            case Up:
                y=-1;
                break;
            case Left:
                x=-1;
                break;
            case Down:
                y=1;
                break;
            case Right:
                x=1;
                break;
        }
        (*getTile(point.x, point.y, &map)).type = Footprint;
        (*getTile(point.x, point.y, &map)).direction = heading;
        point.x+=x;
        point.y+=y;
        
        
        /*
        //check if there is an exit around
        bool exit=map[(point.x-1)+w*point.y].type == Exit ||
                  map[(point.x+1)+w*point.y].type == Exit ||
                  map[point.x+w*(point.y-1)].type == Exit ||
                  map[point.x+w*(point.y+1)].type == Exit;
        
        //if player chooses to go the path with a footprint and there's a path with no footprint choose another way
        if((path || exit) && map[(point.x+x)+w*(point.y+y)].type == Footprint || map[(point.x+x)+w*(point.y+y)].type == Wall)
            goto choose;
        */
        
        //render start
        for(int y=0;y<map.h;y++) {
            for(int x=0;x<map.w;x++) {
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),{x,y});
                if(x == point.x && y == point.y)
                    cout<<"@";
                else
                    switch(getTile(x,y,&map)->type) {
                        case Wall:
                            cout<<"#";
                            break;
                        case Path:
                            cout<<" ";
                            break;
                        case Footprint:
                            switch(getTile(x,y,&map)->direction) {
                                case Up:
                                    cout<<"^";
                                    break;
                                case Left:
                                    cout<<"<";
                                    break;
                                case Down:
                                    cout<<"v";
                                    break;
                                case Right:
                                    cout<<">";
                                    break;
                            }
                            break;
                        case Exit:
                            cout<<"O";
                            break;
                    }
            }
        cout<<"\n";
        }
        cout<<lvlscomplete;
        //render end
    }
    goto play;
    
    deleteMap(&map);
    return 0;
}
