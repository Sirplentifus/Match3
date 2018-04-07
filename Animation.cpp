#include "Animation.h"

#include "Board.h"

Animation::Animation()
{
    start_time = SDL_GetTicks();
    anim_type = INVALID;
    finished = false;
}

bool animationIsFinished(Animation& a){
    return a.finished;
}

void Board::RenderAnimation(Animation& anim){
    switch(anim.anim_type){
        case Animation::MATCH:
            RenderMatch(anim);
            break;
        case Animation::DROP:
            RenderDrop(anim);
            break;
        default:
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "invalid animation type: %d", anim.anim_type);
    }
}

void Board::RenderMatch(Animation& anim){
    MatchAnim matchAnim = anim.matchAnim;

    Uint32 delta_t = SDL_GetTicks()-anim.start_time;
    float anim_fraction = (float)delta_t/(float)matchAnim.fade_time;
    if(anim_fraction>=1){
        anim_fraction = 1;
        anim.finished = true;

        //Is this really a task for the renderer?
        delete anim.matchAnim.is; delete anim.matchAnim.js;
        return;
    }

    for(int k=0; k<matchAnim.numPieces; k++){
        int i = matchAnim.is[k], j = matchAnim.js[k];
        RenderPiece(matchAnim.piece_type, x+tileSize*i, y+tileSize*j, 1.0-anim_fraction);
    }
}

const float DropAnim::drop_speed = 5.0f/1000.0f; //tiles per millisecond
void Board::RenderDrop(Animation& anim){
    int i = anim.dropAnim.i;
    float j = anim.dropAnim.start_j + anim.dropAnim.drop_speed*(SDL_GetTicks()-anim.start_time);
    if(j>anim.dropAnim.target_j){
        j=anim.dropAnim.target_j;
        anim.finished=true;
        if(actualBoard[i*numRows+anim.dropAnim.target_j]!=EMPTY)
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Piece dropping mechanism attempted to overwrite a piece\n");
        actualBoard[i*numRows+anim.dropAnim.target_j] = anim.dropAnim.piece_type; //Is this really a task for the renderer?
    }
    RenderPiece(anim.dropAnim.piece_type, x+tileSize*i, y+tileSize*j);
}

void Board::ProcessMatches(){
    //add the initial three, and then region grow
    //(Not actually adding the three, only one as a seed)
    //Animation newMatchAnim;

    //Find vertical threes
    for(int i=0; i<numColumns; i++){
        for(int j=0; j<numRows-2; j++){
            if(actualBoard[i*numRows+j]!=EMPTY &&
               actualBoard[i*numRows+j]==actualBoard[i*numRows+j+1] &&
               actualBoard[i*numRows+j]==actualBoard[i*numRows+j+2]){
                piece_t piece_type = actualBoard[i*numRows+j];
                auto Region = RegionGrowth(make_pair(i,j+1));
                animations.push_back(CreateMatchAnim(Region, piece_type));
            }
        }
    }
    //Find horizontal threes
    for(int j=0; j<numRows; j++){
        for(int i=0; i<numColumns-2; i++){
            if(actualBoard[i*numRows+j]!=EMPTY &&
               actualBoard[i*numRows+j]==actualBoard[(i+1)*numRows+j] &&
               actualBoard[i*numRows+j]==actualBoard[(i+2)*numRows+j]){
                piece_t piece_type = actualBoard[i*numRows+j];
                auto Region = RegionGrowth(make_pair(i+1,j));
                animations.push_back(CreateMatchAnim(Region, piece_type));
            }
        }
    }
}

list< pair<int,int> > Board::RegionGrowth(pair<int,int> seed){
    piece_t match_piece = actualBoard[seed.first*numRows + seed.second];
    if(match_piece==EMPTY)
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "RegionGrowth can't run on an empty seed");
    list< pair<int,int> > explored, new_frontier, frontier;
    frontier.push_back(seed);
    pair<int,int> nb[] = {make_pair(1,0), make_pair(-1,0), make_pair(0,1), make_pair(0,-1)}; //neighbourhood
    bool doItAgain;
    do{
        doItAgain=false;

        //mark frontier as EMPTY to avoid repeats
        for(auto it=frontier.begin(); it!=frontier.end(); it++){
            actualBoard[it->first*numRows + it->second] = EMPTY;
        }

        //expand, making new_frontier
        for(auto it=frontier.begin(); it!=frontier.end(); it++){
            for(int k=0; k<4; k++){
                int i = it->first+nb[k].first;
                int j = it->second+nb[k].second;
                if(i>=numColumns || j>=numRows || i<0 || j<0)
                    continue;
                if( actualBoard[i*numRows + j] == match_piece ){
                    new_frontier.push_back( make_pair(i, j) );
                    doItAgain = true;
                    //board activity overloads user. user should have been faster
                    if(i*numRows+j==selectedPieceInd){
                        selectedPieceInd = -1; ontoPieceInd = -1;
                    }
                }
            }
        }

        //add previous frontier to explored, swap new_frontier with frontier, and clear new_frontier

        explored.splice(explored.begin(), frontier);
        std::swap(frontier, new_frontier);
        new_frontier.clear(); //Unnecessary. splice already should've cleared frontier

    }while(doItAgain);

    return explored;
}

Animation Board::CreateMatchAnim(list< pair<int,int> >& members, piece_t piece_type){
    Animation newMatchAnim;
    newMatchAnim.matchAnim.numPieces=members.size();
    newMatchAnim.matchAnim.piece_type=piece_type;
    newMatchAnim.anim_type=Animation::MATCH;

    newMatchAnim.matchAnim.is = new int[newMatchAnim.matchAnim.numPieces];
    newMatchAnim.matchAnim.js = new int[newMatchAnim.matchAnim.numPieces];

    int k=0;
    for(auto it=members.begin(); it!=members.end(); it++){
        newMatchAnim.matchAnim.is[k] = it->first;
        newMatchAnim.matchAnim.js[k] = it->second;
        k++;
    }

    return newMatchAnim;
}

void Board::ProcessDrops(){
    int holes;
    for(int i=0; i<numColumns; i++){
        holes = 0;
        //Dropping current pieces
        for(int j=numRows-1; j>=0; j--){
            if(actualBoard[i*numRows+j] == EMPTY){
                holes++;
            } else {
                if(holes==0)
                    continue;
                //the juice!
                Animation newAnimation;
                newAnimation.anim_type = Animation::DROP;
                newAnimation.dropAnim.i=i;
                newAnimation.dropAnim.start_j=j;
                newAnimation.dropAnim.target_j=j+holes;
                newAnimation.dropAnim.piece_type = actualBoard[i*numRows+j];
                animations.push_back(newAnimation);
                actualBoard[i*numRows+j] = EMPTY;

                //board activity overloads user. user should have been faster
                if(i*numRows+j==selectedPieceInd){
                    selectedPieceInd = -1; ontoPieceInd = -1;
                }
            }
        }
        //Generating new pieces
        for(int h=holes; h>0; h--){
            Animation newAnimation;
            newAnimation.anim_type = Animation::DROP;
            newAnimation.dropAnim.i=i;
            newAnimation.dropAnim.start_j=-h;
            newAnimation.dropAnim.target_j=-h+holes;
            newAnimation.dropAnim.piece_type = (piece_t)(rand()%N_PIECES);
            animations.push_back(newAnimation);
        }
    }
}
