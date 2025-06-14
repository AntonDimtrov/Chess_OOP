#pragma once
#include "ChessPiece.h"

class Board {
private:
    ChessPiece* squares[8][8];
    int enPassantRow;
    int enPassantCol;

public:
    Board();
    ~Board();

    void initialize();
    void print(Color turn) const;

    ChessPiece* getPiece(int r, int c) const;
    void setPiece(int r, int c, ChessPiece* piece);

    bool movePiece(int fr, int fc, int tr, int tc, Color color);
    bool isPathClear(int fr, int fc, int tr, int tc) const;

    bool isSquareAttacked(int row, int col, Color byColor) const;
    bool isInCheck(Color color) const;
    bool hasLegalMoves(Color color);
    bool isInsufficientMaterial() const;

    void setEnPassant(int row, int col);
    void clearEnPassant();
    int getEnPassantRow() const;
    int getEnPassantCol() const;
};
