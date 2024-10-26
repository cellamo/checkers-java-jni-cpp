package com.checkers.core;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class NativeInterfaceTest {
    private NativeInterface nativeInterface;

    @BeforeEach
    void setUp() {
        nativeInterface = new NativeInterface();
        nativeInterface.initGame();
    }

    @Test
    void testInitialBoardState() {
        int[][] board = nativeInterface.getBoardState();

        // Check board dimensions
        assertEquals(8, board.length);
        assertEquals(8, board[0].length);

        // Check black pieces (value 1) on top rows
        assertEquals(1, board[0][1]);
        assertEquals(1, board[0][3]);
        assertEquals(1, board[0][5]);
        assertEquals(1, board[0][7]);

        // Check empty middle rows
        assertEquals(0, board[3][0]);
        assertEquals(0, board[4][1]);

        // Check red pieces (value 3) on bottom rows
        assertEquals(3, board[5][0]);
        assertEquals(3, board[5][2]);
        assertEquals(3, board[5][4]);
        assertEquals(3, board[5][6]);
    }

    @Test
    void testValidMove() {
        assertTrue(nativeInterface.validateMove(1, 2, 2, 3));
        assertFalse(nativeInterface.validateMove(1, 2, 0, 1));
    }

    @Test
    void testPerformMove() {
        int[][] initialBoard = nativeInterface.getBoardState();
        nativeInterface.performMove(1, 2, 2, 3);
        int[][] newBoard = nativeInterface.getBoardState();

        assertNotEquals(initialBoard[2][1], newBoard[2][1]);
        assertEquals(initialBoard[2][1], newBoard[3][2]);
    }

    @Test
    void testCapture() {
        // Setup a capture scenario
        int[][] board = nativeInterface.getBoardState();
        // Verify initial positions
        assertEquals(1, board[2][3]); // Black piece
        assertEquals(3, board[3][2]); // Red piece

        // Perform capture move
        assertTrue(nativeInterface.validateMove(2, 3, 4, 1));
        nativeInterface.performMove(2, 3, 4, 1);

        // Verify capture
        board = nativeInterface.getBoardState();
        assertEquals(1, board[4][1]); // Black piece moved
        assertEquals(0, board[3][2]); // Red piece captured
        assertEquals(0, board[2][3]); // Original position empty
    }

    @Test
    void testKingPromotion() {
        // Move piece to promotion position
        nativeInterface.performMove(1, 6, 0, 7);

        int[][] board = nativeInterface.getBoardState();
        assertEquals(2, board[0][7]); // Should be black king (2)
    }

    @Test
    void testKingMovement() {
        // Setup king piece
        // Kings can move diagonally in any direction
        int[][] board = nativeInterface.getBoardState();
        board[4][4] = 2; // Place black king

        assertTrue(nativeInterface.validateMove(4, 4, 2, 2)); // Backward move
        assertTrue(nativeInterface.validateMove(4, 4, 6, 6)); // Forward move
    }
}
