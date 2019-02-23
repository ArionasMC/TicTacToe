#ifndef MYBUTTON_H
#define MYBUTTON_H

#include "GameObject.h"
#include <SDL_mixer.h>

class MyButton : public GameObject
{
    public:
        MyButton(const char* textureSheet, SDL_Renderer* ren, int x, int y, int w, int h);
        bool isTriggered(int mouseX, int mouseY);
        void update(const char* normal, const char* pushed);
        void setPlayTriggerSound(bool p);
    private:
        bool pushedState = false;
        bool changed = false;
        Mix_Chunk* triggerSound;
        bool playTriggerSound = true;
};

#endif // MYBUTTON_H
