#include "King.h"
#include "Board.h"
#include <cmath>

bool King::isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const {
    int dr = tr - fr;
    int dc = tc - fc;
    if (dr >= -1 && dr <= 1 && dc >= -1 && dc <= 1) {
        ChessPiece* dest = board.getPiece(tr, tc);
        if (dest && dest->getColor() == color) return false;
        return true;
    }
    if (!hasMoved() && dr == 0 && abs(dc) == 2) {
            int rookCol = (dc == 2) ? 7 : 0;
            ChessPiece* rook = board.getPiece(fr, rookCol);

            if (!rook || rook->getType() != ROOK || rook->getColor() != color || rook->hasMoved())
                return false;

            int step = (dc > 0) ? 1 : -1;
            for (int c = fc + step; c != rookCol; c += step) {
                if (board.getPiece(fr, c)) return false;
            }

            for (int c = fc; c != tc + step; c += step) {
                if (board.isSquareAttacked(fr, c, color == WHITE ? BLACK : WHITE)) return false;
            }

            return true;
        }
    return false;
}
