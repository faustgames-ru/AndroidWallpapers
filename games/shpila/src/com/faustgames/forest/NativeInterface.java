package com.faustgames.solarsystem;

import android.content.res.AssetManager;

public class NativeInterface 
{
	static
	{
        System.loadLibrary("solarsystem");
    }

	public native void onSurfaceCreated();
	public native void onSurfaceChanged(int width, int height);
	public native void onDrawFrame();
	public native void initAssetManager(AssetManager mng);
}
