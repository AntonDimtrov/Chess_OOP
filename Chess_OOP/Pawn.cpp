#include "Pawn.h"
#include "Board.h"
#include <cmath>

bool Pawn::isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const {
    int dir = (color == WHITE) ? -1 : 1;
    int startRow = (color == WHITE) ? 6 : 1;

    if (fc == tc) {
        if (tr == fr + dir && board.getPiece(tr, tc) == 0)
            return true;
        if (fr == startRow && tr == fr + 2 * dir &&
            board.getPiece(fr + dir, fc) == 0 &&
            board.getPiece(tr, tc) == 0)
            return true;
    } else if (abs(fc - tc) == 1 && tr == fr + dir) {
        ChessPiece* target = board.getPiece(tr, tc);
        if (target && target->getColor() != color)
            return true;
        if (tr == board.getEnPassantRow() && tc == board.getEnPassantCol())
            return true;
    }

    return false;
}
