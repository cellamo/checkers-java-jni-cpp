#include "com_checkers_core_NativeInterface.h"
#include "Game.h"
#include <memory>

static std::unique_ptr<Game> game(nullptr);

JNIEXPORT void JNICALL Java_com_checkers_core_NativeInterface_initGame
  (JNIEnv* env, jobject obj) {
    game.reset(new Game());
}

JNIEXPORT jboolean JNICALL Java_com_checkers_core_NativeInterface_validateMove
  (JNIEnv* env, jobject obj, jint startX, jint startY, jint endX, jint endY) {
    return game->validateMove(startX, startY, endX, endY);
}

JNIEXPORT void JNICALL Java_com_checkers_core_NativeInterface_performMove
  (JNIEnv* env, jobject obj, jint startX, jint startY, jint endX, jint endY) {
    game->performMove(startX, startY, endX, endY);
}

JNIEXPORT jobjectArray JNICALL Java_com_checkers_core_NativeInterface_getBoardState
  (JNIEnv* env, jobject obj) {
    jclass intArrayClass = env->FindClass("[I");
    jobjectArray board = env->NewObjectArray(8, intArrayClass, nullptr);
    
    for (int y = 0; y < 8; y++) {
        jintArray row = env->NewIntArray(8);
        jint buffer[8];
        
        for (int x = 0; x < 8; x++) {
            Piece piece = game->getBoard().getPiece(x, y);
            int value = 0;
            if (piece.color == PieceColor::BLACK) value = piece.type == PieceType::KING ? 2 : 1;
            if (piece.color == PieceColor::RED) value = piece.type == PieceType::KING ? 4 : 3;
            buffer[x] = value;
        }
        
        env->SetIntArrayRegion(row, 0, 8, buffer);
        env->SetObjectArrayElement(board, y, row);
        env->DeleteLocalRef(row);
    }
    
    return board;
}

JNIEXPORT jint JNICALL Java_com_checkers_core_NativeInterface_getCurrentPlayer
  (JNIEnv* env, jobject obj) {
    return static_cast<jint>(game->getCurrentPlayer());
}

JNIEXPORT jboolean JNICALL Java_com_checkers_core_NativeInterface_isGameOver
  (JNIEnv* env, jobject obj) {
    return game->isGameOver();
}

JNIEXPORT jint JNICALL Java_com_checkers_core_NativeInterface_getWinner
  (JNIEnv* env, jobject obj) {
    return static_cast<jint>(game->getWinner());
}
