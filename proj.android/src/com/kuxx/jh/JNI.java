package com.kuxx.jh;

public class JNI {

	public static native void sendMessage(int msg);
	public static native void saveData();
	public static native void changeGameStates(int status);
}
