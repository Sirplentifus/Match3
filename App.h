#ifndef APP_H
#define APP_H

#include <SDL.h>
//#include <SDL_image.h> //Not using images for now
#include <stdio.h>

#include "CEvent.h"
//#include "LTexture.h" //Not using images for now
#include "color.h"
#include "Board.h"

class App : public CEvent {
    public:
        App();
        virtual ~App();

        Uint32 prev_frame_time; //ms

        int screen_width;
        int screen_height;
        int screen_margin = 30;

        bool Running;
        SDL_Window* window;
        SDL_Surface* screenSurface;
        SDL_Renderer* renderer;

        hsv hsv_back_color = {0,1,1};
        rgb rgb_back_color;

        //Board
        Board board;

        //States
        bool active;

    public:

        int OnExecute();

        bool OnInit();
        // Not using images
        #ifdef _SDL_IMAGE_H
        SDL_Surface* loadSurface( char* path );
        SDL_Texture* loadTexture( char* path );
        #endif

        //void OnEvent (SDL_Event* Event);
        void OnKeyDown (SDL_Keycode sym, Uint16 mod, SDL_Scancode scancode);
        void OnResize(int w,int h);
        void OnLButtonDown(int mX, int mY);
        void OnLButtonUp(int mX, int mY);

        void OnExit ();

        void OnLoop();

        void OnRender();

        void OnCleanup();

    protected:
    private:
};

#endif // APP_H
