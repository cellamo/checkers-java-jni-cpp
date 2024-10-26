#pragma once
#include "Board.h"

class Game {
public:
    Game();
    void initGame();
    bool validateMove(int startX, int startY, int endX, int endY) const;
    void performMove(int startX, int startY, int endX, int endY);
    const Board& getBoard() const;
    PieceColor getCurrentPlayer() const;
    bool isGameOver() const;
    PieceColor getWinner() const;

private:
    Board board;
    PieceColor currentPlayer;
    bool checkCapturePossible() const;
};
