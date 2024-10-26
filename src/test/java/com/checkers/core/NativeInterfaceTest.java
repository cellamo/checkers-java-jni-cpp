package com.checkers.core;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.DisplayName;
import static org.junit.jupiter.api.Assertions.*;

class NativeInterfaceTest {
    private NativeInterface nativeInterface;

    @BeforeEach
    void setUp() {
        nativeInterface = new NativeInterface();
        nativeInterface.initGame();
    }

    @Test
    @DisplayName("Initial board state should be correctly set up")
    void testInitialBoardState() {
        int[][] board = nativeInterface.getBoardState();
        
        // Board dimensions
        assertEquals(8, board.length);
        assertEquals(8, board[0].length);
        
        // Black pieces (1) on top rows
        assertEquals(1, board[0][1]);
        assertEquals(1, board[0][3]);
        assertEquals(1, board[0][5]);
        assertEquals(1, board[0][7]);
        assertEquals(1, board[1][0]);
        assertEquals(1, board[1][2]);
        
        // Empty middle rows
        for (int i = 0; i < 8; i++) {
            assertEquals(0, board[3][i]);
            assertEquals(0, board[4][i]);
        }
        
        // Red pieces (3) on bottom rows
        assertEquals(3, board[5][0]);
        assertEquals(3, board[5][2]);
        assertEquals(3, board[5][4]);
        assertEquals(3, board[5][6]);
        assertEquals(3, board[6][1]);
        assertEquals(3, board[6][3]);
    }

    @Test
    @DisplayName("Valid moves should be correctly identified")
    void testValidMoves() {
        // Regular moves
        assertTrue(nativeInterface.validateMove(1, 2, 2, 3));
        assertTrue(nativeInterface.validateMove(5, 2, 4, 3));
        
        // Invalid moves
        assertFalse(nativeInterface.validateMove(0, 0, 0, 1)); // Non-diagonal
        assertFalse(nativeInterface.validateMove(1, 2, 1, 3)); // Vertical move
        assertFalse(nativeInterface.validateMove(1, 2, 3, 4)); // Too far
    }

    @Test
    @DisplayName("Pieces should move correctly")
    void testPerformMove() {
        int[][] initialBoard = nativeInterface.getBoardState();
        nativeInterface.performMove(1, 2, 2, 3);
        int[][] newBoard = nativeInterface.getBoardState();
        
        // Original position should be empty
        assertEquals(0, newBoard[2][1]);
        // New position should have the piece
        assertEquals(initialBoard[2][1], newBoard[3][2]);
    }

    @Test
    @DisplayName("Player turns should alternate correctly")
    void testPlayerTurns() {
        // Game starts with Black (0)
        assertEquals(0, nativeInterface.getCurrentPlayer());
        
        // Make a valid black move
        nativeInterface.performMove(1, 2, 0, 3);
        assertEquals(1, nativeInterface.getCurrentPlayer());
        
        // Make a valid red move
        nativeInterface.performMove(4, 5, 3, 4);
        assertEquals(0, nativeInterface.getCurrentPlayer());
    }
    
    @Test
    @DisplayName("Capture moves should be valid")
    void testCaptureMoves() {
        // Setup a capture scenario with valid moves
        nativeInterface.performMove(1, 2, 2, 3); // Black
        nativeInterface.performMove(2, 5, 3, 4); // Red
        nativeInterface.performMove(3, 2, 4, 3); // Black
        
        // Now Red piece can capture Black piece
        assertTrue(nativeInterface.validateMove(3, 4, 1, 2));
    }    
    
}
