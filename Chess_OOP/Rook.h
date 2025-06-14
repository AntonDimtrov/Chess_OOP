#pragma once
#include "ChessPiece.h"

class Rook : public ChessPiece  {
public:
    Rook(Color c) : ChessPiece(c, ROOK, (c == WHITE ? "♖" : "♜")) {}

    bool isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const;
};
