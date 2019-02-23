#ifndef ANIMATEDTEXT_H
#define ANIMATEDTEXT_H

#include <MyText.h>
#include <SDL.h>
#include <string>
using namespace std;

class AnimatedText
{
    public:
        AnimatedText(MyText* text, SDL_Renderer* renderer, int maxDelay, int startingDelay);
        void update(SDL_Color c);
        void render();

    private:
        SDL_Renderer* renderer;
        int delay;
        int startingDelay;
        int maxDelay;
        MyText* text;
        int currentLetter = 0;
        string t;
};

#endif // ANIMATEDTEXT_H
