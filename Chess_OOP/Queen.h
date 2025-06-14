#pragma once
#include "ChessPiece.h"

class Queen : public ChessPiece {
public:
    Queen(Color c) : ChessPiece(c,QUEEN, (c == WHITE ? "♕" : "♛")) {}

    bool isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const;
};
