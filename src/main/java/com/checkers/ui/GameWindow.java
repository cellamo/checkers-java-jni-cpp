package com.checkers.ui;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import java.awt.BorderLayout;
import java.awt.Font;

public class GameWindow extends JFrame {
    private final GameBoard gameBoard;
    private final JLabel statusLabel;

    public GameWindow() {
        setTitle("Checkers Game");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setResizable(false);

        // Create status panel
        JPanel statusPanel = new JPanel();
        statusLabel = new JLabel("Black's Turn");
        statusLabel.setFont(new Font("Arial", Font.BOLD, 16));
        statusPanel.add(statusLabel);

        // Layout setup
        setLayout(new BorderLayout(10, 10));
        gameBoard = new GameBoard(this::updateStatus);
        add(gameBoard, BorderLayout.CENTER);
        add(statusPanel, BorderLayout.SOUTH);

        pack();
        setLocationRelativeTo(null);
    }

    public void updateStatus(int currentPlayer) {
        String playerText = currentPlayer == 0 ? "Black" : "Red";
        statusLabel.setText(playerText + "'s Turn");
    }
}