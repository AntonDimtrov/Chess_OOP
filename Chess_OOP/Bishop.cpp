#include "Bishop.h"
#include "Board.h"
#include <cmath>

bool Bishop::isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const {
    if (abs(fr - tr) != abs(fc - tc)) return false;

    if (!board.isPathClear(fr, fc, tr, tc)) return false;

    ChessPiece* dest = board.getPiece(tr, tc);
    if (dest && dest->getColor() == color) return false;

    return true;
}
