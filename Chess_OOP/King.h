#pragma once
#include "ChessPiece.h"

class King : public ChessPiece {
public:
    King(Color c) : ChessPiece(c, KING,(c == WHITE ? "♔" : "♚")) {}

    bool isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const;
};
