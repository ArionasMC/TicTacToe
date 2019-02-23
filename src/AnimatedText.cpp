#include "AnimatedText.h"

AnimatedText::AnimatedText(MyText* text, SDL_Renderer* renderer, int maxDelay, int startingDelay)
{
    this->text = text;
    this->renderer = renderer;
    this->maxDelay = maxDelay;
    this->delay = 0;
    this->t = text->getTextInString();
    this->text->setText("");
    this->text->setColor(text->getColor());
    this->startingDelay = startingDelay;
}

void AnimatedText::update(SDL_Color c) {
    if(startingDelay > 0)
        startingDelay--;
    else {
        delay--;
        if(delay <= 0) {
            delay = maxDelay;

            if(currentLetter < t.length()-1) currentLetter++;

            text->setText("");
            string newText;
            for(int i = 0; i <= currentLetter; i++) {
                newText+=t[i];
            }

            text->setText(newText.c_str());
            text->setColor(c);

        }
    }
}

void AnimatedText::render() {
    if(startingDelay<=0) SDL_RenderCopy(renderer, text->getText(), NULL, text->getTextRect());
}
