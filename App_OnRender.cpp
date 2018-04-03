#include "App.h"
#include "primitives/SDL2_gfxPrimitives.h"


void App::OnRender()
{
    //Fill the surface with background color
    //Desat blue: 0x33, 0x80, 0xA0
    SDL_SetRenderDrawColor( renderer, 0x33, 0x80, 0xA0, 0xFF );
    //0xFF*rgb_back_color.r, 0xFF*rgb_back_color.g, 0xFF*rgb_back_color.b, 0xFF );
    SDL_RenderClear( renderer );

    board.OnRender();

    SDL_RenderPresent( renderer );

}
