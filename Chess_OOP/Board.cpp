#include "Board.h"
#include <iostream>
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
using namespace std;

Board::Board() {
    enPassantRow = -1;
    enPassantCol = -1;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            squares[r][c] = nullptr;
}

Board::~Board() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (squares[r][c]) delete squares[r][c];
}

void Board::initialize() {
    for (int c = 0; c < 8; ++c) {
        squares[6][c] = new Pawn(WHITE);
        squares[1][c] = new Pawn(BLACK);
    }

    squares[7][0] = new Rook(WHITE);
    squares[7][7] = new Rook(WHITE);
    squares[0][0] = new Rook(BLACK);
    squares[0][7] = new Rook(BLACK);
    squares[7][1] = new Knight(WHITE);
    squares[7][6] = new Knight(WHITE);
    squares[0][1] = new Knight(BLACK);
    squares[0][6] = new Knight(BLACK);
    squares[7][2] = new Bishop(WHITE);
    squares[7][5] = new Bishop(WHITE);
    squares[0][2] = new Bishop(BLACK);
    squares[0][5] = new Bishop(BLACK);
    squares[7][3] = new Queen(WHITE);
    squares[0][3] = new Queen(BLACK);
    squares[7][4] = new King(WHITE);
    squares[0][4] = new King(BLACK);

    clearEnPassant();
}

void Board::print(Color turn) const {
    cout << "\n";
    
    for (int r = 0; r < 8; ++r) {
        int row = (turn == WHITE) ? r : (7 - r);
        cout << (8 - row) << "   ";
        for (int c = 0; c < 8; ++c) {
            int col = (turn == WHITE) ? c : (7 - c);
            ChessPiece* p = squares[row][col];
            if (p)
                cout << p->getSymbol() << " ";
            else
                cout << ". ";
        }
        cout << " " << "\n";
    }

    cout << "\n    ";
    for (int c = 0; c < 8; ++c) {
        char file = (turn == WHITE) ? ('a' + c) : ('h' - c);
        cout << file << " ";
    }
    cout << "\n\n";
}


ChessPiece* Board::getPiece(int r, int c) const {
    return squares[r][c];
}

void Board::setPiece(int r, int c, ChessPiece* piece) {
    squares[r][c] = piece;
}

void Board::setEnPassant(int row, int col) {
    enPassantRow = row;
    enPassantCol = col;
}

void Board::clearEnPassant() {
    enPassantRow = -1;
    enPassantCol = -1;
}

int Board::getEnPassantRow() const {
    return enPassantRow;
}

int Board::getEnPassantCol() const {
    return enPassantCol;
}

bool Board::isPathClear(int fr, int fc, int tr, int tc) const {
    int dr = (tr - fr) ? ((tr - fr) > 0 ? 1 : -1) : 0;
    int dc = (tc - fc) ? ((tc - fc) > 0 ? 1 : -1) : 0;
    fr += dr;
    fc += dc;
    while (fr != tr || fc != tc) {
        if (squares[fr][fc]) return false;
        fr += dr;
        fc += dc;
    }
    return true;
}

bool Board::isSquareAttacked(int row, int col, Color byColor) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            ChessPiece* p = squares[r][c];
            if (p && p->getColor() == byColor) {
                if (p->isMoveValid(r, c, row, col, *this)) return true;
            }
        }
    }
    return false;
}

bool Board::isInCheck(Color color) const {
    int kr = -1, kc = -1;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (squares[r][c] &&
                squares[r][c]->getType() == KING &&
                squares[r][c]->getColor() == color) {
                kr = r;
                kc = c;
            }
    if (kr == -1) return false;
    return isSquareAttacked(kr, kc, (color == WHITE ? BLACK : WHITE));
}

bool Board::hasLegalMoves(Color color) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            ChessPiece* piece = getPiece(r, c);
            if (!piece || piece->getColor() != color) continue;
            for (int tr = 0; tr < 8; ++tr) {
                for (int tc = 0; tc < 8; ++tc) {
                    if ((r == tr && c == tc) || (getPiece(tr, tc) && getPiece(tr, tc)->getColor() == color))
                        continue;
                    if (piece->isMoveValid(r, c, tr, tc, *this)) {
                        ChessPiece* captured = getPiece(tr, tc);
                        setPiece(tr, tc, piece);
                        setPiece(r, c, nullptr);
                        bool inCheck = isInCheck(color);
                        setPiece(r, c, piece);
                        setPiece(tr, tc, captured);
                        if (!inCheck) return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::movePiece(int fr, int fc, int tr, int tc, Color color) {
    ChessPiece* piece = getPiece(fr, fc);
    if (!piece || piece->getColor() != color) return false;
    if (!piece->isMoveValid(fr, fc, tr, tc, *this)) return false;

    if (piece->getType() == PAWN &&
            tr == getEnPassantRow() && tc == getEnPassantCol()) {
            int capRow = (color == WHITE) ? tr + 1 : tr - 1;
            delete getPiece(capRow, tc);
            setPiece(capRow, tc, nullptr);
        }
    
    ChessPiece* captured = getPiece(tr, tc);
    setPiece(tr, tc, piece);
    setPiece(fr, fc, nullptr);
    
    if (piece->getType() == KING && abs(tc - fc) == 2) {
        int rookFromCol = (tc > fc) ? 7 : 0;
        int rookToCol = (tc > fc) ? tc - 1 : tc + 1;
        ChessPiece* rook = getPiece(tr, rookFromCol);
        setPiece(tr, rookToCol, rook);
        setPiece(tr, rookFromCol, nullptr);
        rook->setMoved(true);
    }

    clearEnPassant();
    if (piece->getType() == PAWN &&
        abs(tr - fr) == 2) {
        int epRow = (fr + tr) / 2;
        setEnPassant(epRow, fc);
    }
    
    bool illegal = isInCheck(color);
    if (illegal) {
        setPiece(fr, fc, piece);
        setPiece(tr, tc, captured);
        return false;
    }

    if (captured) delete captured;
    piece->setMoved(true);
    
    
    if (piece->getType() == PAWN &&
        ((color == WHITE && tr == 0) || (color == BLACK && tr == 7))) {

        char choice;
        do {
            cout << "Promote pawn to (q)ueen, (r)ook, (b)ishop, or (k)night: ";
            cin >> choice;
            choice = tolower(choice);
        } while (choice != 'q' && choice != 'r' && choice != 'b' && choice != 'k');

        delete piece;

        switch (choice) {
            case 'r': setPiece(tr, tc, new Rook(color)); break;
            case 'b': setPiece(tr, tc, new Bishop(color)); break;
            case 'k': setPiece(tr, tc, new Knight(color)); break;
            default:  setPiece(tr, tc, new Queen(color)); break;
        }

        cin.ignore();
    }
    
    return true;
}
bool Board::isInsufficientMaterial() const {
    int nonKingCount = 0;
    
    struct Info {
        PieceType type;
        Color color;
        int row, col;
    };

    Info pieces[4];

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            ChessPiece* p = squares[r][c];
            if (!p || p->getType() == KING) continue;

            if (nonKingCount < 4) {
                pieces[nonKingCount].type = p->getType();
                pieces[nonKingCount].color = p->getColor();
                pieces[nonKingCount].row = r;
                pieces[nonKingCount].col = c;
            }

            nonKingCount++;

            PieceType t = p->getType();
            if (t == ROOK || t == QUEEN || t == PAWN)
                return false;
        }
    }

    if (nonKingCount == 0)
        return true;

    if (nonKingCount == 1 &&
        (pieces[0].type == BISHOP || pieces[0].type == KNIGHT))
        return true;

    if (nonKingCount == 2 &&
        pieces[0].type == BISHOP &&
        pieces[1].type == BISHOP &&
        pieces[0].color != pieces[1].color) {

        bool dark1 = (pieces[0].row + pieces[0].col) % 2;
        bool dark2 = (pieces[1].row + pieces[1].col) % 2;
        if (dark1 == dark2)
            return true;
    }

    return false;
}

