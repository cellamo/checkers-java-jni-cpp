#include "Game.h"

Game::Game() : currentPlayer(PieceColor::BLACK) {
    initGame();
}

void Game::initGame() {
    board.initializeBoard();
    currentPlayer = PieceColor::BLACK;
}

bool Game::validateMove(int startX, int startY, int endX, int endY) const {
    if (!board.isValidPosition(startX, startY) || !board.isValidPosition(endX, endY)) {
        return false;
    }

    Piece piece = board.getPiece(startX, startY);
    if (piece.color != currentPlayer) {
        return false;
    }

    // Basic move validation (diagonal only)
    int dx = endX - startX;
    int dy = endY - startY;
    
    // Normal pieces can only move forward
    if (piece.type == PieceType::NORMAL) {
        if (piece.color == PieceColor::BLACK && dy <= 0) return false;
        if (piece.color == PieceColor::RED && dy >= 0) return false;
    }

    // Check for valid diagonal move
    if (abs(dx) != abs(dy)) return false;

    // Simple move
    if (abs(dx) == 1) {
        return board.getPiece(endX, endY).color == PieceColor::NONE;
    }

    // Capture move
    if (abs(dx) == 2) {
        int midX = startX + dx/2;
        int midY = startY + dy/2;
        Piece capturedPiece = board.getPiece(midX, midY);
        return capturedPiece.color != PieceColor::NONE && 
               capturedPiece.color != currentPlayer &&
               board.getPiece(endX, endY).color == PieceColor::NONE;
    }

    return false;
}

void Game::performMove(int startX, int startY, int endX, int endY) {
    if (!validateMove(startX, startY, endX, endY)) return;

    Piece piece = board.getPiece(startX, startY);
    board.setPiece(endX, endY, piece);
    board.setPiece(startX, startY, Piece());

    // Handle capture
    if (abs(endX - startX) == 2) {
        int midX = startX + (endX - startX)/2;
        int midY = startY + (endY - startY)/2;
        board.setPiece(midX, midY, Piece());
    }

    // Handle promotion
    if ((piece.color == PieceColor::BLACK && endY == Board::BOARD_SIZE-1) ||
        (piece.color == PieceColor::RED && endY == 0)) {
        piece.type = PieceType::KING;
        board.setPiece(endX, endY, piece);
    }

    currentPlayer = (currentPlayer == PieceColor::BLACK) ? PieceColor::RED : PieceColor::BLACK;
}

const Board& Game::getBoard() const {
    return board;
}

PieceColor Game::getCurrentPlayer() const {
    return currentPlayer;
}

bool Game::isGameOver() const {
    // Count pieces and check for possible moves for each player
    int blackPieces = 0;
    int redPieces = 0;
    bool blackHasMoves = false;
    bool redHasMoves = false;

    for (int y = 0; y < Board::BOARD_SIZE; y++) {
        for (int x = 0; x < Board::BOARD_SIZE; x++) {
            Piece piece = board.getPiece(x, y);
            
            // Count pieces
            if (piece.color == PieceColor::BLACK) {
                blackPieces++;
                // Check for possible moves
                if (!blackHasMoves) {
                    for (int dy = -2; dy <= 2; dy++) {
                        for (int dx = -2; dx <= 2; dx++) {
                            if (dx != 0 && dy != 0 && validateMove(x, y, x + dx, y + dy)) {
                                blackHasMoves = true;
                                break;
                            }
                        }
                    }
                }
            } else if (piece.color == PieceColor::RED) {
                redPieces++;
                // Check for possible moves
                if (!redHasMoves) {
                    for (int dy = -2; dy <= 2; dy++) {
                        for (int dx = -2; dx <= 2; dx++) {
                            if (dx != 0 && dy != 0 && validateMove(x, y, x + dx, y + dy)) {
                                redHasMoves = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    // Game is over if either player has no pieces or no valid moves
    return blackPieces == 0 || redPieces == 0 || 
           (currentPlayer == PieceColor::BLACK && !blackHasMoves) ||
           (currentPlayer == PieceColor::RED && !redHasMoves);
}

PieceColor Game::getWinner() const {
    if (!isGameOver()) return PieceColor::NONE;

    // Count pieces
    int blackPieces = 0;
    int redPieces = 0;
    bool blackHasMoves = false;
    bool redHasMoves = false;

    for (int y = 0; y < Board::BOARD_SIZE; y++) {
        for (int x = 0; x < Board::BOARD_SIZE; x++) {
            Piece piece = board.getPiece(x, y);
            if (piece.color == PieceColor::BLACK) blackPieces++;
            if (piece.color == PieceColor::RED) redPieces++;
        }
    }

    // Determine winner based on piece count and available moves
    if (blackPieces == 0 || (currentPlayer == PieceColor::BLACK && !blackHasMoves)) {
        return PieceColor::RED;
    }
    if (redPieces == 0 || (currentPlayer == PieceColor::RED && !redHasMoves)) {
        return PieceColor::BLACK;
    }

    return PieceColor::NONE;
}
