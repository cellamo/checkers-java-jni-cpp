#include "Board.h"

Board::Board() : board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE)) {
    initializeBoard();
}


// end game test
void Board::initializeBoard() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = Piece(PieceColor::NONE);
        }
    }
    
    // Place 2 black pieces
    board[2][1] = Piece(PieceColor::BLACK);
    board[3][4] = Piece(PieceColor::BLACK);
    
    // Place 2 red pieces
    board[4][3] = Piece(PieceColor::RED);
    board[5][2] = Piece(PieceColor::RED);
}

// real init:
// void Board::initializeBoard() {
//     for (int y = 0; y < BOARD_SIZE; y++) {
//         for (int x = 0; x < BOARD_SIZE; x++) {
//             if ((x + y) % 2 == 1) {
//                 if (y < 3) {
//                     board[y][x] = Piece(PieceColor::BLACK);
//                 } else if (y > 4) {
//                     board[y][x] = Piece(PieceColor::RED);
//                 } else {
//                     board[y][x] = Piece(PieceColor::NONE);
//                 }
//             } else {
//                 board[y][x] = Piece(PieceColor::NONE);
//             }
//         }
//     }
// }

Piece Board::getPiece(int x, int y) const {
    return board[y][x];
}

void Board::setPiece(int x, int y, const Piece& piece) {
    board[y][x] = piece;
}

bool Board::isValidPosition(int x, int y) const {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}
