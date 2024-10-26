#include <cassert>
#include "Game.h"

void testInitialization() {
    Game game;
    const Board& board = game.getBoard();
    
    // Test initial piece positions
    // Black pieces should be on top (rows 0-2)
    assert(board.getPiece(1, 0).color == PieceColor::BLACK);
    assert(board.getPiece(3, 0).color == PieceColor::BLACK);
    assert(board.getPiece(5, 0).color == PieceColor::BLACK);
    assert(board.getPiece(7, 0).color == PieceColor::BLACK);
    
    // Middle rows (3-4) should be empty
    assert(board.getPiece(0, 3).color == PieceColor::NONE);
    assert(board.getPiece(1, 4).color == PieceColor::NONE);
    
    // Red pieces should be on bottom (rows 5-7)
    assert(board.getPiece(0, 5).color == PieceColor::RED);
    assert(board.getPiece(2, 5).color == PieceColor::RED);
    assert(board.getPiece(4, 5).color == PieceColor::RED);
    assert(board.getPiece(6, 5).color == PieceColor::RED);
}

void testValidMoves() {
    Game game;
    
    // Test valid diagonal move for black piece
    assert(game.validateMove(1, 2, 2, 3) == true);
    
    // Test invalid backward move for normal piece
    assert(game.validateMove(1, 2, 0, 1) == false);
}

int main() {
    testInitialization();
    testValidMoves();
    printf("All tests passed!\n");
    return 0;
}
