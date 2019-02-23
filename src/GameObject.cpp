#include "GameObject.h"
#include "TextureManager.h"
#include <iostream>

GameObject::GameObject(const char* textureSheet, SDL_Renderer* ren, double x, double y)
{
    this->renderer = ren;
    this->texture = TextureManager::LoadTexture(textureSheet, ren);
    this->x = x;
    this->y = y;
}

void GameObject::update() {
    if(this->isShowing()) {
        this->x += this->speedX;
        this->y += this->speedY;
        this->degrees += this->rotation;

        if(this->simpleTexture) {
            this->destR.x = this->x;
            this->destR.y = this->y;
            this->destR.w = width;
            this->destR.h = height;
        }

    }

}

void GameObject::render() {
    //SDL_RenderCopy(renderer, texture, NULL, &destR);
    //std::cout << "rendering \n";
    if(show) SDL_RenderCopyEx(renderer, texture, NULL, &destR, degrees, NULL, SDL_FLIP_NONE);
}

void GameObject::setSpeedX(double uX) {
    this->speedX = uX;
}

void GameObject::setSpeedY(double uY) {
    this->speedY = uY;
}

void GameObject::setRotation(double r) {
    this->rotation += r;
}

void GameObject::setShow(bool s) {
    this->show = s;
}

double GameObject::getSpeedX() {
    return this->speedX;
}

double GameObject::getSpeedY() {
    return this->speedY;
}

double GameObject::getRotation() {
    return this->rotation;
}

bool GameObject::isShowing() {
    return this->show;
}

double GameObject::getX() {
    return x;
}

double GameObject::getY() {
    return y;
}

void GameObject::setX(double x) {
    this->x = x;
}

void GameObject::setY(double y) {
    this->y = y;
}

SDL_Rect GameObject::getRect() {
    return this->destR;
}

bool GameObject::isSimpleTexture() {
    return this->simpleTexture;
}

void GameObject::setSimpleTexture(bool s) {
    this->simpleTexture = s;
}

void GameObject::setDimensions(int w, int h) {
    width = w;
    height = h;
}

void GameObject::setTexture(const char* textureSheet) {
    this->texture = TextureManager::LoadTexture(textureSheet, renderer);
}

