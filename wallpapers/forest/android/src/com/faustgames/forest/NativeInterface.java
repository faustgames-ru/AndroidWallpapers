package com.faustgames.forest;

import android.content.res.AssetManager;

public class NativeInterface 
{
	static
	{
        System.loadLibrary("forest");
    }

	public native void onSurfaceCreated();
	public native void onSurfaceChanged(int width, int height);
	public native void onDrawFrame();
	public native void initAssetManager(AssetManager mng);
}
