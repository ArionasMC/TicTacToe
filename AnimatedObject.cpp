#include "AnimatedObject.h"
using namespace std;

AnimatedObject::AnimatedObject(const char* textureSheet, SDL_Renderer* ren, double x, double y, vector<SDL_Rect> frameRects, int endFrame, int secondsToNext) : GameObject(textureSheet, ren, x, y)
{
    this->show = true;
    this->setSimpleTexture(true);
    this->frameRects = frameRects;
    this->endFrame = endFrame;
    this->secondsToNext = secondsToNext;
    this->maxFrames = frameRects.size();
    this->playSoundEffect = false;
    /*
    this->destR.w = 75;
    this->destR.h = 75;
    this->srcR.x = this->frame * 50 ;
    this->srcR.y = 0;
    this->srcR.w = 49;
    this->srcR.h = 50;
    */
}

void AnimatedObject::update() {
    GameObject::update();
    if(this->show) {
        this->destR.x = this->x;
        this->destR.y = this->y;
        this->destR.w = this->width;
        this->destR.h = this->height;
        this->srcR.x = frameRects[frame].x;
        this->srcR.y = frameRects[frame].y;
        this->srcR.w = frameRects[frame].w;
        this->srcR.h = frameRects[frame].h;
        if(!animated) timer++;
    }
}

void AnimatedObject::render() {
    if(show) SDL_RenderCopyEx(renderer, texture, &srcR, &destR, degrees, NULL, SDL_FLIP_NONE);
}

void AnimatedObject::playAnimation() {
    if(!animated) {
        if(playSoundEffect) {
            Mix_PlayChannel(-1, soundEffect, 0);
            playSoundEffect = false;
        }
        if(frame < maxFrames) {
            if(timer >= secondsToNext) {
                timer = 0;
                frame++;
                //cout << "heyy";
                //this->srcR.x = this->frame * 50;
            }
            if(frame == maxFrames) {
                animated = true;
                //cout << "hey!";
                frame = endFrame;
            }
        }
        //cout << frame << '\n';
    }

}

void AnimatedObject::restartAnimation() {
    this->frame = 0;
    animated = false;
    playAnimation();
}

void AnimatedObject::generateFrameRects(int times, int deltaX, int deltaY, int conW, int conH) {
    for(int i = 0; i < times; i++) {
        frameRects.push_back({i*deltaX, i*deltaY, conW, conH});
        //cout << i*deltaX <<", "<<i*deltaY<<", "<<conW<<", "<<conH<<'\n';
    }
    this->maxFrames = frameRects.size();
}

bool AnimatedObject::getAnimated() {
    return this->animated;
}

void AnimatedObject::setAnimated(bool a) {
    this->animated = a;
}

void AnimatedObject::setPlaySoundEffect(bool p) {
    this->playSoundEffect = p;
}

void AnimatedObject::setSoundEffectSource(string source) {
    this->soundEffect = Mix_LoadWAV(source.c_str());
}
