package com.faustgames.forest;

import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

//android.os.Debug.waitForDebugger();

public class WPService extends GLWallpaperService
{
    private NativeInterface _ni = new NativeInterface();
    private WallpaperRenderer mRenderer = null;

    public class LiveGLEngine extends GLWallpaperService.GLEngine
    {
    	public LiveGLEngine()
    	{
    	}
    	@Override
        public synchronized void onTouchEvent(MotionEvent event) 
        {
    		super.onTouchEvent(event);
        }
    	@Override
        public synchronized void onVisibilityChanged(final boolean visible) 
    	{
            super.onVisibilityChanged(visible);
        }
    	@Override
        public synchronized void onSurfaceDestroyed(SurfaceHolder holder) {
    		super.onSurfaceDestroyed(holder);
        }
    }

    public class WallpaperRenderer implements GLSurfaceView.Renderer {

        public synchronized void onSurfaceCreated(GL10 unused, EGLConfig config) {
            WPService.this._ni.onSurfaceCreated();
        }

        public synchronized void onDrawFrame(GL10 unused) {
            WPService.this._ni.onDrawFrame();
        }

        public synchronized void onSurfaceChanged(GL10 unused, int width, int height) {
            WPService.this._ni.onSurfaceChanged(width, height);
        }
    }
    
    @Override
    public void onCreate()
    {
        super.onCreate();
		_ni.initAssetManager(getResources().getAssets());
    }

    @Override
    public void onDestroy() 
    {
        super.onDestroy();
    }

    @Override
    public Engine onCreateEngine()  
    {
    	mRenderer = new WallpaperRenderer();
		return new LiveGLEngine()
		{
			{
				setEGLContextClientVersion(2);
				setRenderer(mRenderer);
				setRenderMode(RENDERMODE_CONTINUOUSLY);
			}
		};
    }
}