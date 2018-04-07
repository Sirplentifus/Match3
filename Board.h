#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <list>
#include <utility>

#include "LTexture.h"
#include "primitives\SDL2_gfxPrimitives.h"

#include "Piece.h"
#include "Animation.h"

using std::list;
using std::pair; using std::make_pair;

class Board
{
    public:
        Board();
        virtual ~Board();

        void setPosSize(int nX, int nY, int nW, int nH);

        void OnInit(SDL_Renderer* nRenderer, int nNumColumns=-1, int nNumRows=-1);
        void OnRender();
        //void RenderAnimation(Animation anim);
        void OnLoop();
        void OnLButtonDown(int mX, int mY);
        void OnLButtonUp(int mX, int mY);


    private:
        //mutable
        //LTexture triangle, square;
        int x, y; //Position on screen
        int W, H; //Window Size

        int tileSize, tileMargin, pieceRadius; //Board object dimensions
        float tileMarginFraction, tilePieceFraction;

        //GPU Textures
        SDL_Texture *background, *piece_mold;

        //Non mutable
        int numColumns, numRows;
        SDL_Renderer* renderer;
        Uint32 start_time;
        Uint32 pieceColors[N_PIECES];
        void RenderPiece(piece_t piece, int xp, int yp);

        //The thing in itself
        piece_t *actualBoard;

        list<Animation> animations;

        //Render helpers
        void RenderAnimation(Animation& anim);
        void RenderMatch(Animation& anim);
        void RenderDrop(Animation& anim);

        //Loop helpers
        void ProcessMatches();
        list< pair<int,int> > RegionGrowth(pair<int,int> seed);
        Animation CreateMatchAnim(list< pair<int,int> >& members, Uint32 color);
        void ProcessDrops();

        //Piece moving faciliators
        //onto unused for now. idea was to use it to make the piece the user is about to drop onto,
        //move slightly towards the selected piece. But this is complicated and sublte, so was not implemented
        //TODO: remove ontoPieceInd everywhere.
        int selectedPieceInd, ontoPieceInd;
        bool match_on_point(int i, int j);
};

#endif // BOARD_H
