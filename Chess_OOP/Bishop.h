#pragma once
#include "ChessPiece.h"

class Bishop : public ChessPiece {
public:
    Bishop(Color c) : ChessPiece(c,BISHOP, (c == WHITE ? "♗" : "♝")) {}

    bool isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const;
};
