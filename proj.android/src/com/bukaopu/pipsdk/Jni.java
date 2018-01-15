/*
 * Copyright (c) 2014,  All rights reserved.
 */
package com.bukaopu.pipsdk;

import android.content.Context;

/**
 * @author Zeal
 */
public class Jni {
	
	static {
		System.loadLibrary("pipsdk");
	}
	
	public native String decrypt(Context ctx, String encryptString); 
	


}
