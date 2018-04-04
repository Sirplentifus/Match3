#include "App.h"

bool App::OnInit(){

    prev_frame_time = SDL_GetTicks();

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}

    //Create window
    window = SDL_CreateWindow( "Match 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE  );
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    SDL_SetWindowMinimumSize(window, 175, 200);

    /* Not using images
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ){
            printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
            return false;
    }*/

    //Get window surface
    screenSurface = SDL_GetWindowSurface( window );

    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    board.OnInit(renderer, 8, 12);
    board.setPosSize(screen_margin, screen_margin, screen_width-2*screen_margin, screen_height-2*screen_margin);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    printf("SDL_HINT_RENDER_SCALE_QUALITY: %s\n", SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY));

    return true;
}

// Not using images
#ifdef _SDL_IMAGE_H
SDL_Surface* App::loadSurface( char* path )
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path);

    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError() );
        return NULL;
    }

    //Convert surface to screen format
    optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, 0x0 );
    if( optimizedSurface == NULL )
    {
        printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        return NULL;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );

    return optimizedSurface;
}

SDL_Texture* App::loadTexture( char* path ){
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
		return NULL;
	}

    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if( newTexture == NULL )
    {
        printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );


	return newTexture;
}
#endif
