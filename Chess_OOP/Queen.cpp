#include "Queen.h"
#include "Board.h"
#include <cmath>
bool Queen::isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const {
    if (fr == tr || fc == tc || abs(fr - tr) == abs(fc - tc)) {
        if (!board.isPathClear(fr, fc, tr, tc)) return false;

        ChessPiece* dest = board.getPiece(tr, tc);
        if (dest && dest->getColor() == color) return false;

        return true;
    }
    return false;
}
