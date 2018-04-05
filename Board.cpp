#include "Board.h"

Board::Board()
{
    //ctor
    renderer = NULL;
    numColumns = -1;
    numRows = -1;
    selectedPieceInd = -1, ontoPieceInd=-1;
    background=NULL;
    piece_mold=NULL;
}

Board::~Board()
{
    //dtor
}

void Board::setPosSize(int nX, int nY, int nW, int nH){
    W = nW; H = nH;
    tileSize = std::min(nW/numColumns, nH/numRows); SDL_Log("Tile size: %d\n", tileSize);
    tileMargin = (int)((float)tileSize*tileMarginFraction); SDL_Log("Tile margin: %d\n", tileMargin);
    pieceRadius = (int)((float)tileSize*tilePieceFraction/2.0); SDL_Log("Piece Radius: %d\n", pieceRadius);

    x = nX + (W - tileSize*numColumns)/2;
    y = nY + (H - tileSize*numRows)/2;
    W = numColumns*tileSize;
    H = numRows*tileSize;

    //Redrawing the textures for the current size.
    //Although textures can be easily scaled, this feels more optimal - no waste in space nor lack of resolution
    //Also, this function isn't called often

    SDL_DestroyTexture(piece_mold);
    piece_mold = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tileSize, tileSize);
    SDL_SetTextureBlendMode( piece_mold, SDL_BLENDMODE_BLEND );
    SDL_SetRenderTarget(renderer, piece_mold);
    SDL_SetRenderDrawColor(renderer, 0x33, 0x80, 0xA0, 0x00);
    SDL_RenderClear(renderer);
    aafilledCircleColor(renderer, tileSize/2, tileSize/2, pieceRadius, 0xffffffff);

    SDL_DestroyTexture(background);
    SDL_SetRenderTarget(renderer, background);

    //Making the target back to the window
    SDL_SetRenderTarget(renderer, NULL);


}

void Board::OnInit(SDL_Renderer* nRenderer, int nNumColumns, int nNumRows){
    renderer = nRenderer;
    start_time = SDL_GetTicks();
    /*triangle.OnInit(renderer, "gfx/triangle.png");
    square.OnInit(renderer, "gfx/square.png");*/

    numColumns = nNumColumns;
    numRows = nNumRows;
    actualBoard = new piece_t[numColumns*numRows];
    srand(0);
    for(int i=0; i<numColumns*numRows; i++)
        actualBoard[i] = (piece_t)(rand()%N_PIECES);

}

void Board::OnRender(){

    //rectangleColor(renderer, x, y, x+W, y+H, 0xffffffff); //Bounding box

    //Print Board and pieces on it
    int top = y;
    for(int i=0; i<numColumns; i++){
        for(int j=0; j<numRows; j++){
            boxColor(renderer, x+tileSize*i+tileMargin, y+tileSize*j+tileMargin,
                     x+tileSize*(i+1)-tileMargin, y+tileSize*(j+1)-tileMargin, 0xa0dddddd);//0xa030d0d0);
            int piece_ind = i*numRows+j;
            piece_t piece = actualBoard[piece_ind];
            if(piece != EMPTY && piece_ind!=selectedPieceInd && piece_ind!=ontoPieceInd){
                RenderPiece(piece, x+tileSize*i, y+tileSize*j);
            }
        }
    }

    for(auto it=animations.begin(); it!=animations.end(); it++){
        RenderAnimation(*it);
    }

    //Print selected piece
    if(selectedPieceInd>=0){
        int mX, mY;
        SDL_GetMouseState(&mX, &mY);
        aafilledCircleColor(renderer, mX, mY, pieceRadius, pieceColors[actualBoard[selectedPieceInd]]);
    }
}

void Board::RenderPiece(piece_t piece, int xp, int yp){
    Uint32 Color = pieceColors[piece];
    Uint8 r=Color&0xff, g=(Color&0xff00)>>8, b=(Color&0xff0000)>>16;
    SDL_SetTextureColorMod( piece_mold, r, g, b);
    SDL_Rect renderQuad = { xp, yp, tileSize, tileSize };
    SDL_RenderCopy(renderer, piece_mold, NULL, &renderQuad);
}

void Board::OnLoop(){

    if(SDL_GetTicks()-start_time > 1000){
        animations.remove_if(animationIsFinished);
        if(animations.empty())
            ProcessMatches();
        if(animations.empty())
            ProcessDrops();
    }
    //SDL_Log("Size of animation list: %d\n", animations.size());
}

//TODO: If a selected piece becomes part of a match, it needs to be unselected!
void Board::OnLButtonDown(int mX, int mY){
    if(mX<x || mX>x+W || mY<y || mY>y+H)
        return;
    int i,j;
    i = (mX-x)/tileSize;
    j = (mY-y)/tileSize;
    if(actualBoard[i*numRows+j] == EMPTY)
        return;
    //actualBoard[i*numRows+j] = (piece_t)(((int)actualBoard[i*numRows+j]+1)%N_PIECES);
    selectedPieceInd = i*numRows+j;
}

void Board::OnLButtonUp(int mX, int mY){

    int i,j;
    i = (mX-x)/tileSize;
    j = (mY-y)/tileSize;
    int ontoCandidateInd = i*numRows+j;
    int diff = selectedPieceInd-ontoCandidateInd;

    if( (diff==1||diff==-1||diff==numRows||diff==-numRows) && actualBoard[ontoCandidateInd]!=EMPTY
       && actualBoard[ontoCandidateInd]!=actualBoard[selectedPieceInd]){

        //selected becomes part of a match?
        //if( actualBoard[selectedPieceInd]== )

        std::swap(actualBoard[selectedPieceInd], actualBoard[ontoCandidateInd]);

        //verify a match gets created
        if(!match_on_point(selectedPieceInd/numRows, selectedPieceInd%numRows) &&
           !match_on_point(ontoCandidateInd/numRows, ontoCandidateInd%numRows)){
            //swap back
            std::swap(actualBoard[selectedPieceInd], actualBoard[ontoCandidateInd]);
        }

    }

    selectedPieceInd = -1, ontoPieceInd=-1;
}

bool Board::match_on_point(int i, int j){
    int count=0;

    //horizontal
    for(int k=1; k<=2; k++){
        if(j+k >= numRows)
            break;
        if(actualBoard[i*numRows+j+k] == actualBoard[i*numRows+j])
            count++;
        else
            break;
    }
    for(int k=1; k<=2; k++){
        if(j-k < 0)
            break;
        if(actualBoard[i*numRows+j-k] == actualBoard[i*numRows+j])
            count++;
        else
            break;
    }
    if(count>=2)
        return true;

    //vertical
    count = 0;
    for(int k=1; k<=2; k++){
        if(i+k >= numColumns)
            break;
        if(actualBoard[(i+k)*numRows+j] == actualBoard[i*numRows+j])
            count++;
        else
            break;
    }
    for(int k=1; k<=2; k++){
        if(i-k < 0)
            break;
        if(actualBoard[(i-k)*numRows+j] == actualBoard[i*numRows+j])
            count++;
        else
            break;
    }
    if(count>=2)
        return true;

    return false;
}

//Not actually necessary?
void Board::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle){

}
