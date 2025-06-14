#pragma once
#include "Board.h"
#include "Pawn.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"

enum MoveResult {
    MOVE_OK,
    INVALID_INPUT,
    NO_PIECE,
    WRONG_COLOR,
    FRIENDLY_CAPTURE,
    ILLEGAL_MOVE,
    KING_IN_CHECK
};

class Game {
private:
    Board board;
    Color currentTurn;

    static const int MAX_HISTORY = 100;

    struct BoardSnapshot {
        char board[8][8];
        Color turn;
    };

    BoardSnapshot history[MAX_HISTORY];
    int historyCount;

    void runGameLoop();
    void clearScreen();
    void printTurnInfo();
    void postMoveChecks();
    void printMoveError(MoveResult result);
    
    MoveResult attemptMove(const char* input);
    bool parseMove(const char input[10], int& fr, int& fc, int& tr, int& tc);

    bool handleFileCommand(const char* input);
    bool saveGame(const char* filename);
    bool loadGame(const char* filename);

    void recordBoardState();
    bool isThreefoldRepetition();

public:
    Game();
    void run();
};
