package com.checkers.ui;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.DisplayName;
import java.awt.Point;
import static org.junit.jupiter.api.Assertions.*;

class GameBoardTest {
    private GameBoard gameBoard;

    @BeforeEach
    void setUp() {
        gameBoard = new GameBoard(player -> {});
    }

    @Test
    @DisplayName("GameBoard should initialize with correct dimensions")
    void testBoardDimensions() {
        assertEquals(480, gameBoard.getPreferredSize().width);  // 8 * 60
        assertEquals(480, gameBoard.getPreferredSize().height); // 8 * 60
    }

    @Test
    @DisplayName("Mouse clicks should be correctly converted to board coordinates")
    void testMouseCoordinateConversion() {
        // Create a method to expose the coordinate conversion for testing
        Point boardCoord = gameBoard.convertMouseCoordinatesToBoard(new Point(125, 245));
        assertEquals(2, boardCoord.x); // 125 / 60 = 2
        assertEquals(4, boardCoord.y); // 245 / 60 = 4
    }
}
