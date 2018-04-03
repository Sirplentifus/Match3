#ifndef LTEXTURE_H_INCLUDED
#define LTEXTURE_H_INCLUDED


#include <SDL.h>
//#include <SDL_image.h> //Not using images for now

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Initializations
		void setRenderer(SDL_Renderer* nRenderer);
		#ifdef _SDL_IMAGE_H
		bool loadFromFile( char* path, bool colorkey=false);
        bool OnInit( SDL_Renderer* nRenderer, char* path, bool colorkey=false);
        #endif
        bool OnInit(SDL_Renderer* nRenderer); //Creates a targetable texture

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Deallocates texture
		void OnCleanup();

		//Renders texture at given point
		void OnRender( int x, int y, int width=-1, int height=-1, SDL_Rect* clip=NULL, double angle=0.0, SDL_Point* center=NULL, SDL_RendererFlip flip=SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;

		//Renderer for this texture
		SDL_Renderer* renderer;
};


#endif // LTEXTURE_H_INCLUDED
