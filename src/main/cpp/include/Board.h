#pragma once
#include "Piece.h"
#include <vector>

class Board {
public:
    static const int BOARD_SIZE = 8;
    Board();
    void initializeBoard();
    Piece getPiece(int x, int y) const;
    void setPiece(int x, int y, const Piece& piece);
    bool isValidPosition(int x, int y) const;

private:
    std::vector<std::vector<Piece>> board;
};
