#pragma once
enum PieceType { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
enum Color { WHITE, BLACK };

class Board;

class ChessPiece {
protected:
    Color color;
    PieceType type;
    const char* symbol;
    bool _hasMoved;

public:
    ChessPiece(Color c, PieceType t, const char* s)
            : color(c), type(t), symbol(s), _hasMoved(false) {}
    virtual ~ChessPiece() {}

    Color getColor() const { return color; }
    PieceType getType() const { return type; }
    const char* getSymbol() const { return symbol; }
    void setMoved(bool moved) { _hasMoved = moved; }
    bool hasMoved() const { return _hasMoved; }

    virtual bool isMoveValid(int fr, int fc, int tr, int tc, const Board& board) const = 0;
};
