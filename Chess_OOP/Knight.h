#pragma once
#include "ChessPiece.h"

class Knight : public ChessPiece {
public:
    Knight(Color c) : ChessPiece(c,KNIGHT, (c == WHITE ? "♘" : "♞")) {}

    bool isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const;
};
