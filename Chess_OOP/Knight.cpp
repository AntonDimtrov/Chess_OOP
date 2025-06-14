#include "Knight.h"
#include "Board.h"

bool Knight::isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const {
    int dr = tr - fr;
    int dc = tc - fc;
    if (!((dr * dr == 1 && dc * dc == 4) || (dr * dr == 4 && dc * dc == 1)))
        return false;

    ChessPiece* dest = board.getPiece(tr, tc);
    if (dest && dest->getColor() == color)
        return false;

    return true;
}
