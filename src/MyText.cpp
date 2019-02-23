#include "MyText.h"
#include <string>
using namespace std;

MyText::MyText(const char* t, TTF_Font* f, int x, int y, SDL_Color c, SDL_Renderer* renderer)
{

    this->t = t;
    this->renderer = renderer;
    //font = TTF_OpenFont("OpenSans-Regular.ttf", fontSize);
    font = f;
    if(font == NULL) {
        //std::cout << "FontSize: " << fontSize << " ";
        std::cout << "There was an error! " << TTF_GetError() << "\n";
        return;
    }

    message = TTF_RenderText_Solid(font, t, c);
    text = SDL_CreateTextureFromSurface(renderer, message);
    SDL_QueryTexture(text, NULL, NULL, &w, &h);
    textRect.x = x;
    textRect.y = y;
    textRect.w = w;
    textRect.h = h;

    //SDL_DestroyTexture(text);
    SDL_FreeSurface(message);

}

SDL_Texture* MyText::getText() {
    return text;
}

SDL_Rect* MyText::getTextRect() {
    return &textRect;
}

void MyText::setText(const char* t) {
    message = TTF_RenderText_Solid(this->font, t, this->textColor);
    text = SDL_CreateTextureFromSurface(renderer, message);
    SDL_QueryTexture(text, NULL, NULL, &w, &h);
    textRect.w = w;
    textRect.h = h;
}

void MyText::setColor(SDL_Color c) {
    this->textColor = c;
}

string MyText::getTextInString() {
    string str(t);
    return t;
}

SDL_Color MyText::getColor() {
    return textColor;
}
