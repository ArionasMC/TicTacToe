#include "Game.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject
{
    public:
        GameObject(const char* textureSheet, SDL_Renderer* ren, double x, double y);
        //~GameObject();

        void update();
        void render();
        void setSpeedX(double sX);
        void setSpeedY(double sY);
        void setShow(bool s);
        double getSpeedX();
        double getSpeedY();
        bool isShowing();
        void setRotation(double r);
        double getRotation();
        double getX();
        double getY();
        void setX(double x);
        void setY(double y);
        SDL_Rect getRect();
        bool isSimpleTexture();
        void setSimpleTexture(bool simpleTexture);
        void setDimensions(int w, int h);
        void setTexture(const char* textureSheet);

    protected:
        double x = 0.0;
        double y = 0.0;
        double speedX = 0.0;
        double speedY = 0.0;
        bool show = false;
        bool simpleTexture = false;
        double degrees = 0.0;
        double rotation = 0.0;
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        SDL_Rect srcR, destR;
        int width = 32;
        int height = 32;
};

#endif // GAMEOBJECT_H
