#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <stdio.h>


const char *half_maze = 
    "WWWWWWWWWWWWWWWW"
    "W..............W"
    "W.WWWWW.WWWWWW.W"
    "W*WWWWW.WWWWWW.W"
    "W..............."
    "W.WWWWW.W.WWWWWW"
    "W.......W......W"
    "WWWWWWW.WWWWWW.W"
    "........W......."
    "WWWWWWW.W.WWWWWW"
    "W.......W.W     "
    "W.W.WWW.W.W     "
    "W.W.WWW.W.W     "
    "W.........W     "
    "WWWWWWW.W.WWWWWW"
    "........W......."
    "WWWWWWW.W.WWWWWW"
    "W..............W"
    "W.WWWWW.WWWWWW.W"
    "W*....W........."
    "WWWWW.W.W.WWWWWW"
    "W.......W......W"
    "W.WWWWWWWWWWWW.W"
    "W..............."
    "WWWWWWWWWWWWWWWW";


const char *buildmaze(const char* half_maze) {

    char *maze = (char *) malloc(strlen(half_maze) * 2 + 1);
    int x, y;
    const char *src = half_maze;
    char *tgt = maze;

    for (y=0; y < 25; y++) {
        for (x=0; x < 16; x++) {
            *tgt++ = *src++;
        }
        for (x = 15; x >= 0; x--) {
            *tgt++ = *--src;
        }
        src += 16;
    }
    *tgt = 0;
    return maze;
}


int get_surrounding_walls2(const char *maze, int x, int y) {

    int is_wall[8];
    int dirs[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

    for (int i=0; i<8; i++) {
        int newx = x + dirs[i][0];
        int newy = y + dirs[i][1];

        if (newx < 0 || newx > 31 || newy < 0 || newy > 24) {
            is_wall[i] = 0;
        } else {
            is_wall[i] = (maze[32 * newy + newx] == 'W');
        }
    }

    int w[4];
    for (int dir=0; dir<4; dir++) {
        int sum = 0;
        for (int i = 0; i<5; i++) {
            sum += is_wall[(2 * dir + i + 6) % 8];
        }
        w[dir] = is_wall[2 * dir] && sum < 5;
    }
    return w[0] + 2 * w[1] + 4 * w[2] + 8 * w[3];    
}


void drawmaze(const char *maze, SDL_Renderer *renderer, SDL_Texture *texture, int w, int h, int scale) {

    SDL_Rect src, tgt;
    src.w = src.h = 8;
    tgt.w = src.w * scale; 
    tgt.h = src.h * scale;

    for (int y=0; y<25; y++) {
        for (int x=0; x<32; x++) {
            switch(maze[32 * y + x]) {
                case 'W':
                    src.x = 8 * get_surrounding_walls2(maze, x, y);
                    if ((x == 0 || x == 31) && (y == 0 || y == 24)) {
                        src.y = 40;
                    } else {
                        src.y = 8 * (y == 0) + 16 * (y == 24) + 24 * (x == 0) + 32 * (x == 31);
                    }
                    break;
                case ' ':
                    src.x = 0;
                    src.y = 48;
                    break;
                case '.':
                    src.x = 8;
                    src.y = 48;
                    break;
                case '*':
                    src.x = 16;
                    src.y = 48;
                    break;
            }
            tgt.x = x * tgt.w + (w - 32 * tgt.w) / 2;
            tgt.y = y * tgt.h + (h - 25 * tgt.h) / 2;
            SDL_RenderCopy(renderer, texture, &src, &tgt);
        }
    }
}


int main( int argc, char* args[] ) 
{ 
    const char *maze = buildmaze(half_maze);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Boot-Man III", 100, 100, 640, 480, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    int scalex = w / 256;
    int scaley = h / 200;
    int scale = scalex < scaley ? scalex : scaley;;

    SDL_Surface *maze_bitmaps = SDL_LoadBMP("bootman_III_3.bmp");
    SDL_Texture *maze_texture = SDL_CreateTextureFromSurface(renderer, maze_bitmaps);

    drawmaze(maze, renderer, maze_texture, w, h, scale);
    SDL_RenderPresent(renderer);

    int stopit = 0;
    while (!stopit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                stopit = 1;
            }
        }
    }

    SDL_DestroyWindow(window); 
    SDL_Quit(); 

    return 0;   
}