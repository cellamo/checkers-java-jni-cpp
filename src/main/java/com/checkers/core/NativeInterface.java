package com.checkers.core;

public class NativeInterface {
    static {
        String libraryPath = System.getProperty("user.dir") + "/build/lib/libCheckersGame.dylib";
        System.load(libraryPath);
    }

    public native void initGame();
    public native boolean validateMove(int startX, int startY, int endX, int endY);
    public native void performMove(int startX, int startY, int endX, int endY);
    public native int[][] getBoardState();
    public native int getCurrentPlayer();
    public native boolean isGameOver();
    public native int getWinner();
}
