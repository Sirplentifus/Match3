#include "LTexture.h"

LTexture::LTexture(){
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	renderer = NULL;
}

LTexture::~LTexture(){
	//Deallocate
	OnCleanup();
}

void LTexture::setRenderer(SDL_Renderer* nRenderer){
    renderer = nRenderer;
}

#ifdef _SDL_IMAGE_H
bool LTexture::loadFromFile( char* path, bool colorkey ){
	//Get rid of preexisting texture
	OnCleanup();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL ){
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
		return false;
	}

    //Color key image
    if(colorkey){
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
    }

    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if( newTexture == NULL ){
        printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        return false;
    }

    //Get image dimensions
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;


    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );


	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::OnInit( SDL_Renderer* nRenderer, char* path, bool colorkey){
    setRenderer(nRenderer);
    loadFromFile( path, colorkey );

    //Always turning on alpha blending
    SDL_SetTextureBlendMode( mTexture, SDL_BLENDMODE_BLEND );
}
#endif

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ){
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::OnCleanup()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::OnRender( int x, int y, int width, int height, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    if(width<0){
        if(clip!=NULL){
            width = clip->w;
        }else{
            width = mWidth;
        }
    }

    if(height<0){
        if(clip!=NULL){
            height = clip->h;
        }else{
            height = mHeight;
        }
    }

	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


