package com.faustgames.forest;

import android.opengl.GLSurfaceView;
import android.service.wallpaper.WallpaperService;
import android.view.SurfaceHolder;

public abstract class GLWallpaperService extends WallpaperService 
{
    public class GLEngine extends WallpaperService.Engine 
    {
    	public final static int RENDERMODE_WHEN_DIRTY = 0;
    	public final static int RENDERMODE_CONTINUOUSLY = 1;
    	
    	private GLEnginePendingOperationsController controller = new GLEnginePendingOperationsController();

        public GLEngine() 
        {
        }

        public synchronized void setGLWrapper(final GLSurfaceView.GLWrapper glWrapper) {
        	controller.setGLWrapper(glWrapper);
        }

        public synchronized void setDebugFlags(final int debugFlags) {
        	controller.setDebugFlags(debugFlags);
        }

        public synchronized int getDebugFlags() {
        	return controller.getDebugFlags();
        }

        public synchronized void setRenderer(final GLSurfaceView.Renderer renderer) {
        	controller.setRenderer(renderer, isVisible());
        }

        public synchronized void queueEvent(final Runnable r) {
        }

        public synchronized void setEGLContextFactory(final GLSurfaceView.EGLContextFactory factory) {
        	controller.setEGLContextFactory(factory);
        }

        public synchronized void setEGLWindowSurfaceFactory(final GLSurfaceView.EGLWindowSurfaceFactory factory) {
        	controller.setEGLWindowSurfaceFactory(factory);
        }

        public synchronized void setEGLConfigChooser(final GLSurfaceView.EGLConfigChooser configChooser) {
        	controller.setEGLConfigChooser(configChooser);
        }

        public synchronized void setEGLConfigChooser(final boolean needDepth) {
        	controller.setEGLConfigChooser(needDepth);
        }

        public synchronized void setEGLConfigChooser(final int redSize, final int greenSize, final int blueSize,
            final int alphaSize, final int depthSize, final int stencilSize) {
        	controller.setEGLConfigChooser(redSize, greenSize, blueSize, alphaSize, depthSize, stencilSize);
        }

        public synchronized void setEGLContextClientVersion(final int version) {
        	controller.setEGLContextClientVersion(version);
        }

        public synchronized void setRenderMode(final int renderMode) {
        	controller.setRenderMode(renderMode);
        }

        public synchronized int getRenderMode() {
        	return controller.getRenderMode();
        }

        public synchronized void requestRender() {
        	controller.requestRender();
        }

        @Override
        public synchronized void onVisibilityChanged(final boolean visible) {
            super.onVisibilityChanged(visible);
            controller.VisibilityChanged(visible);
        }

        @Override
        public synchronized void onSurfaceChanged(final SurfaceHolder holder, final int format, final int width, final int height) {
        	super.onSurfaceChanged(holder, format, width, height);
        	controller.SurfaceChanged(holder, format, width, height);
        }

        @Override
        public synchronized void onSurfaceCreated(SurfaceHolder holder) 
        {
        	super.onSurfaceCreated(holder);
        	controller.SurfaceCreated(holder, GLWallpaperService.this, GLEngine.this);
        }

        @Override
        public synchronized void onSurfaceDestroyed(SurfaceHolder holder) {
        	controller.SurfaceDestroyed(holder);
        	super.onSurfaceDestroyed(holder);
        }
    }
}