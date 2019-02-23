#include "Board.h"

Board::Board(SDL_Renderer* ren, int cornerX, int cornerY, int line_length)
{
    rBoard[0] = new SDL_Rect[3];
    rBoard[1] = new SDL_Rect[3];
    rBoard[2] = new SDL_Rect[3];
    pBoard[0] = new Point[3];
    pBoard[1] = new Point[3];
    pBoard[2] = new Point[3];

    this->cornerX = cornerX;
    this->cornerY = cornerY;
    this->line_length = line_length;
    this->renderer = ren;

    line1.x = cornerX+line_length/3-10;
    line1.y = cornerY;
    line1.w = 10;
    line1.h = 0;
    line2.x = cornerX+line_length-line_length/3;
    line2.y = cornerY;
    line2.w = 10;
    line2.h = 0;
    line3.x = cornerX;
    line3.y = cornerY+line_length/3;
    line3.w = 0;
    line3.h = 10;
    line4.x = cornerX;
    line4.y = cornerY+line_length-line_length/3;
    line4.w = 0;
    line4.h = 10;

    pBoard[0][0].x = cornerX+(line_length/6)-(75/2); pBoard[0][0].y = cornerY+(line_length/6)-(75/2);
    pBoard[0][1].x = pBoard[0][0].x+line_length/3; pBoard[0][1].y = cornerY+(line_length/6)-(75/2);
    pBoard[0][2].x = pBoard[0][1].x+line_length/3; pBoard[0][2].y = cornerY+(line_length/6)-(75/2);
    pBoard[1][0].x = pBoard[0][0].x; pBoard[1][0].y = pBoard[0][0].y+line_length/3;
    pBoard[1][1].x = pBoard[0][1].x; pBoard[1][1].y = pBoard[1][0].y;
    pBoard[1][2].x = pBoard[0][2].x; pBoard[1][2].y = pBoard[1][0].y;
    pBoard[2][0].x = pBoard[0][0].x; pBoard[2][0].y = pBoard[1][0].y+line_length/3;
    pBoard[2][1].x = pBoard[0][1].x; pBoard[2][1].y = pBoard[2][0].y;
    pBoard[2][2].x = pBoard[0][2].x; pBoard[2][2].y = pBoard[2][0].y;

    rBoard[0][0] = {cornerX,cornerY,line_length/3,line_length/3};
    rBoard[0][1] = {rBoard[0][0].x+line_length/3, cornerY, line_length/3, line_length/3};
    rBoard[0][2] = {rBoard[0][1].x+line_length/3, cornerY, line_length/3, line_length/3};
    rBoard[1][0] = {rBoard[0][0].x,cornerY+line_length/3, line_length/3, line_length/3};
    rBoard[1][1] = {rBoard[0][1].x,rBoard[1][0].y, line_length/3, line_length/3};
    rBoard[1][2] = {rBoard[0][2].x,rBoard[1][1].y, line_length/3, line_length/3};
    rBoard[2][0] = {rBoard[0][0].x,rBoard[1][0].y+line_length/3,line_length/3,line_length/3};
    rBoard[2][1] = {rBoard[0][1].x,rBoard[2][0].y,line_length/3,line_length/3};
    rBoard[2][2] = {rBoard[0][2].x,rBoard[2][0].y,line_length/3,line_length/3};

}


void Board::update() {
    if(animate) {
        if(line1.h < line_length) line1.h+=5;
        if(line2.h < line_length) line2.h+=5;
        if(line3.w < line_length) line3.w+=5;
        if(line4.w < line_length) line4.w+=5;
        //std::cout << "updating...\n";
    }
}

void Board::render() {

    SDL_SetRenderDrawColor(renderer, 185, 149, 102, 255);
	SDL_RenderFillRect(renderer, &line1);
	SDL_RenderFillRect(renderer, &line2);
	SDL_RenderFillRect(renderer, &line3);
	SDL_RenderFillRect(renderer, &line4);

}

void Board::startAnimation() {
    if(!this->animate) this->animate = true;
}

SDL_Rect** Board::get_rBoard() {
    return this->rBoard;
}

Point** Board::get_pBoard() {
    return this->pBoard;
}
