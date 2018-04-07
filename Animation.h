#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include <vector>

#include "Piece.h"

struct DropAnim{
    public:
        static const float drop_speed; //definition moved to .cpp due to compiler issues

        int i, start_j, target_j;
        piece_t piece_type;
};

struct MatchAnim{
    public:
        static const Uint32 fade_time = 400; //milliseconds

        int numPieces;
        int *is, *js;
        Uint32 color; //TODO: replace with piece_t for uniformity?
};

struct SwitchAnim{
    public:
        static const Uint32 switch_time = 200; //milliseconds
        float start_i, start_j; //coords of selected piece, to ensure continuity of movement.
        int i,j; //original coords of the clicked piece
        int delta_i, delta_j; //relative position we're switching with. One must be -1 or 1, and the other must be 0.
        piece_t selected, onto;
};

class Animation{
    public:
        Animation();

        enum anim_type_t {INVALID, MATCH, DROP, SWITCH} anim_type;
        union{
            DropAnim dropAnim;
            MatchAnim matchAnim;
            SwitchAnim switchAnim;
        };

        bool finished;

        Uint32 start_time;
};

bool animationIsFinished(Animation &a);

#endif // ANIMATION_H
