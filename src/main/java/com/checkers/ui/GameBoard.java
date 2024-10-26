package com.checkers.ui;

import com.checkers.core.NativeInterface;
import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
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
    private Point cursorPosition = new Point(0, 0);
    private boolean isKeyboardSelectionMode = false;

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

        setFocusable(true);
        addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                handleKeyPress(e);
            }
        });

        statusCallback.accept(nativeInterface.getCurrentPlayer());
    }

    private boolean hasCaptureMoves(int playerColor) {
        int[][] board = nativeInterface.getBoardState();
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                int piece = board[y][x];
                // Check if piece belongs to current player
                if ((playerColor == 0 && (piece == 1 || piece == 2)) ||
                        (playerColor == 1 && (piece == 3 || piece == 4))) {
                    // Check for possible captures
                    int[][] captureDirections = { { -2, -2 }, { -2, 2 }, { 2, -2 }, { 2, 2 } };
                    for (int[] dir : captureDirections) {
                        int newX = x + dir[0];
                        int newY = y + dir[1];
                        if (nativeInterface.validateMove(x, y, newX, newY)) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    private void handleMouseClick(Point click) {
        int col = click.x / CELL_SIZE;
        int row = click.y / CELL_SIZE;

        if (selectedPiece == null) {
            int[][] board = nativeInterface.getBoardState();
            int piece = board[row][col];
            int currentPlayer = nativeInterface.getCurrentPlayer();

            // Check if any piece has capture moves
            boolean hasCaptures = hasCaptureMoves(currentPlayer);

            // If captures exist, only allow selecting pieces with captures
            boolean isValidSelection;
            if (hasCaptures) {
                isValidSelection = (currentPlayer == 0 && (piece == 1 || piece == 2)) ||
                        (currentPlayer == 1 && (piece == 3 || piece == 4));

                if (isValidSelection) {
                    // Verify this specific piece has capture moves
                    int[][] captureDirections = { { -2, -2 }, { -2, 2 }, { 2, -2 }, { 2, 2 } };
                    boolean hasCaptureMove = false;
                    for (int[] dir : captureDirections) {
                        int newX = col + dir[0];
                        int newY = row + dir[1];
                        if (nativeInterface.validateMove(col, row, newX, newY)) {
                            hasCaptureMove = true;
                            break;
                        }
                    }
                    isValidSelection = hasCaptureMove;
                }
            } else {
                // No captures available, normal selection rules apply
                isValidSelection = (currentPlayer == 0 && (piece == 1 || piece == 2)) ||
                        (currentPlayer == 1 && (piece == 3 || piece == 4));
            }

            if (isValidSelection) {
                selectedPiece = new Point(col, row);
                highlightValidMoves();
                repaint();
            }
        } else {
            // Check if clicked position is in validMoves list
            Point targetMove = new Point(col, row);
            if (validMoves.stream().anyMatch(move -> move.equals(targetMove))) {
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

        // First check for capture moves
        boolean hasCaptures = false;
        int[][] captureDirections = { { -2, -2 }, { -2, 2 }, { 2, -2 }, { 2, 2 } };

        for (int[] dir : captureDirections) {
            int newX = selectedPiece.x + dir[0];
            int newY = selectedPiece.y + dir[1];
            if (nativeInterface.validateMove(selectedPiece.x, selectedPiece.y, newX, newY)) {
                validMoves.add(new Point(newX, newY));
                hasCaptures = true;
            }
        }

        // Only check for regular moves if no captures are available
        if (!hasCaptures) {
            int[][] regularDirections = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
            for (int[] dir : regularDirections) {
                int newX = selectedPiece.x + dir[0];
                int newY = selectedPiece.y + dir[1];
                if (nativeInterface.validateMove(selectedPiece.x, selectedPiece.y, newX, newY)) {
                    validMoves.add(new Point(newX, newY));
                }
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

        // Draw keyboard cursor - outer border
        g.setColor(new Color(0, 200, 255));
        g.drawRect(cursorPosition.x * CELL_SIZE + 2,
                cursorPosition.y * CELL_SIZE + 2,
                CELL_SIZE - 4, CELL_SIZE - 4);

        // Inner border for better contrast
        g.drawRect(cursorPosition.x * CELL_SIZE + 1,
                cursorPosition.y * CELL_SIZE + 1,
                CELL_SIZE - 3, CELL_SIZE - 3);

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

    public Point convertMouseCoordinatesToBoard(Point mousePoint) {
        return new Point(mousePoint.x / CELL_SIZE, mousePoint.y / CELL_SIZE);
    }

    private void handleKeyPress(KeyEvent e) {
        switch (e.getKeyCode()) {
            case KeyEvent.VK_LEFT:
                cursorPosition.x = Math.max(0, cursorPosition.x - 1);
                break;
            case KeyEvent.VK_RIGHT:
                cursorPosition.x = Math.min(BOARD_SIZE - 1, cursorPosition.x + 1);
                break;
            case KeyEvent.VK_UP:
                cursorPosition.y = Math.max(0, cursorPosition.y - 1);
                break;
            case KeyEvent.VK_DOWN:
                cursorPosition.y = Math.min(BOARD_SIZE - 1, cursorPosition.y + 1);
                break;
            case KeyEvent.VK_ENTER:
            case KeyEvent.VK_SPACE:
                handleKeyboardSelection();
                break;
        }
        repaint();
    }

    private void handleKeyboardSelection() {
        if (!isKeyboardSelectionMode) {
            handleMouseClick(new Point(
                    cursorPosition.x * CELL_SIZE + CELL_SIZE / 2,
                    cursorPosition.y * CELL_SIZE + CELL_SIZE / 2));
            isKeyboardSelectionMode = selectedPiece != null;
        } else {
            handleMouseClick(new Point(
                    cursorPosition.x * CELL_SIZE + CELL_SIZE / 2,
                    cursorPosition.y * CELL_SIZE + CELL_SIZE / 2));
            isKeyboardSelectionMode = false;
        }
    }

}
