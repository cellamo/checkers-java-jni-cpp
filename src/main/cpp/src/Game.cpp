#include "Game.h"

Game::Game() : currentPlayer(PieceColor::BLACK) {
    initGame();
}

void Game::initGame() {
    board.initializeBoard();
    currentPlayer = PieceColor::BLACK;
}

bool Game::validateMove(int startX, int startY, int endX, int endY) {
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
    // Implement game over logic
    return false;
}

PieceColor Game::getWinner() const {
    // Implement winner determination logic
    return PieceColor::NONE;
}
