#include "Game.h"
#include <iostream>
#include <cstdio>
#include <fstream>

using namespace std;
const char* SAVE_FILE = "save.txt";

Game::Game() : currentTurn(WHITE) {
    board.initialize();
    historyCount = 0;
    recordBoardState();
}

void Game::run() {
    char input[10];
    board.print(currentTurn);

    while (true) {
        printTurnInfo();
        cin.getline(input, 10);

        if (input[0] == 'q') break;
        if (handleFileCommand(input)) continue;

        MoveResult result = attemptMove(input);

        if (result == MOVE_OK) {
            postMoveChecks();
            continue;
        }
        else{
            printMoveError(result);
        }
    }

    cout << "Game ended.\n";
}

void Game::printTurnInfo() {
    cout << (currentTurn == WHITE ? "White" : "Black") << " to move: ";
}

void Game::clearScreen(){
    cout << "\033[2J\033[1;1H";
}

void Game::postMoveChecks() {
    clearScreen();

    Color opponent = (currentTurn == WHITE ? BLACK : WHITE);
    
    board.print(currentTurn);
    board.print(opponent);

    if (isThreefoldRepetition()) {
        cout << "Draw by threefold repetition.\n";
        exit(0);
    }

    if (board.isInCheck(opponent) && !board.hasLegalMoves(opponent)) {
        cout << (currentTurn == WHITE ? "White" : "Black") << " wins by checkmate!\n";
        exit(0);
    } else if (!board.isInCheck(opponent) && !board.hasLegalMoves(opponent)) {
        cout << "Stalemate! It's a draw.\n";
        exit(0);
    } else if (board.isInsufficientMaterial()) {
        cout << "Draw by insufficient material!\n";
        exit(0);
    }

    currentTurn = opponent;
}

MoveResult Game::attemptMove(const char* input) {
    int fr, fc, tr, tc;
    if (!parseMove(input, fr, fc, tr, tc)) {
        return INVALID_INPUT;
    }

    ChessPiece* selected = board.getPiece(fr, fc);
    ChessPiece* target = board.getPiece(tr, tc);

    if (!selected) return NO_PIECE;
    if (selected->getColor() != currentTurn) return WRONG_COLOR;
    if (target && target->getColor() == currentTurn) return FRIENDLY_CAPTURE;
    if (!selected->isMoveValid(fr, fc, tr, tc, board)) return ILLEGAL_MOVE;
    if (!board.movePiece(fr, fc, tr, tc, currentTurn)) return KING_IN_CHECK;

    recordBoardState();
    return MOVE_OK;
}

bool Game::parseMove(const char input[10], int& fr, int& fc, int& tr, int& tc) {
    if (input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8' ||
        input[3] < 'a' || input[3] > 'h' || input[4] < '1' || input[4] > '8')
        return false;

    fc = input[0] - 'a';
    fr = 8 - (input[1] - '0');
    tc = input[3] - 'a';
    tr = 8 - (input[4] - '0');
    return true;
}

void Game::printMoveError(MoveResult result) {
    switch (result) {
        case INVALID_INPUT:
            cout << "Invalid input format. Use format like e2 e4.\n"; break;
        case NO_PIECE:
            cout << "No piece at source square.\n"; break;
        case WRONG_COLOR:
            cout << "Not your turn.\n"; break;
        case FRIENDLY_CAPTURE:
            cout << "You can't capture your own piece.\n"; break;
        case ILLEGAL_MOVE:
            cout << "Illegal move for that piece.\n"; break;
        case KING_IN_CHECK:
            cout << "You're in check! You must make a move that gets you out of check.\n"; break;
        default:
            cout << "Unknown move error.\n"; break;
    }
}

void Game::recordBoardState() {
    if (historyCount >= MAX_HISTORY) return;

    BoardSnapshot& snap = history[historyCount++];
    snap.turn = currentTurn;

    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            ChessPiece* p = board.getPiece(r, c);
            if (!p) snap.board[r][c] = '.';
            else {
                switch (p->getType()) {
                    case PAWN:   snap.board[r][c] = (p->getColor() == WHITE ? 'P' : 'p'); break;
                    case ROOK:   snap.board[r][c] = (p->getColor() == WHITE ? 'R' : 'r'); break;
                    case KNIGHT: snap.board[r][c] = (p->getColor() == WHITE ? 'N' : 'n'); break;
                    case BISHOP: snap.board[r][c] = (p->getColor() == WHITE ? 'B' : 'b'); break;
                    case QUEEN:  snap.board[r][c] = (p->getColor() == WHITE ? 'Q' : 'q'); break;
                    case KING:   snap.board[r][c] = (p->getColor() == WHITE ? 'K' : 'k'); break;
                }
            }
        }
}

bool Game::isThreefoldRepetition() {
    int count = 0;
    for (int i = 0; i < historyCount; i++) {
        bool same = true;
        for (int r = 0; r < 8 && same; r++)
            for (int c = 0; c < 8; c++)
                if (history[i].board[r][c] != history[historyCount - 1].board[r][c])
                    same = false;

        if (same) count++;
        if (count >= 3) return true;
    }
    return false;
}

bool Game::handleFileCommand(const char* input) {
    if (input[0] == 's' && input[1] == '\0') {
        saveGame(SAVE_FILE);
        cout << "Game saved.\n";
        return true;
    }
    if (input[0] == 'l' && input[1] == '\0') {
        loadGame(SAVE_FILE);
        cout << "Game loaded.\n";
        return true;
    }
    return false;
}

bool Game::saveGame(const char* filename) {
    std::ofstream out(filename);
    if (!out) return false;

    out << (currentTurn == WHITE ? "WHITE\n" : "BLACK\n");
    out << board.getEnPassantRow() << " " << board.getEnPassantCol() << "\n";

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            ChessPiece* p = board.getPiece(r, c);
            if (!p) {
                out << ".\n";
            } else {
                char typeChar;
                switch (p->getType()) {
                    case PAWN:   typeChar = 'P'; break;
                    case ROOK:   typeChar = 'R'; break;
                    case KNIGHT: typeChar = 'N'; break;
                    case BISHOP: typeChar = 'B'; break;
                    case QUEEN:  typeChar = 'Q'; break;
                    case KING:   typeChar = 'K'; break;
                    default:     typeChar = '?'; break;
                }

                if (p->getColor() == BLACK)
                    typeChar += 32;

                int moved = p->hasMoved() ? 1 : 0;
                out << typeChar << " " << moved << "\n";
            }
        }
    }

    out << historyCount << "\n";

    for (int i = 0; i < historyCount; i++) {
        out << (history[i].turn == WHITE ? "W" : "B") << "\n";
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                out << history[i].board[r][c];
        out << "\n";
    }

    return true;
}

bool Game::loadGame(const char* filename) {
    std::ifstream in(filename);
    if (!in) return false;

    char turnStr[10];
    in >> turnStr;
    currentTurn = (turnStr[0] == 'W') ? WHITE : BLACK;

    int epRow, epCol;
    in >> epRow >> epCol;
    board.setEnPassant(epRow, epCol);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char line[10];
            in >> line;

            if (line[0] == '.') {
                board.setPiece(r, c, nullptr);
            } else {
                char ch = line[0];
                int moved;
                in >> moved;

                Color color = (ch >= 'a') ? BLACK : WHITE;
                if (color == BLACK) ch -= 32;

                ChessPiece* p = nullptr;
                switch (ch) {
                    case 'P': p = new Pawn(color); break;
                    case 'R': p = new Rook(color); break;
                    case 'N': p = new Knight(color); break;
                    case 'B': p = new Bishop(color); break;
                    case 'Q': p = new Queen(color); break;
                    case 'K': p = new King(color); break;
                }

                if (p) p->setMoved(moved == 1);
                board.setPiece(r, c, p);
            }
        }
    }

    in >> historyCount;
    for (int i = 0; i < historyCount; i++) {
        char turnChar;
        in >> turnChar;
        history[i].turn = (turnChar == 'W') ? WHITE : BLACK;

        char boardLine[65];
        in >> boardLine;

        int k = 0;
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                history[i].board[r][c] = boardLine[k++];
    }

    board.print(currentTurn);
    return true;
}

