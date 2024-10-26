package com.checkers.ui;

import com.checkers.core.NativeInterface;
import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

public class GameBoard extends JPanel {
    private static final int CELL_SIZE = 60;
    private static final int BOARD_SIZE = 8;
    private final NativeInterface nativeInterface;
    private Point selectedPiece = null;
    private List<Point> validMoves = new ArrayList<>();
    private final Consumer<Integer> statusCallback;

    public GameBoard(Consumer<Integer> statusCallback) {
        this.statusCallback = statusCallback;

        nativeInterface = new NativeInterface();
        nativeInterface.initGame();

        setPreferredSize(new Dimension(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE));
        setBackground(Color.WHITE);

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                handleMouseClick(e.getPoint());
            }
        });

        statusCallback.accept(nativeInterface.getCurrentPlayer());
    }

    private void handleMouseClick(Point click) {
        int col = click.x / CELL_SIZE;
        int row = click.y / CELL_SIZE;
        
        if (selectedPiece == null) {
            int[][] board = nativeInterface.getBoardState();
            int piece = board[row][col];
            int currentPlayer = nativeInterface.getCurrentPlayer();
            
            // Validate piece selection based on current player
            boolean isValidSelection = (currentPlayer == 0 && (piece == 1 || piece == 2)) || 
                                     (currentPlayer == 1 && (piece == 3 || piece == 4));
            
            if (isValidSelection) {
                selectedPiece = new Point(col, row);
                highlightValidMoves();
                repaint();
            }
        } else {
            // Attempt to move the selected piece
            if (nativeInterface.validateMove(selectedPiece.x, selectedPiece.y, col, row)) {
                nativeInterface.performMove(selectedPiece.x, selectedPiece.y, col, row);
                selectedPiece = null;
                validMoves.clear();
                statusCallback.accept(nativeInterface.getCurrentPlayer());
                
                if (nativeInterface.isGameOver()) {
                    int winner = nativeInterface.getWinner();
                    String winnerText = winner == 0 ? "Black" : "Red";
                    JOptionPane.showMessageDialog(this, 
                        winnerText + " wins!", 
                        "Game Over", 
                        JOptionPane.INFORMATION_MESSAGE);
                }
                
                repaint();
            } else {
                // Invalid move - deselect piece
                selectedPiece = null;
                validMoves.clear();
                repaint();
            }
        }
    }    

    private void highlightValidMoves() {
        validMoves.clear();
        int[][] directions = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
        for (int[] dir : directions) {
            int newX = selectedPiece.x + dir[0];
            int newY = selectedPiece.y + dir[1];
            if (nativeInterface.validateMove(selectedPiece.x, selectedPiece.y, newX, newY)) {
                validMoves.add(new Point(newX, newY));
            }
        }
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        drawBoard(g);

        // Highlight selected piece
        if (selectedPiece != null) {
            g.setColor(new Color(255, 255, 0, 128));
            g.fillRect(selectedPiece.x * CELL_SIZE,
                    selectedPiece.y * CELL_SIZE,
                    CELL_SIZE, CELL_SIZE);
        }

        // Highlight valid moves
        g.setColor(new Color(0, 255, 0, 128));
        for (Point move : validMoves) {
            g.fillRect(move.x * CELL_SIZE,
                    move.y * CELL_SIZE,
                    CELL_SIZE, CELL_SIZE);
        }

        drawPieces(g);
    }

    private void drawBoard(Graphics g) {
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                Color cellColor = (row + col) % 2 == 0 ? Color.WHITE : Color.BLACK;
                g.setColor(cellColor);
                g.fillRect(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            }
        }
    }

    private void drawPieces(Graphics g) {
        int[][] board = nativeInterface.getBoardState();
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                int piece = board[row][col];
                if (piece != 0) {
                    drawPiece(g, row, col, piece);
                }
            }
        }
    }

    private void drawPiece(Graphics g, int row, int col, int piece) {
        int x = col * CELL_SIZE + CELL_SIZE / 6;
        int y = row * CELL_SIZE + CELL_SIZE / 6;
        int diameter = 2 * CELL_SIZE / 3;

        // Black pieces: 1=normal, 2=king
        // Red pieces: 3=normal, 4=king
        Color pieceColor = (piece == 1 || piece == 2) ? Color.DARK_GRAY : Color.RED;
        g.setColor(pieceColor);
        g.fillOval(x, y, diameter, diameter);

        // Draw crown for king pieces
        if (piece == 2 || piece == 4) {
            g.setColor(Color.YELLOW);
            g.drawString("K", x + diameter / 2 - 5, y + diameter / 2 + 5);
        }
    }
}
