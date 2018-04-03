#include "App.h"

void App::OnCleanup(){

	//Destroy window
	SDL_DestroyWindow( window );
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
