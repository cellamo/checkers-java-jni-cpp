#include "Board.h"

Board::Board() : board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE)) {
    initializeBoard();
}

void Board::initializeBoard() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if ((x + y) % 2 == 1) {
                if (y < 3) {
                    board[y][x] = Piece(PieceColor::BLACK);
                } else if (y > 4) {
                    board[y][x] = Piece(PieceColor::RED);
                } else {
                    board[y][x] = Piece(PieceColor::NONE);
                }
            } else {
                board[y][x] = Piece(PieceColor::NONE);
            }
        }
    }
}

Piece Board::getPiece(int x, int y) const {
    return board[y][x];
}

void Board::setPiece(int x, int y, const Piece& piece) {
    board[y][x] = piece;
}

bool Board::isValidPosition(int x, int y) const {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}
