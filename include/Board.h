#ifndef BOARD_H
#define BOARD_H

#include <Game.h>

struct Point {
    int x,y;
};

class Board
{
    public:
        Board(SDL_Renderer* ren, int cornerX, int cornerY, int line_length);
        void render();
        void update();
        void startAnimation();
        SDL_Rect** get_rBoard();
        Point** get_pBoard();

    private:
        SDL_Renderer* renderer;
        int cornerX;
        int cornerY;
        int line_length;
        SDL_Rect line1,line2,line3,line4;
        Point** pBoard = new Point*[3];
        SDL_Rect** rBoard = new SDL_Rect*[3]; //[3][3]
        bool animate = false;

};

#endif // BOARD_H
