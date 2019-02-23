#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H

#include <GameObject.h>
#include <vector>
#include <SDL_mixer.h>
using namespace std;

class AnimatedObject : public GameObject
{
    public:
        AnimatedObject(const char* textureSheet, SDL_Renderer* ren, double x, double y, vector<SDL_Rect> frameRects, int endFrame, int secondsToNext);
        void playAnimation();
        void render();
        void update();
        void restartAnimation();
        void generateFrameRects(int times, int deltaX, int deltaY, int conW, int conH);
        bool getAnimated();
        void setAnimated(bool a);
        void setPlaySoundEffect(bool p);
        void setSoundEffectSource(string source);

    private:
        int frame = 0;
        int timer = 0;
        int animated = false;
        vector<SDL_Rect> frameRects;
        int maxFrames = 0;
        int endFrame = 0;
        int secondsToNext = 0;
        Mix_Chunk* soundEffect;
        bool playSoundEffect = false;

};

#endif // ANIMATEDOBJECT_H
