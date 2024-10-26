#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <vector>
#include <utility>

enum class PieceType {
    EMPTY,
    BLACK,
    RED,
    BLACK_KING,
    RED_KING
};

enum class Player {
    BLACK,
    RED
};

enum class GameStatus {
    IN_PROGRESS,
    BLACK_WINS,
    RED_WINS,
    DRAW
};

class GameBoard {
private:
    static const int BOARD_SIZE = 8;
    std::vector<std::vector<PieceType>> board;
    Player currentPlayer;
    GameStatus status;

    bool isKing(PieceType piece) const;
    bool isPieceOwnedBy(Player player, PieceType piece) const;
    std::vector<std::pair<int, int>> getValidJumps(int x, int y) const;
    std::vector<std::pair<int, int>> getValidMoves(int x, int y) const;
    bool hasValidJumps() const;
    void removeCapturedPiece(int fromX, int fromY, int toX, int toY);
    void updateGameStatus();
    int getPieceCount(Player player) const;

public:
    GameBoard();
    void initializeBoard();
    PieceType getPiece(int x, int y) const;
    bool isValidPosition(int x, int y) const;
    bool makeMove(int fromX, int fromY, int toX, int toY);
    bool hasValidMoves(Player player) const;
    void switchPlayer();
    Player getCurrentPlayer() const;
    GameStatus getGameStatus() const;
};

#endif