#include "App.h"

void App::OnExit (){
    Running = false;
}

void App::OnKeyDown (SDL_Keycode sym, Uint16 mod, SDL_Scancode scancode){
    switch( sym ){
    case SDLK_a:
        active = !active;
        break;
    default:
        break;
    }
}

void App::OnResize(int w,int h){

    SDL_Log("Resize to %d %d\n", w, h);

    screen_width = w;
    screen_height = h;

    board.setPosSize(screen_margin, screen_margin, screen_width-2*screen_margin, screen_height-2*screen_margin);
}

void App::OnLButtonDown(int mX, int mY){
    board.OnLButtonDown(mX, mY);
}

void App::OnLButtonUp(int mX, int mY){
    board.OnLButtonUp(mX, mY);
}

void App::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle){
    board.OnMouseMove(mX, mY, relX, relY, Left,Right,Middle);
}

