#include <jni.h>

#ifndef _Included_com_checkers_core_NativeInterface
#define _Included_com_checkers_core_NativeInterface

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_checkers_core_NativeInterface_initGame
  (JNIEnv *, jobject);

JNIEXPORT jboolean JNICALL Java_com_checkers_core_NativeInterface_validateMove
  (JNIEnv *, jobject, jint, jint, jint, jint);

JNIEXPORT void JNICALL Java_com_checkers_core_NativeInterface_performMove
  (JNIEnv *, jobject, jint, jint, jint, jint);

JNIEXPORT jobjectArray JNICALL Java_com_checkers_core_NativeInterface_getBoardState
  (JNIEnv *, jobject);

JNIEXPORT jint JNICALL Java_com_checkers_core_NativeInterface_getCurrentPlayer
  (JNIEnv *, jobject);

JNIEXPORT jboolean JNICALL Java_com_checkers_core_NativeInterface_isGameOver
  (JNIEnv *, jobject);

JNIEXPORT jint JNICALL Java_com_checkers_core_NativeInterface_getWinner
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
