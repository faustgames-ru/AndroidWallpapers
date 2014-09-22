package com.faustgames.forest;

import android.content.res.AssetManager;

public class NativeInterface 
{
	public native void initAssetManager(AssetManager mng);
	//public native void run();
	//public native void render();

	public native void onSurfaceCreated();
	public native void onSurfaceChanged(int width, int height);
	public native void onDrawFrame();
    
    static
	{
        System.loadLibrary("forest");
    }
}
