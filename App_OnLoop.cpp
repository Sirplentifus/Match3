#include "App.h"

void App::OnLoop(){

    Uint32 cur_frame_time = SDL_GetTicks();
    int SleepTime = 30 - (cur_frame_time-prev_frame_time);
    if(SleepTime > 0)
        SDL_Delay( SleepTime );

    hsv_back_color.h += 1;

    if(hsv_back_color.h > 360)
        hsv_back_color.h -= 360;

    rgb_back_color = hsv2rgb(hsv_back_color);

    board.OnLoop();
}
