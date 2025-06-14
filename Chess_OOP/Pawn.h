#pragma once
#include "ChessPiece.h"

class Pawn : public ChessPiece {
public:
    Pawn(Color c) : ChessPiece(c, PAWN,(c == WHITE ?  "♙" : "♟")) {}

    bool isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const;
};
